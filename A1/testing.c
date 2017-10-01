
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "LinkedListAPI.h"
#include "CalendarParser.h"




void deleteEvent (Event * toDelete);
char * printEvent(Event * event);


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
    strcpy(newDT->date, date);
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

void deleteCalendar(Calendar * obj){
    deleteEvent(obj->event);
    free(obj);
}

char* printCalendar(const Calendar* obj){
	char * str;
	char * event = printEvent(obj->event);
	int len = strlen(obj->prodID) + strlen(obj->event->UID) + strlen(event) + 40; 
	str = malloc(sizeof(char)*len);
	sprintf(str, "Product ID: %s\nVersion: %ld\n", obj->prodID, obj->version);
	free(event);
	return str;
}

/* ------------------------Event------------------------ */

Event * initEvent (char * uID, DateTime * dt, List propList, List alarmList){
    Event * newEvent = malloc(sizeof(Event));
    strcpy(newEvent->UID,uID);
    newEvent->creationDateTime = *dt;
    newEvent->properties = propList;
    newEvent->alarms = alarmList;
    return newEvent;
}

void deleteEvent (Event * toDelete){
    /*deleteDT(&(toDelete->creationDateTime));*/
    clearList(&(toDelete->properties));
    clearList(&(toDelete->alarms));
    free(toDelete);
}

char * printEvent(Event * event){
	char * str;
	char * dt = printDT(&(event->creationDateTime));
	char * list1 = toString(event->properties);
	char * list2 = toString(event->alarms);
	int len = strlen(dt) + strlen(list1) + strlen(list2) + strlen(event->UID) + 60;
	str = malloc(sizeof(char)*len);
	sprintf(str,"UID: %s\nDate and Time of Creation: %s\nAlarms:\n%s\nProperties:\n%s\n",event->UID,dt,list2,list1);
	free(dt);
	free(list1);
	free(list2);
	return str;
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

/* returns 0 if the property is not valid
   returns 1 if the property is valid
*/
int evPropCheck(Property * prop, List propList){
    char * propName = prop->propName;    

    if (strcmp(propName,"DSTART")==0||strcmp(propName,"CLASS")==0||strcmp(propName,"CREATED")==0||
        strcmp(propName,"DESCRIPTION")==0||strcmp(propName,"GEO")==0||strcmp(propName,"LAST-MOD")==0||
        strcmp(propName,"LOCATION")==0||strcmp(propName,"ORGANIZER")==0||strcmp(propName,"SEQ")==0||
        strcmp(propName,"PRIORITY")==0||strcmp(propName,"STATUS")==0||strcmp(propName,"SUMMARY")==0||
        strcmp(propName,"TRANSP")==0||strcmp(propName,"URL")==0||strcmp(propName,"RECURID")==0||
        strcmp(propName,"RRULE")==0){

        //check the list for duplicates
        int check = findElement((void*)prop, propList);
        if (check == 1)
            return 0;
        else 
            return 1;
    } else if (strcmp(propName,"DURATION")==0||strcmp(propName,"DTEND")==0){
        //check the list for the other
        //need to make a temporary struct of the other
        Property *temp;
        if  (strcmp(propName,"DURATION")==0){
            temp = initProperty("DTEND","temp");
        } else {
            temp = initProperty("DURATION","temp");
        }
        int check = findElement((void*)temp, propList);
        free(temp);
        if (check == 1)
            return 0;
        else 
            return 1;
    } else if (strcmp(propName,"ATTACH")==0||strcmp(propName,"ATTENDEE")==0||strcmp(propName,"CATEGORIES")==0||
        strcmp(propName,"COMMENT")==0||strcmp(propName,"CONTACT")==0||strcmp(propName,"EXDATE")==0||
        strcmp(propName,"RSTATUS")==0||strcmp(propName,"RELATED")==0||strcmp(propName,"REASOURSES")==0||
        strcmp(propName,"RDATE")==0||strcmp(propName,"X-PROP")==0||strcmp(propName,"IANA-PROP")==0){

        return 1;
    } else {
        return 0;
    }
}

ErrorCode parseAlarm(FILE * fp, char * currentLine, Alarm ** alarmPtr, char * holdLong){

    List propList = initializeList(printProperty, deleteProperty, compareProperty);

    int checkAction = 0;
    int checkTrigger = 0;
    int checkDuration = 0;
    int checkRepeat = 0;

    char * action;
    char * trigger;

    /* parsing bits */
    char current[75];
    strcpy(current,currentLine);
    char next[75];
    char * hold = currentLine; /* this is so hold isn't NULL */
    int multi;
    long pos;

    while (hold != NULL){
    	pos = ftell(fp);
        hold = fgets(next,75,fp);
        /* make sure the line can be parsed */
        if (strchr(current,':') == NULL && strchr(current,';') == NULL){
            /* this handles the case where there are chracters but no : or ; 
               if the line is just whitespace it will be ignored */
            if (isWhitespace(current) != 1){
                clearList(&propList);
                if (checkAction == 1)
                	free(action);
                if  (checkTrigger == 1)
                	free(trigger);
                return INV_EVENT;
            }

        } else {
        	/* parse the line */
            char * token = strtok(current, ":;\t");
            char * holdVal = strtok(NULL, "\n");

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
                    if (checkAction == 1)
                		free(action);
                	if  (checkTrigger == 1)
                		free(trigger);
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
                if (strcmp(token,"ACTION")==0){
                	if (checkAction == 0){
                		action = malloc(sizeof(char)*(strlen(value)+1));
                		strcpy(action, value);
                		checkAction = 1;
                	} else {
                		free(value);
                		clearList(&propList);
                		free(action);
                		if (checkTrigger == 1)
                			free(trigger);
                		return INV_EVENT;
                	}
                } else if (strcmp(token,"TRIGGER")==0){
                	if (checkTrigger == 0){
                		trigger = malloc(sizeof(char)*(strlen(value)+1));
                		strcpy(trigger,value);
                		checkTrigger = 1;
                	} else {
                		free(value);
                		clearList(&propList);
                		free(trigger);
                		if (checkAction == 1)
                			free(action);
                		return INV_EVENT;
                	}
                } else if (strcmp(token,"END")==0){
                	if (strcmp(value,"VALARM")==0&&checkAction == 1 && checkAction == 1){
                		if (checkDuration == 1 || checkRepeat == 1){
                			if (!(checkDuration == 1 && checkRepeat == 1)){
                				free(value);
                				clearList(&propList);
                				free(trigger);
                				free(action);
                				return INV_EVENT;
                			}
                		}
                		// the alarm is good to be made
                		Alarm * newAlarm = initAlarm(action, trigger, propList);
                		*alarmPtr = newAlarm;
                		sprintf(holdLong,"%ld",pos);
                        free(value);
                        free(action);
                        free(trigger);
                		return OK;
                	} else {
                		free(value);
                		clearList(&propList);
                		if (checkTrigger == 1)
                			free(trigger);
                		if (checkAction == 1)
                			free(action);
                		return INV_EVENT;
                	}

                } else {
                	free(value);
                	clearList(&propList);
                	if (checkTrigger == 1)
                		free(trigger);
               		if (checkAction == 1)
               			free(action);
                	return INV_EVENT;
                }
                free(value);
            }
        }
        strcpy(current,next);
    }
    return INV_EVENT;
}

ErrorCode parseEvent (FILE * fp,char * currentLine, Event ** eventPtr, char * holdLong){
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
    long pos;
    
    
    while (hold != NULL){
    	pos = ftell(fp);
        hold = fgets(next,75,fp);
        /* make sure the line can be parsed */
        if (strchr(current,':') == NULL && strchr(current,';') == NULL){
            /* this handles the case where there are chracters but no : or ; 
               if the line is just whitespace it will be ignored */
            if (isWhitespace(current) != 1){
                clearList(&propList);
                clearList(&alarmList);
                if (checkUID == 1)
                	free(evUID);
                if  (checkDT == 1)
                	deleteDT(evDT);
                return INV_EVENT;
            }

        } else {
            /* parse the line */
            char * token = strtok(current, ":;\t");
            char * holdVal = strtok(NULL, "\n");

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
                    if (checkUID == 1)
                		free(evUID);
                	if  (checkDT == 1)
                		deleteDT(evDT);
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
                    if (strcmp(value, "VALARM") == 0 && checkUID == 1 && checkDT == 1){
                        //go to parseAlarm 
                        Alarm ** newAlarm = malloc(sizeof(Alarm*));
                        char * holdLong = malloc(sizeof(char)*10);
                        ErrorCode eCode = parseAlarm(fp, next, newAlarm, holdLong);
                        //add alarm to the list
                        insertFront(&alarmList, *newAlarm);
                        free(newAlarm);
                        if (eCode != OK){
                            freeEv(&propList, &alarmList, value);
                            if (checkUID == 1)
                            	free(evUID);
                            if (checkDT == 1)
                            	deleteDT(evDT);
                            return eCode;
                        }
                        long pos = atol(holdLong);
                        fseek(fp,pos,SEEK_SET);
                        fgets(next,75,fp);
                        free(holdLong);
                    } else {
                        freeEv(&propList, &alarmList, value);
                        if (checkUID == 1)
                            free(evUID);
                        if (checkDT == 1)
                            deleteDT(evDT);
                        return INV_EVENT;
                    }

                } else if (strcmp(token,"END")==0){ /* don't want multiple ends */
                    if (strcmp(value, "VEVENT") == 0 && checkUID == 1 && checkDT == 1){
                        //create a event object
                        Event * temp = initEvent(evUID,evDT,propList,alarmList);
                        *eventPtr = temp;
                        sprintf(holdLong,"%ld",pos);
                        free(value);
                        free(evUID);
                        deleteDT(evDT);
                        return OK;
                    } else {
                        freeEv(&propList, &alarmList, value);
                        if (checkUID == 1)
                            free(evUID);
                        if (checkDT == 1)
                            deleteDT(evDT);
                        return INV_EVENT;
                    }
                } else {
                    // handle properties
                	// create a property
                	Property * newProp = initProperty(token,value);
                	//check if the property is valid
                	int check = evPropCheck(newProp,propList);

                	if (check == 1){
                		insertFront(&propList, (void *)newProp);
                	} else {
                		deleteProperty(newProp);
	                    freeEv(&propList, &alarmList, value);
    	                if (checkUID == 1)
        	                free(evUID);
            	        if (checkDT == 1)
                	        deleteDT(evDT);
                    	return INV_EVENT;
                	}
                }
                free(value);
            }
        }
        strcpy(current,next);
    }
    return INV_EVENT;
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
    Event ** eventPtr;

    while (hold != NULL){
        hold = fgets(next,75,fp);
        /* make sure the line can be parsed */
        if (strchr(current,':') == NULL && strchr(current,';') == NULL){
            /* this handles the case where there are chracters but no : or ; 
               if the line is just whitespace it will be ignored */
            if (isWhitespace(current) != 1){
            	if (eventEnd == 1)
            		free(eventPtr);
                return INV_CAL;
            } 
        } else {
            /* parse the line */
            char * token = strtok(current, ":;\t");
            char * holdVal = strtok(NULL, "\n");

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
                    if (eventEnd == 1)
            			free(eventPtr);
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
                if (strcmp(token, "VERSION") == 0 && eventEnd == 0){
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
                } else if (strcmp(token,"PRODID")==0 && eventEnd == 0){
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
                        eventPtr = malloc(sizeof(Event*));
                        char * holdLong = malloc(sizeof(char)*10);
                        ErrorCode eCode = parseEvent(fp, next, eventPtr,holdLong);
                        if (eCode != OK){
                        	free(eventPtr);
                            free(value);
                            free(calID);
                            free(holdLong);
                            return eCode;
                        }
                        long pos = atol(holdLong);
                        fseek(fp,pos,SEEK_SET);
                        fgets(next,75,fp);
                        free(holdLong);
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
                        Calendar * newCal = initCal(calVer,calID,*eventPtr);
                        *obj = newCal;
                        free(eventPtr);
                    } else {
                        free(value);
                        if (checkID == 1)
                            free(calID);
                        if (eventEnd == 1)
                            deleteEvent(*eventPtr);
                        return INV_CAL;
                    }
                } else {
                    free(value);            			
                    if (checkID == 1)
                        free(calID);
                    if (eventEnd == 1){
                        deleteEvent(*eventPtr);
                    	free(eventPtr);
                    }
                    return INV_CAL;
                }
                free(value);
            }
        }
        strcpy(current,next);
    }
    free(calID);
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
            char * token = strtok(current, ":;\t");
            char * holdVal = strtok(NULL, "\n");
            int len = strlen(holdVal) + 1;
            char * value = malloc(sizeof(char)*len);
            strcpy(value, holdVal);

            /* this should be the beginning of the calendar object */
            if (strcmp(token, "BEGIN") == 0){
                /* if the next word is not VCALENDAR then the file is wrong
                and INV_CAL is returned */
                if (strcmp(value, "VCALENDAR") == 0){
                    ErrorCode eCode = parseCalendar(fp,obj);
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
    return "Other Error\n";
}

int main(int argc, char * argv[]){
    
    char * fileName = "simpleICFile.ics";
    Calendar ** cal = malloc(sizeof(Calendar*));
    ErrorCode code =  createCalendar(fileName, cal);
    printf("%s\n", printError(code));


    if (code == OK){
    	char * hold = printCalendar(*cal);
    	printf("%s\n", hold);
    	deleteCalendar(*cal);
    }
    free(cal);

    return 0;
}
