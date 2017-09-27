
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "LinkedListAPI.h"
#include "CalendarParser.h"

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

void deleteDT(DateTime * toDelete){
    free(toDelete);
}

void deleteEvent (Event * toDelete){
    deleteDT(&toDelete->creationDateTime);
    clearList(&(toDelete->properties));
    clearList(&(toDelete->alarms));
    free(toDelete);
}

void freeCal(char * value, FILE * fp){
    free(value);
    fclose(fp);
}

ErrorCode parseEvent(FILE * fp, char * currentLine, Event ** eventPtr){
	return OK;

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
