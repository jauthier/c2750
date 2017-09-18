#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum ers {OK, INV_FILE, INV_CAL, INV_VER, DUP_VER, INV_PRODID, DUP_PRODID, INV_EVENT, INV_CREATEDT} ErrorCode;


ErrorCode parseCalendar (FILE * fp){
    printf("In parseCalendar\n");
    char buffer[75];
    fgets(buffer, 75, fp);
    printf("%s\n", buffer);


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

    while (){
        if (strcmp(token, "BEGIN")==0){
            printf("%s\n", token);
            /* if the next word is not VCALENDAR then the file is wrong
            and INV_CAL is returned */
            token = strtok(NULL,":;\n");
            printf("%s\n", token);
            if (strcmp(token, "VCALENDAR") == 0){
                ErrorCode eCode = parseCalendar(fp);
            } else {
                return INV_CAL;
            }
        } else if (strcmp(token, "COMMENT")==0){
            printf("comment\n");
        } 
    }
    


    return OK;
}

int main(int argc, char * argv[]){
    
    char * fileName = "simpleICFile.ics";
    createCalendar(fileName);
}