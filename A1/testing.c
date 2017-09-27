
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "LinkedListAPI.h"
#include "CalendarParser.h"

void deleteEvent (Event * toDelete);


/* determines whether or no a line is completely composed of whitespace */
int isWhitespace (char * str){
    if (str == NULL)
        return -1;
    int len = strlen(str);
    int i = 0;
    for (i=0;i<len;i++){
        if (str[i] != ' ' && str[i] != '\t' && str[i] != '\n')
            return 0;
    }
    return 1;
}

/* checks the current line and its following line for signs that 
   there is a multi line */
int checkMultiLine (char * firstLine, char * secondLine){
    /*check if the first line reached its length limit */
    if (strlen(firstLine) >= 74){
        /* check if the second line has a : or ; and starts with whitespace */
        if (secondLine[0] == ' ' || secondLine[0] == '\t'){
            if (strchr(secondLine,':') == NULL && strchr(secondLine,';') == NULL){
                return 1;
            }
        }
    }
    return 0;
}

/* ------------------------Property------------------------ */

Property * initProperty(char * name, char * descr){
    Property * newProp = malloc(sizeof(Property) + (sizeof(int)*strlen(descr)));
    strcpy(newProp->propName, name);
    strcpy(newProp->propDescr,descr);
    return newProp;
}

char * printProperty(void * toBePrinted){
    Property * toPrint = (Property *)toBePrinted;
    int len = strlen(toPrint->propName)+strlen(toPrint->propDescr)+10;
    char * str = malloc(sizeof(char)*len);
    sprintf(str,"%s\n%s\n", toPrint->propName,toPrint->propDescr);
    return str;
}
//TEST
int compareProperty(const void * first,const void * second){
    char * name1 = ((Property *)first)->propName;
    char * name2 = ((Property *)second)->propName;
    return strcmp(name1, name2);
}

void deleteProperty(void * toDelete){
    free((Property *)toDelete);
}

/* ------------------------Alarm------------------------ */

Alarm * initAlarm(char * action, char * trigger, List propList){
    Alarm * newAlarm = malloc(sizeof(Alarm));
    newAlarm->trigger = malloc(sizeof(char)*(strlen(trigger)+1));
    strcpy(newAlarm->action, action);
    strcpy(newAlarm->trigger,trigger);
    newAlarm->properties = propList;
    return newAlarm;
}

char * printAlarm(void * toBePrinted){
    Alarm * toPrint = (Alarm *)toBePrinted;

    char * list = toString(toPrint->properties);
    int len = strlen(list)+strlen(toPrint->action)+strlen(toPrint->trigger)+10;
    char * str = malloc(sizeof(char)*len);
    sprintf(str,"%s\n%s\n%s\n", toPrint->action,toPrint->trigger,list);
    free(list);
    return str;
}
//TEST
int compareAlarm(const void * first,const void * second){
    char * action1 = ((Alarm *)first)->action;
    char * action2 = ((Alarm *)second)->action;
    return strcmp(action1, action2);
}

void deleteAlarm(void * toDelete){
    free(((Alarm *)toDelete)->trigger);
    free((Alarm *)toDelete);
}

/* ------------------------DateTime------------------------ */

DateTime * initDT (char * str){

    DateTime * newDT = malloc(sizeof(DateTime));

    char date[9] = "";
    char time[7] = "";

    if (strlen(str) == 15)
        newDT->UTC = true;
    else
        newDT->UTC = false;
    int i = 0;
    for (i=0;i<8;i++){
        date[i] = str[i];
    }
    int j = 0;
    i = 8;
    for (i=8;i<14;i++){
        time[j] = str[i];
        j++;
    }
    strcpy(newDT->date, time);
    strcpy(newDT->time, time);
    return newDT;
}

char * printDT(DateTime * dt){

    int len = strlen(dt->date)+strlen(dt->time)+6;
    char * dtStr = malloc(sizeof(char)*len);
    sprintf(dtStr, "%s, %s", dt->date, dt->time);
    if (dt->UTC == true)
        strcat(dtStr," UTC");
    return dtStr;
}

void deleteDT(DateTime * toDelete){
    free(toDelete);
}

/* ------------------------Calendar------------------------ */

Calendar * initCal (float ver, char * id, Event * event){
    Calendar * newCal = malloc(sizeof(Calendar));
    newCal->version = ver;
    strcpy(newCal->prodID, id);
    newCal->event = event;
    return newCal;
}

void deleteCal(Calendar * toDelete){
    deleteEvent(toDelete->event);
    free(toDelete);
}

/* ------------------------Event------------------------ */

Event * initEvent (char * uID, DateTime dt, List propList, List alarmList){
    Event * newEvent = malloc(sizeof(Event));
    strcpy(newEvent->UID,uID);
    newEvent->creationDateTime = dt;
    newEvent->properties = propList;
    newEvent->alarms = alarmList;
    return newEvent;
}

void deleteEvent (Event * toDelete){
    deleteDT(&toDelete->creationDateTime);
    clearList(&(toDelete->properties));
    clearList(&(toDelete->alarms));
    free(toDelete);
}

ErrorCode parseAlarm(FILE * fp, char * currentLine, Alarm ** alarmPtr){
    return OK;
}

void freeCal(char * value, FILE * fp){
    free(value);
    fclose(fp);
}

void freeEv(List *list1, List *list2, char * value){
    clearList(list1);
    clearList(list2);
    free(value);
}

ErrorCode parseEvent (FILE * fp,char * currentLine, Event ** eventPrt){
    // the file pointer will be pointing to the next line so we must pass the current line 

    List propList = initializeList(printProperty, deleteProperty, compareProperty);
    List alarmList = initializeList(printAlarm, deleteAlarm, compareAlarm);

    /* Keeps track of whether or DTSTAMP and UID 
    have been declared. There must be only one
    DTSTAMP declared and only one UID declared */
    int checkUID = 0;
    int checkDT = 0;
    /* hold the values of the DTSTAMP and UID */
    char * evUID;
    DateTime * evDT;
    /* parsing bits */
    char current[75];
    strcpy(current,currentLine);
    char next[75];
    char * hold = currentLine; /* this is so hold isn't NULL */
    int multi;
    fpos_t * filePos;
    fgetpos(fp,filePos);

    while (hold != NULL){
        hold = fgets(next,75,fp);
        /* make sure the line can be parsed */
        if (strchr(current,':') == NULL && strchr(current,';') == NULL){
            /* this handles the case where there are chracters but no : or ; 
               if the line is just whitespace it will be ignored */
            if (isWhitespace(current) != 1){
                clearList(&propList);
                clearList(&alarmList);
                return INV_EVENT;
            }

        } else {
            /* parse the line */
            char * token = strtok(current, ":; \t");
            char * holdVal = strtok(NULL, ":;\n");

            /* For comments it doesn't matter if the value is empty */
            if (strcmp(token,"COMMENT") == 0){
                /* if the line doesnt exist then it can't be a multi line */
                if (hold != NULL)
                    multi = checkMultiLine(current, next);
                else
                    multi = 0;
                while (multi == 1){
                    /* check if the line after the next line is also a multi line */
                    char buffer[75];
                    hold = fgets(buffer, 75, fp);
            
                    if (hold != NULL)
                        multi = checkMultiLine(next, buffer);
                    else 
                        multi = 0;
                    strcpy(next, buffer);
                }
            } else {
                /* check if the value following the key word is NULL */
                if (holdVal == NULL){
                    clearList(&propList);
                    clearList(&alarmList);
                    return INV_EVENT;
                }
                int len = strlen(holdVal) + 1;
                char * value = malloc(sizeof(char)*len);
                strcpy(value, holdVal);

                /* ------------Check for multi lines----------- */
                if (hold != NULL)
                    multi = checkMultiLine(current, next);
                else
                    multi = 0;
                while (multi == 1){ /* check if the line after the next line is also a multi line */
                    char buffer[75];
                    hold = fgets(buffer, 75, fp);
            
                    if (hold != NULL)
                        multi = checkMultiLine(next, buffer);
                    else 
                        multi = 0;
                    /* realloc and add the next line to the end of value */
                    char * temp = strtok(next, "\n");
                    temp++;
                    len = len + strlen(temp);
                    value = realloc(value, len);
                    strcat(value, temp);
                    strcpy(next, buffer);
                }
                /* ------------Identify Key Word----------- */
                if (strcmp(token, "DTSTAMP") == 0){
                    if (checkDT == 0){ /* make sure this is the only dtstamp */
                        evDT = initDT(value);
                        checkDT = 1;
                    } else {
                        freeEv(&propList, &alarmList, value);
                        deleteDT(evDT);
                        if (checkUID == 1)
                            free(evUID);
                        return INV_EVENT;
                    }
                } else if (strcmp(token,"UID")==0){
                    if (checkUID == 0){ /* make sure this is the only UID */
                        evUID = malloc (sizeof(char)*(strlen(value)+1));
                        strcpy(evUID, value);
                        checkUID = 1;
                    } else {
                        free(evUID);
                        freeEv(&propList, &alarmList, value);
                        if (checkDT == 1)
                            deleteDT(evDT);
                        return INV_EVENT;
                    }
                } else if (strcmp(token,"BEGIN")==0){ /* only allow one Event per calendar object */
                    if (strcmp(value, "ALARM") == 0 && checkUID == 1 && checkDT == 1){
                        //go to parseAlarm 
                        Alarm ** newAlarm = malloc(sizeof(Alarm*));
                        ErrorCode eCode = parseAlarm(fp, next, newAlarm);
                        //add alarm to the list

                        if (eCode != OK){
                            freeEv(&propList, &alarmList, value);
                            clearList(&propList);
                            clearList(&alarmList);
                            return eCode;
                        }
                    } else {
                        freeEv(&propList, &alarmList, value);
                        if (checkUID == 1)
                            free(evUID);
                        return INV_EVENT;
                    }

                } else if (strcmp(token,"END")==0){ /* don't want multiple ends */
                    if (strcmp(value, "VCALENDAR") == 0 && checkUID == 1 && checkDT == 1){
                        fsetpos(fp,filePos); // go back one line in the file 
                        //create a calendar object
                    } else {
                        freeEv(&propList, &alarmList, value);
                        if (checkUID == 1)
                            free(evUID);
                        return INV_EVENT;
                    }
                } else {
                    // handle properties

                    freeEv(&propList, &alarmList, value);
                    if (checkUID == 1)
                        free(evUID);
                    return INV_EVENT;
                }
            }
        }
        fgetpos(fp,filePos);
        strcpy(current,next);
    }

    
}

ErrorCode parseCalendar (FILE * fp, Calendar ** obj){
    /* Keeps track of whether or not there has been a 
    version and product ID declared. There must be only
    one version declared and only one product ID declared */
    int checkVer = 0;
    int checkID = 0;
    /* hold the values of the version and porduct ID */
    float calVer;
    char * calID;
    /* parsing bits */
    char current[75], next[75];
    char * hold = fgets(current,75,fp);
    int multi;

    int eventEnd = 0;
    int end = 0;
    Event ** eventPrt;

    while (hold != NULL){
        hold = fgets(next,75,fp);
        /* make sure the line can be parsed */
        if (strchr(current,':') == NULL && strchr(current,';') == NULL){
            /* this handles the case where there are chracters but no : or ; 
               if the line is just whitespace it will be ignored */
            if (isWhitespace(current) != 1) 
                return INV_CAL;
        } else {
            /* parse the line */
            char * token = strtok(current, ":; \t");
            char * holdVal = strtok(NULL, ":;\n");

            /* For comments it doesn't matter if the value is empty */
            if (strcmp(token,"COMMENT") == 0){
                /* if the line doesnt exist then it can't be a multi line */
                if (hold != NULL)
                    multi = checkMultiLine(current, next);
                else
                    multi = 0;
                while (multi == 1){
                    /* check if the line after the next line is also a multi line */
                    char buffer[75];
                    hold = fgets(buffer, 75, fp);
            
                    if (hold != NULL)
                        multi = checkMultiLine(next, buffer);
                    else 
                        multi = 0;
                    strcpy(next, buffer);
                }
            } else {
                /* check if the value following the key word is NULL */
                if (holdVal == NULL){
                    fclose(fp);
                    if (strcmp(token, "VERSION") == 0)
                        return INV_VER;
                    else if (strcmp(token, "PRODID") == 0)
                        return INV_PRODID;
                    else
                        return INV_CAL;
                }
                int len = strlen(holdVal) + 1;
                char * value = malloc(sizeof(char)*len);
                strcpy(value, holdVal);

                /* ------------Check for multi lines----------- */
                if (hold != NULL)
                    multi = checkMultiLine(current, next);
                else
                    multi = 0;
                while (multi == 1){ /* check if the line after the next line is also a multi line */
                    char buffer[75];
                    hold = fgets(buffer, 75, fp);
            
                    if (hold != NULL)
                        multi = checkMultiLine(next, buffer);
                    else 
                        multi = 0;
                    /* realloc and add the next line to the end of value */
                    char * temp = strtok(next, "\n");
                    temp++;
                    len = len + strlen(temp);
                    value = realloc(value, len);
                    strcat(value, temp);
                    strcpy(next, buffer);
                }
                /* ------------Identify Key Word----------- */
                if (strcmp(token, "VERSION") == 0){
                    if (checkVer == 0){
                        /* make sure this is the only version */
                        calVer = atof(value);
                        if (calVer <= 0){
                            free(value);
                            if (checkID == 1)
                                free(calID);
                            return INV_VER;
                        }
                        checkVer = 1;
                    } else {
                        free(value);
                        if (checkID == 1)
                            free(calID);
                        return DUP_VER;
                    }
                } else if (strcmp(token,"PRODID")==0){
                    if (checkID == 0){ /* make sure this is the only declaration of the version */
                        calID = malloc (sizeof(char)*(strlen(value)+1));
                        strcpy(calID, value);
                        checkID = 1;
                    } else {
                        free(calID);
                        free(value);
                        return DUP_PRODID;
                    }
                } else if (strcmp(token,"BEGIN")==0 && eventEnd == 0){ /* only allow one Event per calendar object */
                    if (strcmp(value, "VEVENT") == 0 && checkID == 1 && checkVer == 1){
                        //go to parseEvent 
                        eventPrt = malloc(sizeof(Event*));
                        printf("Going to parseEvent\n");
                        ErrorCode eCode = parseEvent(fp, next, eventPrt);
                        if (eCode != OK){
                            free(value);
                            return eCode;
                        }
                        eventEnd = 1;   
                    } else {
                        free(value);
                        if (checkID == 1)
                            free(calID);
                        return INV_CAL;
                    }

                } else if (strcmp(token,"END")==0 && end == 0){ /* don't want multiple ends */
                    if (strcmp(value, "VCALENDAR") == 0 && checkID == 1 && checkVer == 1 && eventEnd == 1){
                        end = 1;
                        //create a calendar object
                    } else {
                        free(value);
                        if (checkID == 1)
                            free(calID);
                        if (eventEnd == 1)
                            deleteEvent(*eventPrt);
                        return INV_CAL;
                    }
                } else {
                    free(value);
                    if (checkID == 1)
                        free(calID);
                    if (eventEnd == 1)
                        deleteEvent(*eventPrt);
                    return INV_CAL;
                }
                free(value);
            }
        }
        strcpy(current,next);
    }

    if (end == 1)
        return OK;
    else 
        return INV_CAL;
}

ErrorCode createCalendar(char* fileName, Calendar ** obj){
    /* check that the file exists and open it */
    FILE * fp = fopen(fileName,"r");
    if (fp == NULL)
        return INV_FILE;

    char current[75];
    char * hold = fgets(current,75,fp);

    while (hold != NULL){
        /* make sure the line can be parsed */
        if (strchr(current,':') == NULL && strchr(current,';') == NULL){
            /* this handles the case where there are chracters but no : or ; */
            if (isWhitespace(current) != 1){ 
                fclose(fp);
                return INV_CAL;
            }
            else 
                hold = fgets(current,75,fp);
        } else {
            /* parse the line */
            char * token = strtok(current, ":; \t");
            char * holdVal = strtok(NULL, ":;\n");
            int len = strlen(holdVal) + 1;
            char * value = malloc(sizeof(char)*len);
            strcpy(value, holdVal);

            /* this should be the beginning of the calendar object */
            if (strcmp(token, "BEGIN") == 0){
                /* if the next word is not VCALENDAR then the file is wrong
                and INV_CAL is returned */
                if (strcmp(value, "VCALENDAR") == 0){
                    ErrorCode eCode = parseCalendar(fp, obj);
                    freeCal(value, fp);
                    return eCode;
                } else {
                    freeCal(value, fp);
                    return INV_CAL;
                }
            } else {
                /* Comments can be ignored, anything else is invalid */
                if (strcmp(token, "COMMENT") != 0){
                    freeCal(value, fp);
                    return INV_CAL;
                }
                char next[75];
                hold = fgets(next,75,fp);
                int multi;
                /* if the line doesnt exist then it can't be a multi line */
                if (hold != NULL)
                    multi = checkMultiLine(current, next);
                else
                    multi = 0;
                /* check if the line after the next line is also a multi line */
                while (multi == 1){
                    char buffer[75];
                    hold = fgets(buffer, 75, fp);
                    if (hold != NULL)
                        multi = checkMultiLine(next, buffer);
                    else 
                        multi = 0;
                    strcpy(next, buffer);
                }
                strcpy(current,next);
            }
            free(value);
        }
    }
    fclose(fp);
    return INV_CAL;
}

const char * printError (ErrorCode err){
    if (err == INV_CAL)
        return "Invaid Calendar\n";
    if (err == OK)
        return "Ok\n";
    if (err == INV_FILE)
        return "Invaid File\n";
    if (err == INV_VER)
        return "Invaid Version\n";
    if (err == INV_PRODID)
        return "Invaid Product ID\n";
    if (err == INV_EVENT)
        return "Invaid Event\n";
    if (err == DUP_VER)
        return "Duplicate Version\n";
    if (err == DUP_PRODID)
        return "Duplicate Product ID\n";
    if (err == INV_CREATEDT)
        return "Invaid DateTime\n";
}

int main(int argc, char * argv[]){
    
    char * fileName = "simpleICFile.ics";
    Calendar ** cal;
    ErrorCode code =  createCalendar(fileName, cal);
    printf("%s\n", printError(code));


    return 0;
}
