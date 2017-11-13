#include "CalendarParser.h"
#include "readFile.h"
#include "BasicFunctions.h"
#include "validateFunctions.h"


void deleteEvent (void * toDelete);
char * printEvent(void * event);
char * toUpper(char * str);

/*-------- For checking the whether a property is valid --------*/

int calPropCheck(Property * prop, List propList){
    char * propName = toUpper(prop->propName);
    if (strcmp(propName,"CALSCALE")==0||strcmp(propName,"METHOD")==0||strcmp(propName,"CALSCALE")==0){ //only one
        //check the list for duplicates
        void * check = findElement(propList, custCompareProp, (void*)prop);
        free(propName);
        if (check != NULL)
            return 0;
        else 
            return 1;
    } else {
        free(propName);
        return 0;
    }
}

int evPropCheck(Property * prop, List propList){
    char * propName = toUpper(prop->propName);
    if (strcmp(propName,"CLASS")==0||strcmp(propName,"CREATED")==0||
        strcmp(propName,"DESCRIPTION")==0||strcmp(propName,"GEO")==0||strcmp(propName,"LAST-MOD")==0||
        strcmp(propName,"LOCATION")==0||strcmp(propName,"ORGANIZER")==0||strcmp(propName,"SEQ")==0||
        strcmp(propName,"PRIORITY")==0||strcmp(propName,"STATUS")==0||strcmp(propName,"SUMMARY")==0||
        strcmp(propName,"TRANSP")==0||strcmp(propName,"URL")==0||strcmp(propName,"RECURID")==0||
        strcmp(propName,"RRULE")==0||strcmp(propName,"LAST-MODIFIED")==0){

        //check the list for duplicates
        void * check = findElement(propList,custCompareProp, (void*)prop);
        free(propName);
        if (check != NULL)
            return 0;
        else 
            return 1;
    } else if (strcmp(propName,"DURATION")==0||strcmp(propName,"DTEND")==0){
        //check the list for the other
        //need to make a temporary struct of the other
        Property * temp = initProperty("DTEND","temp");
        Property * temp2 = initProperty("DURATION","temp");
        void * check = findElement(propList, custCompareProp, (void*)temp);
        void * check2 = findElement(propList, custCompareProp, (void*)temp2);
        free(temp);
        free(temp2);
        free(propName);
        if (check == NULL && check2 == NULL)
            return 1;
        else 
            return 0;
    } else if (strcmp(propName,"ATTACH")==0||strcmp(propName,"ATTENDEE")==0||strcmp(propName,"CATEGORIES")==0||
        strcmp(propName,"COMMENT")==0||strcmp(propName,"CONTACT")==0||strcmp(propName,"EXDATE")==0||
        strcmp(propName,"RSTATUS")==0||strcmp(propName,"RELATED")==0||strcmp(propName,"RESOURCES")==0||
        strcmp(propName,"RDATE")==0){
        free(propName);
        return 1;
    } else {
        free(propName);
        return 0;
    }
}

/*----- For freeing the memory when the calendar is invalid ------*/

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

/* ----- For parsing the iCal file ----- */

ICalErrorCode parseAlarm(Node * current, Alarm ** alarmPtr, Node ** returnPos){
    // check that it is an audio alarms
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
        if (line[0] == ';'){
            current = current->next;
            continue;
        }
        if (strchr(line,':') == NULL && strchr(line,';') == NULL){
            clearList(&propList);
            if (checkAction == 1)
                free(action);
            if  (checkTrigger == 1)
                free(trigger);
            return INV_ALARM;
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
            return INV_ALARM;
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
                return INV_ALARM;
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
                return INV_ALARM;
            }
        } else if (strcmp(token,"END")==0){
            if (strcmp(value,"VALARM")==0 &&checkAction == 1 && checkTrigger == 1){
                if (checkDuration == 1 || checkRepeat == 1){
                    if (!(checkDuration == 1 && checkRepeat == 1)){
                        free(value);
                        clearList(&propList);
                        free(trigger);
                        free(action);
                        return INV_ALARM;
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
                return INV_ALARM;
            }

        } else {
            // porperties - eliminate case sensitivity
            char * temp = toUpper(token);
            if (strcmp(temp,"DURATION")==0||strcmp(temp,"REPEAT")==0||strcmp(temp,"ATTACH")==0
                ||strcmp(temp,"DESCRIPTION")==0||strcmp(temp,"SUMMARY")==0){

                Property * newProp = initProperty(token, value);
                void * check = findElement(propList, custCompareProp, (void*)newProp);
                if (check == NULL){
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
                    return INV_ALARM;
                }
            } else {
                free(value);
                clearList(&propList);
                if (checkTrigger == 1)
                    free(trigger);
                if (checkAction == 1)
                    free(action);
                return INV_ALARM;   
            }
        }
        free(value);
        current = current->next;
    }
    return INV_ALARM;
}

ICalErrorCode parseEvent (Node * current, Event ** eventPtr, Node ** returnPos){
    current = current->next;

    List propList = initializeList(printProperty, deleteProperty, compareProperty);
    List alarmList = initializeList(printAlarm, deleteAlarm, compareAlarm);

    /* Keeps track of whether or DTSTAMP and UID 
    have been declared. There must be only one
    DTSTAMP declared and only one UID declared */
    int checkUID = 0;
    int checkDT = 0;
    int checkDTStart = 0;
    /* hold the values of the DTSTAMP and UID */
    char * evUID;
    DateTime * evDT;
    DateTime * DTStart;

    while(current != NULL){
        char * line = (char *)current->data;
        printf("%s\n", line);
        /* make sure the line can be parsed */
        if (line[0] == ';'){
            current = current->next;
            continue;
        }
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
                if (evDT == NULL){
                    if (checkUID == 1)
                        free(evUID);
                    return INV_CREATEDT;
                }
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
        } else if (strcmp(token,"DTSTART")==0){ // fix this
            if (checkDTStart == 0){ /* make sure this is the only UID */
                DTStart = initDT(value);
                if (DTStart == NULL){
                    if (checkDT == 1)
                        deleteDT(evDT);
                    if (checkUID == 1)
                        free(evUID);
                    return INV_EVENT;
                }
                checkDTStart = 1;
            } else {
                deleteDT(DTStart);
                freeEv(&propList, &alarmList, value);
                if (checkUID == 1)
                    free(evUID);
                if (checkDT == 1)
                    deleteDT(evDT);
                return INV_EVENT;
            }
        } else if (strcmp(token,"BEGIN")==0){ /* only allow one Event per calendar object */
            if (strcmp(value, "VALARM") == 0 && checkUID == 1 && checkDT == 1){
                //go to parseAlarm 
                Alarm ** newAlarm = malloc(sizeof(Alarm*));
                Node ** returnPos = malloc(sizeof(Node*));
                ICalErrorCode eCode = parseAlarm(current, newAlarm, returnPos);
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
                if (checkDTStart == 0){
                    DTStart = evDT;
                }     
                Event * temp = initEvent(evUID,evDT, DTStart,propList,alarmList);
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

ICalErrorCode parseCalendar (Node * current, Calendar ** obj){
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
    List propList = initializeList(printProperty, deleteProperty, compareProperty);
    List eventList = initializeList(printEvent, deleteEvent, compareEvent); // make a compare event

    while (current != NULL){
        char * line = (char *)current->data;
        printf("%s\n", line);
        if (line[0] == ';'){
            current = current->next;
            continue;
        }
        /* make sure the line can be parsed */
        if (strchr(line,':') == NULL && strchr(line,';') == NULL){
            clearList(&propList);
            clearList(&eventList);
            return INV_CAL;
        }
        /* parse the line */
        char * token = strtok(line, ":;\t");
        char * holdVal = strtok(NULL, "\n");
        
        /* check if the value following the key word is NULL */
        if (holdVal == NULL){
            clearList(&propList);
            clearList(&eventList);
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
                    clearList(&propList);
                    clearList(&eventList);
                    free(value);
                    if (checkID == 1)
                        free(calID);
                    return INV_VER;
                }
                checkVer = 1;
            } else {
                clearList(&propList);
                clearList(&eventList);
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
                clearList(&propList);
                clearList(&eventList);
                return DUP_PRODID;
            }
        } else if (strcmp(token,"BEGIN")==0){ /* only allow one Event per calendar object */
            if (strcmp(value, "VEVENT") == 0 && checkID == 1 && checkVer == 1){
                //go to parseEvent 
                eventPtr = malloc(sizeof(Event*));
                Node ** returnPos = malloc(sizeof(Node*));
                ICalErrorCode eCode = parseEvent(current, eventPtr, returnPos);
                if (eCode != OK){
                    clearList(&propList);
                    clearList(&eventList);
                    free(value);
                    free(calID);
                    free(returnPos);
                    return eCode;
                }
                /* add the event to the event list */
                insertFront(&eventList, *eventPtr);
                current = *returnPos;
                free(returnPos);
                eventEnd = 1;

            } else {
                free(value);
                clearList(&propList);
                clearList(&eventList);
                if (checkID == 1)
                    free(calID);
                return INV_CAL;
            }

        } else if (strcmp(token,"END")==0 && end == 0){ /* don't want multiple ends */
            if (strcmp(value, "VCALENDAR") == 0 && checkID == 1 && checkVer == 1 && eventEnd == 1){
                end = 1;
                //create a calendar object
                Calendar * newCal = initCal(calVer,calID, eventList, propList);
                *obj = newCal;
                free(eventPtr);
            } else {
                free(value);
                clearList(&propList);
                clearList(&eventList);
                if (checkID == 1)
                    free(calID);
                if (eventEnd == 1)
                    deleteEvent(*eventPtr);
                return INV_CAL;
            }
        } else {
            /* properties */
            Property * newProp = initProperty(token,value);
            if (calPropCheck(newProp, propList) == 1){ // the property is valid
                insertFront(&propList, (void *)newProp);
            } else { 
                deleteProperty(newProp);
                clearList(&propList);
                clearList(&eventList);
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
    if (end == 1){
        return OK;
    } else {
        clearList(&propList);
        clearList(&eventList);
        return INV_CAL;
    } 
}

ICalErrorCode createCalendar(char* fileName, Calendar ** obj){
    if (fileName == NULL)
        return INV_FILE;
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
        if (line[0] == ';'){
            current = current->next;
            continue;
        }
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
                ICalErrorCode eCode = parseCalendar(current,obj);
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

void deleteCalendar(Calendar * obj){
    clearList(&(obj->events));
    clearList(&(obj->properties));
    free(obj);
}

char * printError (ICalErrorCode err){
    char buffer[100];
    if (err == INV_CAL)
        strcpy(buffer, "Invaid Calendar");
    else if (err == OK)
        strcpy(buffer, "Ok");
    else if (err == INV_FILE)
        strcpy(buffer, "Invaid File");
    else if (err == INV_VER)
        strcpy(buffer, "Invaid Version");
    else if (err == INV_PRODID)
        strcpy(buffer, "Invaid Product ID");
    else if (err == INV_EVENT)
        strcpy(buffer, "Invaid Event");
    else if (err == DUP_VER)
        strcpy(buffer, "Duplicate Version");
    else if (err == DUP_PRODID)
        strcpy(buffer, "Duplicate Product ID");
    else if (err == INV_CREATEDT)
        strcpy(buffer, "Invaid Date-Time");
    else if (err == INV_ALARM)
        strcpy(buffer, "Invaid Alarm");
    else if (err == WRITE_ERROR)
        strcpy(buffer, "Write Error");
    else
        strcpy(buffer, "Other Error");
    
    char * rtn = malloc(sizeof(char)*(strlen(buffer)+1));
    strcpy(rtn, buffer);
    return rtn;
}

ICalErrorCode writeCalendar(char* fileName, const Calendar* obj){
    /* check if obj is valid */
    ICalErrorCode ec = validateCalendar(obj);
    if (ec != OK)
        return ec;
    /* check file is valid */
    FILE * fp = fopen(fileName, "w");
    if (fp == NULL)
        return INV_FILE;
    /* start file */
    fprintf(fp, "BEGIN:VCALENDAR\r\n");
    fprintf(fp, "PRODID:%s\r\n", obj->prodID);
    fprintf(fp, "VERSION:%.1f\r\n", obj->version);
    /* calendadr properties */
    Node * hold = obj->properties.head;
    while (hold != NULL){
        Property * prop = (Property *)hold->data;
        fprintf(fp, "%s:%s\r\n", prop->propName,prop->propDescr);
        hold = hold->next;
    }
    /* calendar events */
    hold = obj->events.head;
    while (hold != NULL){
        fprintf(fp, "BEGIN:VEVENT\r\n");
        Event * evt = (Event *)hold->data;
        fprintf(fp, "UID:%s\r\n", evt->UID);
        if (evt->creationDateTime.UTC == true)
            fprintf(fp, "DTSTAMP:%sT%sZ\r\n", evt->creationDateTime.date, evt->creationDateTime.time);
        else 
            fprintf(fp, "DTSTAMP:%sT%s\r\n", evt->creationDateTime.date, evt->creationDateTime.time);
        /* event properties */
        Node * hold2 = evt->properties.head;
        while (hold2 != NULL){
            Property * prop = (Property *)hold2->data;
            fprintf(fp, "%s:%s\r\n", prop->propName,prop->propDescr);
            hold2 = hold2->next;
        }
        /* event alarms */
        hold2 = evt->alarms.head;
        while (hold2 != NULL){
            fprintf(fp, "BEGIN:VALARM\r\n");
            Alarm * alarm = (Alarm *)hold2->data;
            fprintf(fp, "ACTION:%s\r\n", alarm->action);
            fprintf(fp, "TRIGGER:%s\r\n", alarm->trigger);
            /* alarm properties */
            Node * hold3 = alarm->properties.head;
            while (hold3 != NULL){
                Property * prop = (Property *)hold3->data;
                fprintf(fp, "%s:%s\r\n", prop->propName,prop->propDescr);
                hold3 = hold3->next;
            }
            fprintf(fp, "END:VALARM\r\n");
            hold2 = hold2->next;
        }
        fprintf(fp, "END:VEVENT\r\n");
        hold = hold->next;
    }
    fprintf(fp, "END:VCALENDAR\r\n");
    fclose(fp);
    return OK;
}

ICalErrorCode validateCalendar(const Calendar * obj){

    //make sure the calendar exists
    if (obj == NULL)
        return INV_CAL;
    //check version
    if (obj->version != 2.0)
        return INV_VER;
    //check prodid
    if (obj->prodID == NULL)
        return INV_PRODID;
    //check for at least one event
    if (obj->events.head == NULL)
        return INV_CAL;
    if (obj->events.head->data == NULL)
        return INV_CAL;
    //check properties
    ICalErrorCode ec2 = validateProperties(obj->properties,calProp);
    if (ec2 != OK)
        return ec2;
    //check the events
    Node * hold = obj->events.head;
    while (hold != NULL){
        ICalErrorCode ec = validateEvent((Event *)hold->data);
        if (ec != OK)
            return ec;
        hold = hold->next;
    }

    return OK;
}

