#include "CalendarParser.h"
#include "ParserUtilities.h"

const char * descr[] = {
    "OK", "INV_FILE", "INV_CAL", "INV_VER", "DUP_VER", "INV_PRODID", "DUP_PRODID", "INV_EVENT", "INV_CREATEDT", "INV_ALARM", "WRITE_ERROR", "OTHER_ERROR"
};

ICalErrorCode createCalendar(char* fileName, Calendar** newCalObject){
	FILE* icalFile;
	
	if (fileName == NULL){
		fprintf(stderr, "filename must not be null or empty\n");
		return INV_FILE;
	}
	
	if (strstr(fileName, ".ics") == NULL){
		fprintf(stderr, "file must have a .ics extension\n");
		return INV_FILE;
	}
	
	if ( (icalFile = fopen(fileName, "r")) == NULL ){
		fprintf(stderr, "File %s ccould not be opened.  Check file name and permissions\n", fileName);
		return INV_FILE;
	}
				
	Calendar* tmpCalObject;
    
    ICalErrorCode code = createObjectFromFile(icalFile, &tmpCalObject);
    
    if (code == OK){
        *newCalObject = tmpCalObject;
    }
	
	fclose(icalFile);
	
    if (code == OK){
        code = validateCalendar(tmpCalObject);
    }
    
	return code;
}

void deleteCalendar(Calendar* obj){
	if (obj == NULL){
		return;
	}
	
    /*
    if (obj->event != NULL){
        deleteEvent(obj->event);
    }*/
    
    clearList(&obj->events);
    clearList(&obj->properties);
    free(obj);
}

ICalErrorCode writeCalendar(char* fileName, const Calendar* obj){
    
    FILE* file;
    
    if (fileName == NULL){
        return WRITE_ERROR;
    }
    
    if (strcmp(fileName, "") == 0){
        return WRITE_ERROR;
    }
    
    if (strstr(fileName, ".ics") == NULL){
        return WRITE_ERROR;
    }
    
    if ( (file = fopen(fileName, "w")) == NULL ){
        return WRITE_ERROR;
    }
    
    if (obj == NULL){
        return WRITE_ERROR;
    }
    
    fprintf(file,"BEGIN:VCALENDAR\r\n");
    fprintf(file, "PRODID:%s\r\n", obj->prodID);
    fprintf(file, "VERSION:%.02f\r\n", obj->version);
    if (getLength(obj->properties) > 0){
        ListIterator iter = createIterator(obj->properties);
        void* elem;
        while( (elem = nextElement(&iter)) != NULL){
            Property* prop = (Property*)elem;
            writeProperty(file, prop);
        }
    }
    ListIterator iter = createIterator(obj->events);
    void* elem;
    while( (elem = nextElement(&iter)) != NULL){
        Event* event = (Event*)elem;
        writeEvent(file, event);
    }
    
    fprintf(file,"END:VCALENDAR\r\n");
    fclose(file);
    return OK;
}

ICalErrorCode validateCalendar(const Calendar* obj){
    
    //Validate required properties
    if (obj == NULL){
        fprintf(stderr, "11111\n");
        return INV_CAL;
    }
    
    if (obj->version <= 0){
        fprintf(stderr, "22222\n");
        return INV_CAL;
    }
    
    if (strlen(obj->prodID) == 0){
        fprintf(stderr, "33333\n");
        return INV_CAL;
    }
    
    //Validate optional properties
    if (!validateCalProperties(obj)){
        fprintf(stderr, "44444\n");
        return INV_CAL;
    }
    
    //Validate events
    if (getLength(obj->events) == 0){
        fprintf(stderr, "55555\n");
        return INV_CAL;
    }
    
    ICalErrorCode err;
    
    ListIterator iter = createIterator(obj->events);
    void* elem;
    
    while( (elem = nextElement(&iter)) != NULL){
        Event* evt = (Event*)elem;
        err = validateEvent(evt);
        if (err != OK){
            fprintf(stderr, "66666\n");
            return err;
        }
    }
    
    return OK;
}

char* printCalendar(const Calendar* obj){
	char* calString;	
	char tmpStr[2000];
	
    if (obj != NULL) {
        sprintf(tmpStr, "Calendar: version = %f, prodID = %s\n", obj->version, obj->prodID);
        
        char* calProps = toString(obj->properties);
        char* eventDescr = toString(obj->events);
        
        char calPropHeader[] = "\tOther properties:";
        
        int len = strlen(calProps) + strlen(calPropHeader) + strlen(tmpStr)+strlen(eventDescr)+20;
		calString = malloc(len*sizeof(char));
        strcpy(calString, tmpStr);
        strcat(calString, calPropHeader);
        strcat(calString, calProps);
        strcat(calString, "\n\n\tEvents:\n");
		strcat(calString, eventDescr);
		free(eventDescr);
        free(calProps);
    }else{
    	calString = malloc(sizeof(char)*5);
		strcpy(calString, "NULL");
    }
	
	return calString;
}

char* printError(ICalErrorCode err){
    //fprintf(stderr, "PintError: %s\n", (descr[err]));
    char* tmp = malloc((strlen(descr[err])+1)*sizeof(char));
    strcpy(tmp, descr[err]);
    return tmp;
}
