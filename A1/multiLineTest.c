/*
* CalendarParser.c
*
*
*
*/

#include "CalendarParser.h"



int checkMultiLine (char * firstLine, char * secondLine){
    /*check if the first line reached its length limit */
    if (strlen(firstLine) == 75){
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
    
    //check that the file exists and open it
    FILE * fp = fopen(fileName,"r");
    if (fp == NULL){
        return INV_FILE;
    }
    char current[75];
    char next[75];
    int multi;
    

    while (fgets(current,75,fp) != NULL){
        /* if the line doesnt exist then it can't be a multi line */
        if (fgets(next,75,fp) != NULL){
            multi = checkMultiLine(current, next);
        } else {
            multi = 0;
        }


        //char * token = strtok(buffer, ":; \t");



        /*if (strcmp(token, "BEGIN")==0){
            printf("%s\n", token);
            /* if the next word is not VCALENDAR then the file is wrong
            and INV_CAL is returned //
            token = strtok(NULL,":;\n");
            printf("%s\n", token);
            if (strcmp(token, "VCALENDAR") == 0){
                ErrorCode eCode = parseCalendar(fp);
                return eCode;
            } else {
                return INV_CAL;
            }
        } else if (strcmp(token, "COMMENT")==0){
            printf("comment\n");
        } else {
            return INV_CAL;
        }
        */
    }
    


    return OK;
}


int main(int argc, char * argv[]){
    
    char * fileName = "simpleICFile.ics";
    createCalendar(fileName);
}


