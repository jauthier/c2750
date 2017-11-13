#include "CalendarParser.h"
#include "ParserUtilities.h"

ICalErrorCode createObjectFromFile(FILE* file, Calendar** newCalObject){
//	char line [1000];
	char* tmpTok[2];
	char prodID[1000];
	float version = -1;
    bool verSet = false;
    bool prodSet = false;
//    bool evtSet = false;
    bool calEnd = false;
    
    
    ICalErrorCode evtCode;
    Event* tmpEvt;
		
	Calendar* tmpCalendar;
    //Check header
    //if (fgets(line, sizeof line, file)!= NULL){
        //parseLine(line, tmpTok);
    if (!parseFoldedLine(file, tmpTok)){
        return INV_CAL;
    }
    if (strcmp(tmpTok[0], "BEGIN") != 0 || strcmp(tmpTok[1], "VCALENDAR") != 0){
        return INV_CAL;
    }
    free(tmpTok[0]);
    free(tmpTok[1]);

    List events = initializeList(&printEvent, &deleteEvent, &compareEvents);
    List properties = initializeList(&printProperty, &deleteProperty, &compareProperties);

    while(!feof(file)){
        if (!parseFoldedLine(file, tmpTok)){
            return INV_CAL;
        }
        
        //Comments are ignored
        if (strcmp(tmpTok[0], ";") == 0){
            //free(tmpTok[0]);
        }
        //Read version
        else if (strcmp(tmpTok[0], "VERSION") == 0){
            if (verSet){
                return DUP_VER;
            }else if (getVersion(tmpTok, &version)){
                verSet = true;
            }else{
                return INV_VER;
            }
        }
        //Read product ID
        else if (strcmp(tmpTok[0], "PRODID") == 0){
            if (prodSet){
                return DUP_PRODID;
            }else if (getProd(tmpTok, prodID)){
                prodSet = true;
            }else{
                return INV_PRODID;
            }
        }
        //Read event
        else if (strcmp(tmpTok[0], "BEGIN") == 0 && strcmp(tmpTok[1], "VEVENT") == 0){
            evtCode = getEvent(file, &tmpEvt);
            if (evtCode != OK){
                return evtCode;
            }
            insertBack(&events, tmpEvt);
        }
        //Check footer
        else if (strcmp(tmpTok[0], "END") == 0 && strcmp(tmpTok[1], "VCALENDAR") == 0){
            calEnd = true;
            free(tmpTok[0]);
            free(tmpTok[1]);
            
            break;
        }else{
            Property* tmpProp = createProperty(tmpTok);
            insertBack(&properties, (void*)tmpProp);
        }
        free(tmpTok[0]);
        free(tmpTok[1]);
        
    }
	
    //Create and initialize the object
    if (getLength(events) != 0 && verSet && prodSet && calEnd){
    	tmpCalendar = (Calendar*)malloc(sizeof(Calendar));
        tmpCalendar->version = version;
        strcpy(tmpCalendar->prodID, prodID);
        //tmpCalendar->event = tmpEvt;
        tmpCalendar->events = events;
        tmpCalendar->properties = properties;
		printf("Allocated object\n");
    }else{
        return INV_CAL;
    }
    
    *newCalObject = tmpCalendar;
    
	return OK;
	
}

ICalErrorCode getEvent(FILE *file, Event** newEvent){
    //char        line [1000];
    
    bool        UIDset = false;
    bool        crtDTset = false;
    
    DateTime    tmpDateTime;
	ICalErrorCode 	status = OK;
	
    char*       tmpTok[2];
    
	Event* tmpEvent = (Event*)malloc(sizeof(Event));
    tmpEvent->properties = initializeList(&printProperty, &deleteProperty, &compareProperties);
    tmpEvent->alarms = initializeList(&printAlarm, &deleteAlarm, &compareAlarms);
	
   // while(parseFoldedLine(line, tmpTok)){
        //parseLine(line, tmpTok);
    while(!feof(file)){
        if (!parseFoldedLine(file, tmpTok)){
            status = INV_EVENT;
            //free(tmpTok[0]);
            //free(tmpTok[1]);
            fprintf(stderr, "getEvent: parseFoldedLine\n");
            break;
        }else  if (strcmp(tmpTok[0], ";") == 0){
            // **** FIX ***
            //free(tmpTok[0]);
        }
        //Get UID
        else if (strcmp(tmpTok[0], "UID") == 0){
            if (UIDset){
                status = INV_EVENT;
            }else{
                strcpy(tmpEvent->UID, tmpTok[1]);
                UIDset = true;
            }
        }else if (strcmp(tmpTok[0], "DTSTAMP") == 0){
            if (crtDTset){
                status = INV_EVENT;
            }else if (!getDate(tmpTok[1], &tmpDateTime)){
                status = INV_CREATEDT;
            }else{
				tmpEvent->creationDateTime  = tmpDateTime;
                crtDTset = true;
            }
        }else if (strcmp(tmpTok[0], "END") == 0 && strcmp(tmpTok[1], "VEVENT") == 0){
            free(tmpTok[0]);
            free(tmpTok[1]);

            break;
        }else{
            if (strcmp(tmpTok[0], "BEGIN") == 0 && strcmp(tmpTok[1], "VALARM") == 0){
				
                Alarm* tmpAlm;
                if (createAlarm(file, &tmpAlm) == OK){
                    insertBack(&tmpEvent->alarms, (void*)tmpAlm);
                }else{
                    status = INV_ALARM;
                }
				
            }else{
            	Property* tmpProp = createProperty(tmpTok);
                if (tmpProp ==NULL){
                    status = INV_EVENT;
                }
                insertBack(&tmpEvent->properties, (void*)tmpProp);    
            }
        }
        free(tmpTok[0]);
        free(tmpTok[1]);
        
		
        if (status != OK){
        	break;
        }
    }

	if (status != OK){
		printf("Freeing invalid event");
		deleteEvent(tmpEvent);
		return status;
	}
    
    
    
    *newEvent = tmpEvent;
    
    return OK;
} 

Property* createProperty(char* tokens[]){
	Property* tmpProp;
	
	if (tokens[0] == NULL || tokens[1] == NULL){
		return NULL;
	}
	
	if (strlen(tokens[0]) == 0 || strlen(tokens[1]) == 0){
		return NULL;
	}
	
	int memSize = sizeof(Property) + sizeof(char) * (strlen(tokens[1]) + 1);
	tmpProp = (Property*)malloc(memSize);
	strcpy(tmpProp->propName, tokens[0]);
	strcpy(tmpProp->propDescr, tokens[1]);
	
	return tmpProp;
}

ICalErrorCode createAlarm(FILE *file, Alarm** newAlarm){
    Alarm*      tmpAlarm;
    //char        line [1000];
    char*       tmpTok[2];
    bool        actionSet = false;
    bool        triggerSet = false;
    ICalErrorCode 	status = OK;
    
	tmpAlarm = (Alarm*)malloc(sizeof(Alarm));
    tmpAlarm->properties = initializeList(&printProperty, &deleteProperty, &compareProperties);
    tmpAlarm->trigger = NULL;
    
    //while(fgets(line, sizeof line, file)!= NULL){
    //parseLine(line, tmpTok);
    while(!feof(file)){
        if (!parseFoldedLine(file, tmpTok)){
            status = INV_ALARM;
            fprintf(stderr,"BROKEN ALARUMS!\n");
            free(tmpTok[0]);
            free(tmpTok[1]);

            break;
        }
        
        //Comments are ignored
        if (strcmp(tmpTok[0], ";") == 0){
           // free(tmpTok[0]);
        }
        //Get action
        else if (strcmp(tmpTok[0], "ACTION") == 0){
            if (actionSet){
                status = INV_ALARM;
            }else{
                strcpy(tmpAlarm->action, tmpTok[1]);
                actionSet = true;
            }
        }else if (strcmp(tmpTok[0], "TRIGGER") == 0){
            if (triggerSet){
                status = INV_ALARM;
            }else{
                tmpAlarm->trigger = (char*)malloc(strlen(tmpTok[1])+1);
                strcpy(tmpAlarm->trigger, tmpTok[1]);
                triggerSet = true;
            }
        }else if (strcmp(tmpTok[0], "END") == 0 && strcmp(tmpTok[1], "VALARM") == 0){
            free(tmpTok[0]);
            free(tmpTok[1]);

            break;
        }else{
        	Property* tmpProp = createProperty(tmpTok);
            if (tmpProp ==NULL){
                status = INV_ALARM;
            }

            insertBack(&tmpAlarm->properties, (void*)tmpProp);  
        }
        free(tmpTok[0]);
        free(tmpTok[1]);
        
        if (status != OK){
            break;
        }
    }
    
    if (status != OK){
        deleteAlarm(tmpAlarm);
    }else{
        *newAlarm = tmpAlarm;
    }
    
    
    return status;
}

bool getDate(char* textStr, DateTime* tmpDT){
	char* tmpTok[2];
    
	tmpTok[0] = strtok(textStr, "T");
	tmpTok[1] = strtok(NULL, "T");
    if (tmpTok[0] == NULL){
        tmpTok[0] = malloc(sizeof(char));
    }
    
    if (tmpTok[1] == NULL){
        tmpTok[1] = malloc(sizeof(char));
    }
    
    int len1 = strlen(tmpTok[0]);
    int len2 = strlen(tmpTok[1]);
    
    if (len1 != 8 || len2 < 6 || len2 > 7){
        return false;
    }
    
    strcpy(tmpDT->date, tmpTok[0]);
    if (tmpTok[1][6] == 'Z'){
        tmpDT->UTC = true;
        tmpTok[1][6]='\0';
    }else{
        tmpTok[1][6]='\0';
        tmpDT->UTC = false;
    }
    strcpy(tmpDT->time, tmpTok[1]);
    
    return true;
}

bool isFolded(FILE* file){
    char lineHead[3];
    int i;
    for (i = 0; i < 3 && !feof(file); i++){
        lineHead[i] = fgetc(file);
    }
    
    if (lineHead[0] == '\r' && lineHead[1] == '\n' &&
        (lineHead[2] == ' ' || lineHead[2] == '\t') ){
        
        //put them back, return true
        ungetc(lineHead[2], file);
        ungetc(lineHead[1], file);
        ungetc(lineHead[0], file);
        
        return true;
        
    }else{
        //put them back, return false
        if (!feof(file)){
            ungetc(lineHead[2], file);
            ungetc(lineHead[1], file);
            ungetc(lineHead[0], file);
        }
        
        return false;
    }
}

bool getLine(char* line, FILE* file){
    char* retVal = fgets(line, 10000, file);
    //If at the end of file
    if (retVal == NULL){
        return false;
    }
    
    //If last two chars are not CRLF, invalid file format
    char* eolPos = strstr(line, "\r\n");
    if (eolPos == NULL){
        return false;
    }
    ungetc(eolPos[1], file);
    ungetc(eolPos[0], file);
    
    *eolPos = '\0';
    return true;
}

bool parseFoldedLine(FILE* file, char* tokens[]){
    //fgets a line - how does it work, what happens with \r\n?
    char line[10000];
    
    if (!getLine(line, file)){
        return false;
    }
    
    char* unfoldedLine = malloc(sizeof(char)*(1+strlen(line)));
    strcpy(unfoldedLine, line);
    
    while (isFolded(file)){
        fgetc(file);
        fgetc(file);
        fgetc(file);
        
        if (!getLine(line, file)){
            return false;
        }
        
        int newLen = strlen(line)+strlen(unfoldedLine)+1;
        unfoldedLine = realloc(unfoldedLine, sizeof(char) * newLen);
        strcat(unfoldedLine, line);
    }
    fgetc(file);
    fgetc(file);
    
    //Add dealing with comment lines
    if (unfoldedLine[0] == ';'){
        printf("%s\n", unfoldedLine);
        tokens[0] = malloc(sizeof(char)*2);
        tokens[1] = malloc(sizeof(char)*1);
        strcpy(tokens[0], ";");
        strcpy(tokens[1], "");
        free(unfoldedLine);
        return true;
    }
    
    
    //First occurrent of this symbol
    char* ret = strpbrk(unfoldedLine, ":;");
    if (ret == NULL){
        return false;
    }
    *ret = '\0';
    
    tokens[0] = malloc(sizeof(char)*(1+strlen(unfoldedLine)));
    tokens[1] = malloc(sizeof(char)*(1+strlen(ret+1)));
    strcpy(tokens[0], unfoldedLine);
    strcpy(tokens[1], ret+1);
    /*
     if (strlen(tokens[0]) == 0 || strlen(tokens[1]) == 0){
         free(unfoldedLine);
         return false;
     }
    */
    free(unfoldedLine);
    return true;
}

void parseLine(char* line, char* tokens[]){
	char *ret;

    //Comment line
    if (line[0] == ';'){
        tokens[0] = malloc(2);
        strcpy(tokens[0], ";");
        tokens[1] = NULL;
        return;
    }
    
	line[strcspn(line, "\r\n")] = 0;
   // printf("Line %s\n", line);	

    //First occurrent of this symbol
	ret = strpbrk(line, ":;");
	*ret = '\0'; 

	tokens[0] = line;
	//tokens[1] = strtok(NULL, ":");	
	tokens[1] = ret+1;
    //printf("parsed into %s %s\n", tokens[0], tokens[1]);
}

bool getVersion(char* tmpTok[], float* val){
    float tmpVal;

	if (strcmp(tmpTok[0], "VERSION") != 0){
		return false;
	}

	if (sscanf(tmpTok[1], "%f", &tmpVal) != 1){
		return false;
	}
    
    if (tmpVal <= 0){
        return false;
    }
    
    *val = tmpVal;

	return true;
}

bool getProd(char* tmpTok[], char* prod){

	if (strcmp(tmpTok[0], "PRODID") != 0){
		return false;
	}

    if (tmpTok[1] == NULL){
		return false;
	}

    if (strcmp(tmpTok[1], "") == 0){
        return false;
    }
    
    strcpy(prod, tmpTok[1]);

	return true;
}
