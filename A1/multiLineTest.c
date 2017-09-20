/*
* CalendarParser.c
*
*
*
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "LinkedListAPI.h"
//#include "CalendarParser.h"

typedef enum ers {OK, INV_FILE, INV_CAL, INV_VER, DUP_VER, INV_PRODID, DUP_PRODID, INV_EVENT, INV_CREATEDT} ErrorCode;

typedef struct dt {
    //YYYYMMDD
    char date[9]; 
    //hhmmss
    char time[7]; 
    //indicates whether this is UTC time
    bool    UTC;  
} DateTime;

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


/* TEST
  str is NULL
  str contains only whitespace
  str contains only non-whitespace
  str contains both whitespace and non-whitespace
*/
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
    /* parsing bits */
    char current[75];
    char * hold = fgets(current,75,fp);
    char next[75];
    int multi;


    printf("In parseCalendar\n");
    while (hold != NULL){
        hold = fgets(next,75,fp);
        /* make sure the line can be parsed */
        if (strchr(current,':') == NULL && strchr(current,';') == NULL){
            /* this handles the case where there are chracters but no : or ; 
               if the line is just whitespace it will be ignored */
            if (isWhitespace(current) != 1) 
                return INV_CAL;
        } else {
            /* if the line doesnt exist then it can't be a multi line */
            if (hold != NULL)
                multi = checkMultiLine(current, next);
            else
                multi = 0;
            /* parse the line */
            char * token = strtok(current, ":; \t");
            char * holdVal = strtok(NULL, ":;\n");
            int len = strlen(holdVal) + 1;
            char * value = malloc(sizeof(char)*len);
            strcpy(value, holdVal);
            /* this loop handles multi lines */
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
            printf("%s:%s\n", token, value);
            if (strcmp(token,"VERSION")==0){
                if (checkVer == 0){ /* make sure this is the only declaration of the version */
                    calVer = atof(value);
                    checkVer = 1;
                } else {
                    if (checkID == 1)
                        free(calID);
                    free(value);
                    return DUP_VER;
                }
            } else if (strcmp(token,"PRODID")==0){
                if (checkID == 0){ /* make sure this is the only declaration of the product ID */
                    calID = malloc (sizeof(char)*strlen(value));
                    strcpy(calID, value);
                    checkID = 1;
                } else {
                    free(calID);
                    free(value);
                    return DUP_PRODID;
                }
            } else if (strcmp(token,"BEGIN")==0){
                if (strcmp(value,"VCALENDAR")==0){
                    free(value);
                    return INV_CAL;
                }
                if (checkID == 1 && checkVer == 1){
                    //Event ** eventPrt = malloc(sizeof(Event*));
                    free(value);
                    return OK;
                }

            } else if (strcmp(token,"END")==0){

            } else {
                if (strcmp(token,"COMMENT")!=0)
                    free(value);
                    return INV_CAL;
            }
            free(value);
        }
        strcpy(current,next);
    }
    return OK;
}

ErrorCode createCalendar(char* fileName){
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
            if (isWhitespace(current) != 1) 
                return INV_CAL;
            /* this handles the case where there is only whitespace */
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
            if (strcmp(token, "BEGIN")==0){
                /* if the next word is not VCALENDAR then the file is wrong
                and INV_CAL is returned */
                if (strcmp(value, "VCALENDAR") == 0){
                    ErrorCode eCode = parseCalendar(fp);
                    free(value);
                    return eCode;
                } else {
                    free(value);
                    return INV_CAL;
                }
            } else {
                /* Comments can be ignored, anything else is invalid */
                if (strcmp(token, "COMMENT") != 0)
                    return INV_CAL;

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
        }
    }
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
    ErrorCode code =  createCalendar(fileName);
    printf("%s\n", printError(code));

    return 0;
}


