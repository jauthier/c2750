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

        printf("%s:%s\n", token, value);
        strcpy(current,next);
    }
    
    return OK;
}


int main(int argc, char * argv[]){
    
    char * fileName = "simpleICFile.ics";
    createCalendar(fileName);
}


