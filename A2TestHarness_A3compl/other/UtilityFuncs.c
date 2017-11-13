#include "CalendarParser.h"
#include "ParserUtilities.h"
#include <ctype.h>

#define EVT_UNIQUE 15
static char* evtAppear1[EVT_UNIQUE] = {
    "DTSTART", "CLASS", "CREATED", "DESCRIPTION", "GEO", "LAST-MODIFIED", "LOCATION", "ORGANIZER", "PRIORITY", "SEQUENCE", "STATUS", "SUMMARY", "TRANSP", "URL", "RECURRENCE-ID"
};

#define EVT_EXCL 2
static char* evtExcl[EVT_EXCL] = {
    "DTEND","DURATION"
};

#define EVT_OPT 11
static char* evtOpt[EVT_OPT] = {
    "RRULE", "ATTACH", "ATTENDEE", "CATEGORIES", "COMMENT", "CONTACT", "EXDATE", "REQUEST-STATUS", "RELATED-TO", "RESOURCES", "RDATE"
};

int strcicmp(char const *a, char const *b)
{
    for (;; a++, b++) {
        int d = tolower(*a) - tolower(*b);
        if (d != 0 || !*a)
            return d;
    }
}

int findInArr(char* arr[], int len, const char* str){
    
    for (int i = 0; i < len; i++){
        if (strcicmp(arr[i], str) == 0){
            return i;
        }
    }
    return -1;
}

bool validateCalProperties(const Calendar* cal){

    if (getLength(cal->properties) == 0){
        return true;
    }
    bool hasCalscale = false;
    bool hasMethod = false;
    
    void* elem;
    ListIterator iter = createIterator(cal->properties);
   
    
    while( (elem = nextElement(&iter)) != NULL){
        Property* prop = (Property*)elem;
        
        if (strcicmp(prop->propName, "calscale") == 0){
            if (hasCalscale){
                return false;
            }else{
                hasCalscale = true;
            }
        }else if (strcicmp(prop->propName, "method") == 0){
            if (hasMethod){
                return false;
            }else{
                hasMethod = true;
            }
        }else{
            return false;
        }
    }
    return true;
}

ICalErrorCode validateEvent(const Event* evt){
    //Validate required properties
    if (strlen(evt->UID) == 0){
        return INV_EVENT;
    }
        
    if (strlen(evt->creationDateTime.date) != 8){
        return INV_EVENT;
    }
    
    if (strlen(evt->creationDateTime.time) != 6){
        return INV_EVENT;
    }
    
    ICalErrorCode err;
    
    //Validate optional properties
    if (!validateEventProperties(evt)){
        return INV_EVENT;
    }
    
    //Validate alarms
    void* elem;
    ListIterator iter = createIterator(evt->alarms);
    while( (elem = nextElement(&iter)) != NULL){
        Alarm* alm = (Alarm*)elem;
        err = validateAlarm(alm);
        if (err != OK){
            return err;
        }
    }
    
    return OK;
}

ICalErrorCode validateAlarm(const Alarm* alm){
    
    //Validate required properties
    if (strlen(alm->action) == 0 ){
        return INV_ALARM;
    }
    
    if (alm->trigger == NULL){
        return INV_ALARM;
    }
    
    if (strlen(alm->trigger) == 0 ){
        return INV_ALARM;
    }
    
    //Validate optional properties
    if (!validateAlarmProperties(alm)){
        return INV_ALARM;
    }
    
    return OK;
}

bool validateEventProperties(const Event* evt){
    
    int evtUniqueVals[EVT_UNIQUE] = {0};
    int evtExclVals[EVT_EXCL] = {0};
    int evtOptVals[EVT_OPT] = {0};
    
    /*
    
    ;
    ; The following are OPTIONAL,
    ; but MUST NOT occur more than once.
    ;
    dtstart", "class", "created", "description", "geo /
    last-mod", "location", "organizer", "priority /
    seq", "status", "summary", "transp /
    url", "recurid /
     */
    
    void* elem;
    ListIterator iter = createIterator(evt->properties);
    while( (elem = nextElement(&iter)) != NULL){
        Property* prop = (Property*)elem;
        if (strlen(prop->propName) == 0){
            return false;
        }
        //Check if optional unique elements appear only once
        int indUnique = findInArr(evtAppear1, EVT_UNIQUE, prop->propName);
        if (indUnique >= 0){
            if (evtUniqueVals[indUnique] > 0){
                return false;
            }else{
                evtUniqueVals[indUnique] += 1;
            }
        }
        //Check optional mutually exclusive elements
        int indExcl = findInArr(evtExcl, EVT_EXCL, prop->propName);
        if (indExcl >= 0){
            evtExclVals[indExcl] += 1;
        }
        //Check optional elements
        int indOpt = findInArr(evtOpt, EVT_OPT, prop->propName);
        if (indOpt >= 0){
            evtOptVals[indOpt] += 1;
        }
        //If property is valid, it must be in one of the three arrays, so one of thr three
        //indeces must be positive
        if (indOpt < 0 && indExcl < 0 && indUnique < 0){
            return false;
        }
    }
    if (evtExclVals[0] > 0 && evtExclVals[1] > 0){
        return false;
    }
    return true;
}

bool validateAlarmProperties(const Alarm* alm){
    /*
     'duration' and 'repeat' are both OPTIONAL, and MUST NOT occur more than
     once each; but if one occurs, so MUST the other.
     
     attach is OPTIONAL, but MUST NOT occur more than once.
    */
    
    void* elem;
    ListIterator iter = createIterator(alm->properties);
    bool hasAttach = false;
    bool hasDur = false;
    bool hasRepeat = false;
    
    while( (elem = nextElement(&iter)) != NULL){
        Property* prop = (Property*)elem;
        
        //Attach
        if (strcicmp(prop->propName, "attach") == 0){
            if (hasAttach){
                return false;
            }else{
                hasAttach = true;
            }
        }
        
        //Duration
        if (strcicmp(prop->propName, "duration") == 0){
            if (hasDur){
                return false;
            }else{
                hasDur = true;
            }
        }
        
        //Repeat
        if (strcicmp(prop->propName, "repeat") == 0){
            if (hasRepeat){
                return false;
            }else{
                hasRepeat = true;
            }
        }
    }
    
    if (hasRepeat == hasDur){
        return true;
    }else{
        return false;
    }
}

char* printAlarm(void *toBePrinted){
	char* tmpStr;
	Alarm* tmpAlarm;
	
	int len;
	
	if (toBePrinted == NULL){
		return NULL;
	}
	
	tmpAlarm = (Alarm*)toBePrinted;
	char* alarmProps = toString(tmpAlarm->properties);
	
	len = strlen(tmpAlarm->action)+strlen(tmpAlarm->trigger)+38+ strlen(alarmProps);
	tmpStr = malloc(sizeof(char)*len);
	if (tmpStr == NULL){
		return NULL;
	}
	
	sprintf(tmpStr, "\t\tAction: %s\n\t\tTrigger: %s\n\t\tProperties:%s\n", tmpAlarm->action, tmpAlarm->trigger, alarmProps);
	free(alarmProps);
	
	return tmpStr;
}

char* printProperty(void *toBePrinted){
	char* tmpStr;
	Property* tmpProp;
	
	int len;
	
	if (toBePrinted == NULL){
		return NULL;
	}
	
	tmpProp = (Property*)toBePrinted;
	
	len = strlen(tmpProp->propName)+strlen(tmpProp->propDescr)+7;
	tmpStr = malloc(sizeof(char)*len);
	if (tmpStr == NULL){
		return NULL;
	}
	
	sprintf(tmpStr, "\t\t- %s:%s", tmpProp->propName, tmpProp->propDescr);
	
	return tmpStr;
}

void writeProperty(FILE* file, Property* prop){
    fprintf(file, "%s:%s\r\n", prop->propName, prop->propDescr);
}

void writeEvent(FILE* file, Event* event){
    
    if (event == NULL){
        return;
    }
    
    fprintf(file, "BEGIN:VEVENT\r\n");
    fprintf(file, "UID:%s\r\n", event->UID);
    writeDateTime(file, event->creationDateTime);
    
    ListIterator iter = createIterator(event->properties);
    void* elem;
    while( (elem = nextElement(&iter)) != NULL){
        Property* prop = (Property*)elem;
        writeProperty(file, prop);
    }
    
    iter = createIterator(event->alarms);
    while( (elem = nextElement(&iter)) != NULL){
        Alarm* alarm = (Alarm*)elem;
        writeAlarm(file, alarm);
    }
    
    fprintf(file,"END:VEVENT\r\n");
}

void writeAlarm(FILE* file, Alarm* alarm){
    
    if (alarm == NULL){
        return;
    }
    
    fprintf(file, "BEGIN:VALARM\r\n");
    fprintf(file, "ACTION:%s\r\n", alarm->action);
    fprintf(file, "TRIGGER:%s\r\n", alarm->trigger);
    
    ListIterator iter = createIterator(alarm->properties);
    void* elem;
    while( (elem = nextElement(&iter)) != NULL){
        Property* prop = (Property*)elem;
        writeProperty(file, prop);
    }
    
    fprintf(file,"END:VALARM\r\n");
}

char* printEvent(void *toBePrinted){
	
    if (toBePrinted == NULL){
        return NULL;
    }
    
    Event* event;
    
    event = (Event*)toBePrinted;
    
	char* eventStr;
	
    if (event != NULL){
		char tmpUID[1000];
		char tmpVersion[1000];
		int len;
		
		
        sprintf(tmpUID, "\tUID = %s\n", event->UID);
        sprintf(tmpVersion, "\tcreationDateTime = ");
		char* dtDescr = printDateTime(event->creationDateTime);
		char* propDescr = toString(event->properties);
		char* alarmDescr = toString(event->alarms);
		
		len = strlen(tmpUID)+strlen(tmpVersion)+strlen(dtDescr)+strlen(propDescr)+strlen(alarmDescr)+40;
		eventStr = malloc(sizeof(char)*len);
		
		strcpy(eventStr, tmpUID);
		strcat(eventStr, tmpVersion);
		strcat(eventStr, dtDescr);
		strcat(eventStr, "\n\tAlarms:");
		strcat(eventStr, alarmDescr);
		strcat(eventStr, "\n\n\tOther properties:");
		strcat(eventStr, propDescr);
		strcat(eventStr, "\n\n");
		
		free(dtDescr);
		free(alarmDescr);
		free(propDescr);
    }else{
    	eventStr = malloc(sizeof(char)*5);
		strcpy(eventStr, "NULL");
    }
	
	return eventStr;
}

void writeDateTime(FILE* file, DateTime dt){
    
    fprintf(file, "DTSTAMP:%sT%s%s", dt.date, dt.time, dt.UTC == true ? "Z\r\n": "\r\n" );
    
}

char* printDateTime(DateTime dt){
	char* tmpStr;
	int len = sizeof(bool)+14+7+1;
	
	
	tmpStr = malloc(sizeof(char)*len);
	if (tmpStr == NULL){
		return NULL;
	}
	
    sprintf(tmpStr, "%s:%s, UTC=%d", dt.date, dt.time, dt.UTC);
	
	return tmpStr;
}

int compareEvents(const void *first, const void *second){
    Event* tmpEvent1;
    Event* tmpEvent2;
    
    if (first == NULL || second == NULL){
        return 0;
    }
    
    tmpEvent1 = (Event*)first;
    tmpEvent2 = (Event*)second;
    
    return strcmp((char*)tmpEvent1->UID, (char*)tmpEvent2->UID);
}

int compareAlarms(const void *first, const void *second){
	Alarm* tmpAlarm1;
	Alarm* tmpAlarm2;
	
	if (first == NULL || second == NULL){
		return 0;
	}
	
	tmpAlarm1 = (Alarm*)first;
	tmpAlarm2 = (Alarm*)second;
		
	return strcmp((char*)tmpAlarm1->action, (char*)tmpAlarm2->action);
}

int compareProperties(const void *first, const void *second){
	Property* tmpProp1;
	Property* tmpProp2;
	
	if (first == NULL || second == NULL){
		return 0;
	}
	
	tmpProp1 = (Property*)first;
	tmpProp2 = (Property*)second;
		
	return strcmp((char*)tmpProp1->propName, (char*)tmpProp2->propName);
}

void deleteAlarm(void *toBeDeleted){
	
	Alarm* tmpAlarm;
	
	if (toBeDeleted == NULL){
		return;
	}
	
	tmpAlarm = (Alarm*)toBeDeleted;
	
	free(tmpAlarm->trigger);
	clearList(&tmpAlarm->properties);
	free(tmpAlarm);
}

void deleteProperty(void *toBeDeleted){
	
	Property* tmpProp;
	
	if (toBeDeleted == NULL){
		return;
	}
	
	tmpProp = (Property*)toBeDeleted;
	
	free(tmpProp);
}

void deleteEvent(void* toBeDeleted){
    
    Event* evt;
    
    if (toBeDeleted == NULL){
        return;
    }
    
    evt = (Event*)toBeDeleted;
	
	clearList(&evt->properties);
	clearList(&evt->alarms);
	free(evt);
}
