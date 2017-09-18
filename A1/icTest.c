#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum ers {OK, INV_FILE, INV_CAL, INV_VER, DUP_VER, INV_PRODID, DUP_PRODID, INV_EVENT, INV_CREATEDT} ErrorCode;


ErrorCode parseCalendar (FILE * fp){
    int checkVer = 0;
    int checkID = 0;
    char buffer[75];
    char * calVer;
    char * calID;
    fgets(buffer, 75, fp);
    char * token = strtok(buffer, ":; \t");
    while (1){
        if (strcmp(token,"VERSION")==0){
            if (checkVer == 0){
                token = strtok(NULL, ":;\n");
                calVer = malloc (sizeof(char)*strlen(token));
                strcpy(calVer, token);
                checkVer = 1;
            } else {
                free(calVer);
                if (checkID == 1)
                    free(calID);
                return DUP_VER;
            }
        } else if (strcmp(token,"PRODID")==0){
            if (checkID == 0){
                token = strtok(NULL, ":;\n");
                calID = malloc (sizeof(char)*strlen(token));
                strcpy(calID, token);
                checkID = 1;
            } else {
                free(calID);
                if (checkVer == 1)
                    free(calVer);
                return DUP_PRODID;
            }
        } else if (strcmp(token,"BEGIN")==0){

        } else if (strcmp(token,"COMMENT")==0){

        } else {

        }
        fgets(buffer, 75, fp);
        token = strtok(buffer, ":; \t");
    }    
    return OK;
}


ErrorCode createCalendar(char* fileName){
    
    //check that the file exists and open it
    FILE * fp = fopen(fileName,"r");
    if (fp == NULL){
        return INV_FILE;
    }
    char buffer[75];
    fgets(buffer,75,fp);
    printf("%s\n", buffer);

    char * token = strtok(buffer, ":; \t");

    while (1){
        if (strcmp(token, "BEGIN")==0){
            printf("%s\n", token);
            /* if the next word is not VCALENDAR then the file is wrong
            and INV_CAL is returned */
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
    }
    


    return OK;
}

int main(int argc, char * argv[]){
    
    char * fileName = "simpleICFile.ics";
    createCalendar(fileName);
}