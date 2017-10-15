#include "CalendarParser.h"
#include "readFile.h"
#include <ctype.h>

void deleteEvent (Event * toDelete);
char * printEvent(Event * event);
char * toUpper(char * str);

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
    sprintf(str,"      %s:%s", toPrint->propName,toPrint->propDescr);
    return str;
}

int compareProperty(const void * first,const void * second){
    char * name1 = toUpper(((Property *)first)->propName);
    char * name2 = toUpper(((Property *)second)->propName);
    int check = strcmp(name1, name2);
    free(name1);
    free(name2);
    return check;
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
    int len = strlen(toPrint->action)+strlen(toPrint->trigger)+50;
    if (list != NULL)
        len = len + strlen(list);
    char * str = malloc(sizeof(char)*len);

    if (list == NULL)
        sprintf(str,"      Action:%s\n      Trigger:%s\n", toPrint->action,toPrint->trigger);
    else 
        sprintf(str,"      Action:%s\n      Trigger:%s\n      Properties:\n%s\n", toPrint->action,toPrint->trigger,list);

    free(list);
    return str;
}

int compareAlarm(const void * first,const void * second){
    char * action1 = ((Alarm *)first)->action;
    char * action2 = ((Alarm *)second)->action;
    return strcmp(action1, action2);
}

void deleteAlarm(void * toDelete){

    free(((Alarm *)toDelete)->trigger);
    clearList(&(((Alarm *)toDelete)->properties));
    free((Alarm *)toDelete);
}

/* ------------------------DateTime------------------------ */
DateTime * initDT (char * str){

    DateTime * newDT = malloc(sizeof(DateTime));

    char date[9] = "";
    char time[7] = "";

    if (strlen(str) == 16)
        newDT->UTC = true;
    else
        newDT->UTC = false;
    int i = 0;
    for (i=0;i<8;i++){
        date[i] = str[i];
    }
    int j = 0;
    i = 8;
    for (i=9;i<15;i++){
        time[j] = str[i];
        j++;
    }
    strcpy(newDT->date, date);
    strcpy(newDT->time, time);
    return newDT;
}

char * printDT(DateTime * dt){

    int len = strlen(dt->date)+strlen(dt->time)+10;
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
    int len = strlen(obj->prodID) + strlen(event) + 50; 
    str = malloc(sizeof(char)*len);
    sprintf(str, "Calendar\n  Product ID: %s\n  Version: %0.1f\n%s", obj->prodID, obj->version,event);
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
    clearList(&(toDelete->properties));
    clearList(&(toDelete->alarms));
    free(toDelete);
}

char * printEvent(Event * event){
    char * str;
    char * dt = printDT(&(event->creationDateTime));
    char * list1 = toString(event->properties);
    char * list2 = toString(event->alarms);
    int len = strlen(dt) + strlen(event->UID) + 80;
    if (list1 != NULL)
        len = len +  strlen(list1);
    if (list2 != NULL)
        len = len + strlen(list2);
    str = malloc(sizeof(char)*len);
    if (list1 != NULL && list2 != NULL)
        sprintf(str,"  Event\n    UID: %s\n    Date and Time of Creation: %s\n    Alarms:\n%s    Properties:\n%s",event->UID,dt,list2,list1);
    else if (list1 != NULL && list2 == NULL)
        sprintf(str,"  Event\n    UID: %s\n    Date and Time of Creation: %s\n    Properties:\n%s",event->UID,dt,list1);
    else if (list1 == NULL && list2 != NULL)
        sprintf(str,"  Event\n    UID: %s\n    Date and Time of Creation: %s\n    Alarms:\n%s",event->UID,dt,list2);
    else 
        sprintf(str,"  Event\n    UID: %s\n    Date and Time of Creation: %s\n",event->UID,dt);

    free(dt);
    free(list1);
    free(list2);
    return str;
}

char * toUpper(char * str){
    if (str == NULL)
        return NULL;
    int len = strlen(str);

    char * result =  malloc(sizeof(char)*(len+1));
    int i = 0;
    for (i = 0; i < len; ++i){
        result[i] = toupper(str[i]);
    }
    result[len] = '\0';
    return result;
}

int evPropCheck(Property * prop, List propList){
    char * propName = toUpper(prop->propName);
    if (strcmp(propName,"DTSTART")==0||strcmp(propName,"CLASS")==0||strcmp(propName,"CREATED")==0||
        strcmp(propName,"DESCRIPTION")==0||strcmp(propName,"GEO")==0||strcmp(propName,"LAST-MOD")==0||
        strcmp(propName,"LOCATION")==0||strcmp(propName,"ORGANIZER")==0||strcmp(propName,"SEQ")==0||
        strcmp(propName,"PRIORITY")==0||strcmp(propName,"STATUS")==0||strcmp(propName,"SUMMARY")==0||
        strcmp(propName,"TRANSP")==0||strcmp(propName,"URL")==0||strcmp(propName,"RECURID")==0||
        strcmp(propName,"RRULE")==0){

        //check the list for duplicates
        int check = findElement((void*)prop, propList);
        free(propName);
        if (check == 1)
            return 0;
        else 
            return 1;
    } else if (strcmp(propName,"DURATION")==0||strcmp(propName,"DTEND")==0){
        //check the list for the other
        //need to make a temporary struct of the other
        Property * temp = initProperty("DTEND","temp");
        Property * temp2 = initProperty("DURATION","temp");
        int check = findElement((void*)temp, propList);
        int check2 = findElement((void*)temp2, propList);
        free(temp);
        free(temp2);
        free(propName);
        if (check == 0 && check2 == 0)
            return 1;
        else 
            return 0;
    } else if (strcmp(propName,"ATTACH")==0||strcmp(propName,"ATTENDEE")==0||strcmp(propName,"CATEGORIES")==0||
        strcmp(propName,"COMMENT")==0||strcmp(propName,"CONTACT")==0||strcmp(propName,"EXDATE")==0||
        strcmp(propName,"RSTATUS")==0||strcmp(propName,"RELATED")==0||strcmp(propName,"REASOURSES")==0||
        strcmp(propName,"RDATE")==0||strcmp(propName,"X-PROP")==0||strcmp(propName,"IANA-PROP")==0){
        free(propName);
        return 1;
    } else {
        free(propName);
        return 1;
    }
}

void free1(char * value, List * list){
    clearList(list);
    free(list);
    free(value);
}

void freeEv(List *list1, List *list2, char * value){
    clearList(list1);
    clearList(list2);
    free(value);
}

ErrorCode parseAlarm(Node * current, Alarm ** alarmPtr, Node ** returnPos){
    current = current->next;

    List propList = initializeList(printProperty, deleteProperty, compareProperty);

    int checkAction = 0;
    int checkTrigger = 0;
    int checkDuration = 0;
    int checkRepeat = 0;

    char * action;
    char * trigger;

    while(current != NULL){

        char * line = (char *)current->data;
        printf("%s\n", line);

        if (strchr(line,':') == NULL && strchr(line,';') == NULL){
            clearList(&propList);
            if (checkAction == 1)
                free(action);
            if  (checkTrigger == 1)
                free(trigger);
            return INV_EVENT;
        }

        /* parse the line */
        char * token = strtok(line, ":;\t");
        char * holdVal = strtok(NULL, "\n");
        
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
            if (strcmp(value,"VALARM")==0 &&checkAction == 1 && checkTrigger == 1){
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
                *returnPos = current;
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
            // porperties - eliminate case sensitivity
            char * temp = toUpper(token);
            if (strcmp(temp,"DURATION")==0||strcmp(temp,"REPEAT")==0||strcmp(temp,"ATTACH")==0
                ||strcmp(temp,"DESCRIPTION")==0||strcmp(temp,"SUMMARY")==0){

                Property * newProp = initProperty(token, value);
                int check = findElement((void*)newProp, propList);
                if (check == 0){
                    //add to the list 
                    insertFront(&propList,(void*)newProp);
                    if (strcmp(temp,"DURATION")==0)
                        checkDuration = 1;
                    if (strcmp(temp,"REPEAT")==0)
                        checkRepeat = 1;
                } else {
                    free(value);
                    free(newProp);
                    clearList(&propList);
                    if (checkTrigger == 1)
                        free(trigger);
                    if (checkAction == 1)
                        free(action);
                    return INV_EVENT;
                }
            } else if (strcmp(temp,"X-PROP")==0||strcmp(temp,"IANA-PROP")==0){
                Property * newProp = initProperty(token, value);
                insertFront(&propList,(void*)newProp);
            } else {
                free(value);
                clearList(&propList);
                if (checkTrigger == 1)
                    free(trigger);
                if (checkAction == 1)
                    free(action);
                return INV_EVENT;   
            }
        }
        free(value);
        current = current->next;
    }
    return INV_EVENT;
}

ErrorCode parseEvent (Node * current, Event ** eventPtr, Node ** returnPos){
    current = current->next;

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

    while(current != NULL){
        char * line = (char *)current->data;
        printf("%s\n", line);
        /* make sure the line can be parsed */
        if (strchr(line,':') == NULL && strchr(line,';') == NULL){
            return INV_EVENT;
        }

        /* parse the line */
            char * token = strtok(line, ":;\t");
            char * holdVal = strtok(NULL, "\n");

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
                Node ** returnPos = malloc(sizeof(Node*));
                ErrorCode eCode = parseAlarm(current, newAlarm, returnPos);
                if (eCode != OK){
                    free(newAlarm);
                    freeEv(&propList, &alarmList, value);
                    if (checkUID == 1)
                        free(evUID);
                    if (checkDT == 1)
                        deleteDT(evDT);
                    return eCode;
                }
                //add alarm to the list
                insertFront(&alarmList, *newAlarm);
                free(newAlarm);
                current = *returnPos;
                free(returnPos);
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
                free(value);
                free(evUID);
                deleteDT(evDT);
                *returnPos = current;
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
        current = current->next;
    }
    return INV_EVENT;
}

ErrorCode parseCalendar (Node * current, Calendar ** obj){
    current = current->next;
    /* Keeps track of whether or not there has been a version and product ID 
    declared. There must be only one version declared and only one product ID declared */
    int checkVer = 0;
    int checkID = 0;
    /* hold the values of the version and porduct ID */
    float calVer;
    char * calID;
    /* for determining if we have found the end of the event or not */
    int eventEnd = 0;
    int end = 0;
    Event ** eventPtr;

    while (current != NULL){
        char * line = (char *)current->data;
        printf("%s\n", line);
        /* make sure the line can be parsed */
        if (strchr(line,':') == NULL && strchr(line,';') == NULL){
            return INV_CAL;
        }
        /* parse the line */
        char * token = strtok(line, ":;\t");
        char * holdVal = strtok(NULL, "\n");
        
        /* check if the value following the key word is NULL */
        if (holdVal == NULL){
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
                Node ** returnPos = malloc(sizeof(Node*));
                ErrorCode eCode = parseEvent(current, eventPtr, returnPos);
                if (eCode != OK){
                    free(eventPtr);
                    free(value);
                    free(calID);
                    free(returnPos);
                    return eCode;
                }
                current = *returnPos;
                free(returnPos);
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
            /* skip over any comments */
            if (strcmp(token, "COMMENT") != 0){ 
                free(value);                        
                if (checkID == 1)
                    free(calID);
                if (eventEnd == 1){
                    deleteEvent(*eventPtr);
                    free(eventPtr);
                }
                return INV_CAL;
            }
        }
        free(value);

        current = current->next;
    }
    free(calID);
    if (end == 1)
        return OK;
    else 
        return INV_CAL;
}

ErrorCode createCalendar(char* fileName, Calendar ** obj){
    /* check file extension */
    int l = strlen(fileName);
    char ext[5];
    ext[0] = fileName[l-4];
    ext[1] = fileName[l-3];
    ext[2] = fileName[l-2];
    ext[3] = fileName[l-1];
    ext[4] = '\0';
    int check = strcmp(ext, ".ics");
    if (check != 0){
        return INV_FILE;
    }

    if (fileName == NULL)
        return INV_FILE;
    /* call fileToList to read the file and put it in a list all multi 
    lines are unfolded and all lines with only white space are removes */
    List * list = malloc(sizeof(List));
    int fileCheck = fileToList(fileName, list);
    if (fileCheck == 0){
        free(list);
        return INV_FILE;
    }

    Node * current = list->head;
    if (current == NULL){
        free(list);
        return INV_FILE;
    }

    while (current != NULL){
        char * line = (char *)current->data;
        printf("%s\n", line);
        /* make sure the line can be parsed */
        if (strchr(line,':') == NULL && strchr(line,';') == NULL){
            clearList(list);
            free(list);
            return INV_CAL;
        }

        /* parse the line */
        char * token = strtok(line, ":;\t");
        char * holdVal = strtok(NULL, "\n");
        int len = strlen(holdVal) + 1;
        char * value = malloc(sizeof(char)*len);
        strcpy(value, holdVal);

        /* this should be the beginning of the calendar object */
        if (strcmp(token, "BEGIN") == 0){
            /* if the next word is not VCALENDAR then the file is wrong
            and INV_CAL is returned */
            if (strcmp(value, "VCALENDAR") == 0){
                ErrorCode eCode = parseCalendar(current,obj);
                free1(value, list);
                return eCode;
            } else {
                free1(value, list);
                return INV_CAL;
            }
        } else {
            if (strcmp(token, "COMMENT") != 0){
                free1(value, list);
                return INV_CAL;
            }
        }
        free(value);
        current = current->next;
    }
    clearList(list);
    free(list);
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
