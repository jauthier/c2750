
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

/* ------------------------Creating the Calendar------------------------ */

ErrorCode createCalendar(char* fileName, Calendar ** obj){
    /* have everything in one loop
       keep track of what state we are in
       state: 0 -> before calendar object has been created
       state: 1 -> in calendar object before event has been created
       state: 2 -> in event
       state: 3 -> in calendar after event has ended properly
       state: 4 -> out of calendar after it has ended properly
       state: 5 -> in an alarm

    */
    /* check that the file exists and open it */
    FILE * fp = fopen(fileName,"r");
    if (fp == NULL)
        return INV_FILE;

    char current[75], next[75];
    char * hold = fgets(current,75,fp);

    int multi;
    int state = 0, checkID = 0, checkVer = 0, checkUID = 0, checkDT = 0;

    char * calID;
    float calVer = 0.0;
    char * evID;
    DateTime dtStamp;
    Event * event = NULL;
    List propList, alarmList, alarmPropList;
    Calendar * tempCal;

    while (hold != NULL){
        hold = fgets(next,75,fp);
         /* make sure the line can be parsed */
        if (strchr(current,':') == NULL && strchr(current,';') == NULL){
            /* this handles the case where there are chracters but no : or ; */
            if (isWhitespace(current) != 1){ 
                fclose(fp);
                return INV_CAL;
            }
            /* this handles the case where there is only whitespace */
            else /* do we need to do this?? */
                hold = fgets(current,75,fp);
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
                    // there are more options for return values here---------------------------
                    if (state == 0)
                        return INV_CAL;
                    else 
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
                /* ------------Multi line check complete------------ */

                if (strcmp(token,"BEGIN")==0){
                    if (strcmp(value, "VCALENDAR") == 0){
                        if (state == 0){
                            state = 1;
                        } else {
                            fclose(fp);
                            free(value);
                            if (state == 1){
                                if (checkID == 1)
                                    free(calID);
                            } else if (state == 2 || state == 5){
                                free(calID);
                                if (checkUID == 1)
                                    free(evID);
                                if (checkDT == 1)
                                    deleteDT(&dtStamp);
                                clearList(&propList);
                                clearList(&alarmList);
                                if (state == 5)
                                    clearList(&alarmPropList);
                                return INV_EVENT;
                            } else if (state == 3){
                                free(calID);
                                deleteEvent(event);
                            } else if (state == 4){
                                deleteCal(tempCal);
                            }
                            return INV_CAL;
                        }
                    } else if (strcmp(value, "VEVENT") == 0){
                        if (state == 1){
                            if (checkID == 1 && checkVer == 1){
                                propList = initializeList(printProperty, deleteProperty, compareProperty);
                                alarmList = initializeList(printAlarm, deleteAlarm, compareAlarm);
                                state = 2;
                            } else {
                                fclose(fp);
                                free(value);
                                if (checkID == 1)
                                    free(calID);
                                return INV_CAL;
                            }
                        } else {
                            fclose(fp);
                            free(value);
                            if (state == 2 || state == 5){
                                free(calID);
                                if (checkUID == 1)
                                    free(evID);
                                if (checkDT == 1)
                                    deleteDT(&dtStamp);
                                clearList(&propList);
                                clearList(&alarmList);
                                if (state == 5)
                                    clearList(&alarmPropList);
                                return INV_EVENT;
                            } else if (state == 3){
                                free(calID);
                                deleteEvent(event);
                            } else if (state == 4){
                                deleteCal(tempCal);
                            }
                            return INV_CAL;
                        }
                    } else if (strcmp(value, "VALARM") == 0){
                        if (state == 2){
                            alarmPropList = initializeList(printProperty, deleteProperty, compareProperty);
                            state = 5;
                        } else {
                            fclose(fp);
                            free(value);
                            if (state == 1){
                                if (checkID == 1)
                                    free(calID);
                            } else if (state == 5){
                                free(calID);
                                if (checkUID == 1)
                                    free(evID);
                                if (checkDT == 1)
                                    deleteDT(&dtStamp);
                                clearList(&propList);
                                clearList(&alarmList);
                                clearList(&alarmPropList);
                                return INV_EVENT;
                            } else if (state == 3){
                                free(calID);
                                deleteEvent(event);
                            } else if (state == 4){
                                deleteCal(tempCal);
                            }
                            return INV_CAL;
                        }
                    } else {
                        fclose(fp);
                        free(value);
                        if (state == 1){
                            if (checkID == 1)
                                free(calID);
                        } else if (state == 5){
                            free(calID);
                            if (checkUID == 1)
                                free(evID);
                            if (checkDT == 1)
                                deleteDT(&dtStamp);
                            clearList(&propList);
                            clearList(&alarmList);
                            clearList(&alarmPropList);
                            return INV_EVENT;
                        } else if (state == 3){
                            free(calID);
                            deleteEvent(event);
                        } else if (state == 4){
                            deleteCal(tempCal);
                        }
                        return INV_CAL;
                    }
                } else if (strcmp(token,"VERSION")==0){
                    if (state == 1){
                        if (checkVer == 0){ 
                        /* make sure this is the only declaration of the version */    
                            calVer = atof(value);
                            if (calVer < 0){
                                fclose(fp);
                                return INV_VER;
                            }
                            checkVer = 1;
                        } else {
                            if (checkID == 1)
                                free(calID);
                            free(value);
                            fclose(fp);
                            return DUP_VER;
                        }
                    } else {
                        fclose(fp);
                        free(value);
                        if (state == 2 ||state == 5){
                            free(calID);
                            if (checkUID == 1)
                                free(evID);
                            if (checkDT == 1)
                                deleteDT(&dtStamp);
                            clearList(&propList);
                            clearList(&alarmList);
                            if (state == 5)
                                clearList(&alarmPropList);
                            return INV_EVENT;
                        } else if (state == 3){
                            free(calID);
                            deleteEvent(event);
                        } else if (state == 4){
                            deleteCal(tempCal);
                        }
                        return INV_CAL;
                    }
                } else if (strcmp(token,"PRODID")==0){
                    if (state == 1){
                        if (checkID == 0){ /* make sure this is the only declaration of the version */
                            calID = malloc (sizeof(char)*(strlen(value)+1));
                            strcpy(calID, value);
                            checkID = 1;
                        } else {
                            free(calID);
                            free(value);
                            fclose(fp);
                            return DUP_PRODID;
                        }
                    } else {
                        fclose(fp);
                        free(value);
                        if (state == 2 ||state == 5){
                            free(calID);
                            if (checkUID == 1)
                                free(evID);
                            if (checkDT == 1)
                                deleteDT(&dtStamp);
                            clearList(&propList);
                            clearList(&alarmList);
                            if (state == 5)
                                clearList(&alarmPropList);
                            return INV_EVENT;
                        } else if (state == 3){
                            free(calID);
                            deleteEvent(event);
                        } else if (state == 4){
                            deleteCal(tempCal);
                        }
                        return INV_CAL;
                    }
                } else if (strcmp(token,"UID")==0||strcmp(token,"uid")==0){
                    if (state == 2){
                        if (checkUID == 0){ /* make sure this is the only declaration of the version */
                            evID = malloc (sizeof(char)*(strlen(value)+1));
                            strcpy(evID, value);
                            checkUID = 1;
                        } else {
                            fclose(fp);
                            free(value);
                            free(calID);
                            if (checkUID == 1)
                                free(evID);
                            if (checkDT == 1)
                                deleteDT(&dtStamp);
                            clearList(&propList);
                            clearList(&alarmList);
                            return INV_EVENT;
                        }
                    } else {
                        fclose(fp);
                        free(value);
                        if (state == 1){
                            if (checkID == 1)
                                free(calID);
                        } else if (state == 5){
                            free(calID);
                            if (checkUID == 1)
                                free(evID);
                            if (checkDT == 1)
                                deleteDT(&dtStamp);
                            clearList(&propList);
                            clearList(&alarmList);
                            clearList(&alarmPropList);
                            return INV_EVENT;
                        } else if (state == 3){
                            free(calID);
                            deleteEvent(event);
                        } else if (state == 4){
                            deleteCal(tempCal);
                        }
                        return INV_CAL;
                    }
                } else if (strcmp(token,"DTSTAMP")==0||strcmp(token,"dtstamp")==0){
                    if (state == 2){
                        if (checkDT == 0){ /* make sure this is the only declaration of the version */
                            //make dt struct
                            checkDT = 1;
                        } else {
                            fclose(fp);
                            free(value);
                            free(calID);
                            if (checkUID == 1)
                                free(evID);
                            if (checkDT == 1)
                                deleteDT(&dtStamp);
                            clearList(&propList);
                            clearList(&alarmList);
                            return INV_EVENT;
                        }
                    } else {
                        fclose(fp);
                        free(value);
                        if (state == 1){
                            if (checkID == 1)
                                free(calID);
                        } else if (state == 5){
                            free(calID);
                            if (checkUID == 1)
                                free(evID);
                            if (checkDT == 1)
                                deleteDT(&dtStamp);
                            clearList(&propList);
                            clearList(&alarmList);
                            clearList(&alarmPropList);
                            return INV_EVENT;
                        } else if (state == 3){
                            free(calID);
                            deleteEvent(event);
                        } else if (state == 4){
                            deleteCal(tempCal);
                        }
                        return INV_CAL;
                    }
                } else if (strcmp(token,"END")==0){
                    if (strcmp(value, "VCALENDAR") == 0){
                        if (state == 3){
                            /* must have PID, Version and Event struct*/
                            if (checkID == 1 && checkVer == 1 && event != NULL) {
                                tempCal = initCal(calVer, calID, event);
                                *obj = tempCal;
                                state = 4;
                            } else {
                                fclose(fp);
                                free(value);
                                free(calID);
                                deleteEvent(event);
                                return INV_CAL;
                            }
                        } else {
                            fclose(fp);
                            free(value);
                            if (state == 1){
                                if (checkID == 1)
                                    free(calID);
                            } else if (state == 2 || state == 5){
                                free(calID);
                                if (checkUID == 1)
                                    free(evID);
                                if (checkDT == 1)
                                    deleteDT(&dtStamp);
                                clearList(&propList);
                                clearList(&alarmList);
                                if (state == 5)
                                    clearList(&alarmPropList);
                                return INV_EVENT;
                            } else if (state == 4){
                                deleteCal(tempCal);
                            }
                            return INV_CAL;
                        }
                    } else if (strcmp(value, "VEVENT") == 0){
                        if (state == 2){
                            if (checkUID == 1 && checkDT == 1){
                                event = initEvent(evID,dtStamp,propList,alarmList);
                                state = 3;
                            } else {
                                fclose(fp);
                                free(calID);
                                if (checkUID == 1)
                                    free(evID);
                                if (checkDT == 1)
                                    deleteDT(&dtStamp);
                                clearList(&propList);
                                clearList(&alarmList);
                                return INV_EVENT;
                            }
                        } else {
                            fclose(fp);
                            free(value);
                            if (state == 1){
                                if (checkID == 1)
                                    free(calID);
                            } else if (state == 5){
                                free(calID);
                                if (checkUID == 1)
                                    free(evID);
                                if (checkDT == 1)
                                    deleteDT(&dtStamp);
                                clearList(&propList);
                                clearList(&alarmList);
                                clearList(&alarmPropList);
                                return INV_EVENT;
                            } else if (state == 3){
                                free(calID);
                                deleteEvent(event);
                            } else if (state == 4){
                                deleteCal(tempCal);
                            }
                            return INV_CAL;
                        }
                    } else if (strcmp(value, "VALARM") == 0){
                        if (state == 5){
                            if (checkUID == 1 && checkDT == 1){
                                state = 2;
                            } else {
                                fclose(fp);
                                free(value);
                                free(calID);
                                if (checkUID == 1)
                                    free(evID);
                                if (checkDT == 1)
                                    deleteDT(&dtStamp);
                                clearList(&propList);
                                clearList(&alarmList);
                                clearList(&alarmPropList);
                                return INV_EVENT;
                            }
                        } else {
                            fclose(fp);
                            free(value);
                            if (state == 1){
                                if (checkID == 1)
                                    free(calID);
                            } else if (state == 2){
                                free(calID);
                                if (checkUID == 1){
                                    free(evID);
                                }
                                if (checkDT == 1){
                                    deleteDT(&dtStamp);
                                }
                                clearList(&propList);
                                clearList(&alarmList);
                                return INV_EVENT;
                            } else if (state == 3){
                                free(calID);
                                deleteEvent(event);
                            } else if (state == 4){
                                deleteCal(tempCal);
                            }
                            return INV_CAL;
                        }
                    } else {
                        fclose(fp);
                        free(value);
                        if (state == 2 ||state == 5){
                            free(calID);
                            if (checkUID == 1)
                                free(evID);
                            if (checkDT == 1)
                                deleteDT(&dtStamp);
                            clearList(&propList);
                            clearList(&alarmList);
                            if (state == 5)
                                clearList(&alarmPropList);
                            return INV_EVENT;
                        } else if (state == 3){
                            free(calID);
                            deleteEvent(event);
                        } else if (state == 4){
                            deleteCal(tempCal);
                        }
                        return INV_CAL;
                    }
                } else {
                    //properties
                    printf("prop\n");
                }
            }
        }
        strcpy(current,next);
    }
    
    fclose(fp);

    if (state == 4)
        return OK;
    else
        return INV_CAL;
}


void deleteCalendar(Calendar* obj){

}

char* printCalendar(const Calendar* obj){

}

const char * printError (ErrorCode err){

    char buffer[100];
    if (err == INV_CAL){
        
        strcpy(buffer,"Invaid Calendar");
    }
    if (err == OK)
        strcpy(buffer,"Ok\n");
    if (err == INV_FILE)
        strcpy(buffer,"Invaid File\n");
    if (err == INV_VER)
        strcpy(buffer,"Invaid Version\n");
    if (err == INV_PRODID)
        strcpy(buffer,"Invaid Product ID\n");
    if (err == INV_EVENT)
        strcpy(buffer,"Invaid Event\n");
    if (err == DUP_VER)
        strcpy(buffer,"Duplicate Version\n");
    if (err == DUP_PRODID)
        strcpy(buffer,"Duplicate Product ID\n");
    if (err == INV_CREATEDT)
        strcpy(buffer,"Invaid DateTime\n");

    char * error = malloc(sizeof(char)*strlen(buffer));
    strcpy(error, buffer);
    return error;
}

int main(int argc, char const *argv[])
{
    char * file = "simpleICFile.ics";
    Calendar ** cal = malloc(sizeof(Calendar*));

    ErrorCode ec = createCalendar(file,cal);
    const char * error = printError(ec);
    printf("%s\n", error);

    return 0;
}
