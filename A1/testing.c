
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

ErrorCode parseCalendar (FILE * fp, Calendar ** obj){
	return OK;
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
                    //fclose(fp);
                    return eCode;
                } else {
                    //freeCal(value, fp);
                    return INV_CAL;
                }
            } else {
                /* Comments can be ignored, anything else is invalid */
                if (strcmp(token, "COMMENT") != 0){
                    //freeCal(value, fp);
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
