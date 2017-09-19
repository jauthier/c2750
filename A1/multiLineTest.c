/*
* CalendarParser.c
*
*
*
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//#include "CalendarParser.h"

typedef enum ers {OK, INV_FILE, INV_CAL, INV_VER, DUP_VER, INV_PRODID, DUP_PRODID, INV_EVENT, INV_CREATEDT} ErrorCode;

//Represents an iCalendar event component
typedef struct evt {
    //Event user ID.  We will assume that the UserID, even if malformed, does not exceed 1000 bytes
    char        UID[1000];
    //Alarm creation date-time.
    DateTime    creationDateTime;
    //Additional event properties.  All objects in the list will be of type Property.  It may be empty.
    List        properties;
    //List of alarms associated with the event.  All objects in the list will be of type Alarm.  It may be empty.
    List        alarms;
    
} Event;

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

ErrorCode parseCalendar (FILE * fp){
    /* Keeps track of whether or not there has been a 
    version and product ID declared. There must be only
    one version declared and only one product ID declared */
    int checkVer = 0;
    int checkID = 0;
    /* hold the values of the version and porduct ID */
    float calVer;
    char * calID;

    char current[75];
    char * hold = fgets(current,75,fp);
    char next[75];
    int multi;
    
    while (hold != NULL){
        /* if the line doesnt exist then it can't be a multi line */
        hold = fgets(next,75,fp);
        if (hold != NULL)
            multi = checkMultiLine(current, next);
        else
            multi = 0;
        /* parse the line */
        char * token = strtok(current, ":; \t");
        char * hold = strtok(NULL, ":;\n");
        int len = strlen(hold) + 1;
        char * value = malloc(sizeof(char)*len);
        strcpy(value, hold);
        while (multi == 1){
            /* check if the line after the next line is also a multi line */
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

        if (strcmp(token,"VERSION")==0){
            if (checkVer == 0){ /* make sure this is the only declaration of the version */
                calVer = atof(value);
                checkVer = 1;
            } else {
                if (checkID == 1)
                    free(calID);
                return DUP_VER;
            }
        } else if (strcmp(token,"PRODID")==0){
            if (checkID == 0){ /* make sure this is the only declaration of the product ID */
                calID = malloc (sizeof(char)*strlen(value));
                strcpy(calID, value);
                checkID = 1;
            } else {
                free(calID);
                return DUP_PRODID;
            }
        } else if (strcmp(token,"BEGIN")==0){
            if (strcmp(value,"VCALENDAR")==0){
                return INV_CAL;
            }
            if (checkID == 1 && checkVer == 1){
                Event ** eventPrt = malloc(sizeof(Event*));
                ErrorCode ecode = parseEvent(fp, eventPrt);
            }

        } else if (strcmp(token,"COMMENT")==0){

        } else if (strcmp(token,"END")==0){

        } else {
            return
        }
        free(value);
        strcpy(current,next);
    }    
    return OK;
}

ErrorCode createCalendar(char* fileName){
    /* check that the file exists and open it */
    FILE * fp = fopen(fileName,"r");
    if (fp == NULL){
        return INV_FILE;
    }

    char current[75];
    char * hold = fgets(current,75,fp);
    char next[75];
    int multi;
    
    while (hold != NULL){
        /* if the line doesnt exist then it can't be a multi line */
        hold = fgets(next,75,fp);
        if (hold != NULL)
            multi = checkMultiLine(current, next);
        else
            multi = 0;
        /* parse the line */
        char * token = strtok(current, ":; \t");
        char * hold = strtok(NULL, ":;\n");
        int len = strlen(hold) + 1;
        char * value = malloc(sizeof(char)*len);
        strcpy(value, hold);
        while (multi == 1){
            /* check if the line after the next line is also a multi line */
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

        if (strcmp(token, "BEGIN")==0){
            printf("%s\n", token);
            /* if the next word is not VCALENDAR then the file is wrong
            and INV_CAL is returned */
            if (strcmp(value, "VCALENDAR") == 0){
                ErrorCode eCode = parseCalendar(fp);
                return eCode;
            } else {
                return INV_CAL;
            }
        } else if (strcmp(token, "COMMENT")==0){
            continue;
        } else {
            return INV_CAL;
        }
        free(value);
        strcpy(current,next);
    }
    return INV_CAL;
}


int main(int argc, char * argv[]){
    
    char * fileName = "simpleICFile.ics";
    createCalendar(fileName);
}


