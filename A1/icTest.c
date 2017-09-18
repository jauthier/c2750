#include <stdio.h>
#include <string.h>

typedef enum ers {OK, INV_FILE, INV_CAL, INV_VER, DUP_VER, INV_PRODID, DUP_PRODID, INV_EVENT, INV_CREATEDT} ErrorCode;

ErrorCode createCalendar(char* fileName){
    
    //check that the file exists and open it
    FILE * fp = fopen(fileName,"r");
    if (fp == NULL){
        return INV_FILE;
    }
    char buffer[75];
    /* keep track of the things started and ended */
    int numBegins = 0;
    int numEnds = 0;


    while (fgets(buffer,75,fp) != NULL){
        printf("%s\n", buffer);

        char * token = strtok(buffer, ":;");
        printf("%s\n", token);

        if (strcmp(token, "BEGIN")==0){
            /* the first occurance of BEGIN */
            if (numBegins ==0){
                /* if the next word is not VCALENDAR then the file is wrong
                and INV_CAL is returned */
                token = strtok(NULL,":;\n");
                printf("%s\n", token);

            }
        } else{
            printf("not the beginning\n");
        }
    }


    return 0;
}

int main(int argc, char * argv[]){
    
    char * fileName = "simpleICFile.ics";
    createCalendar(fileName);
}