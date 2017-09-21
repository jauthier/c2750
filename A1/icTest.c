#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "LinkedListAPI.h"

typedef enum ers {OK, INV_FILE, INV_CAL, INV_VER, DUP_VER, INV_PRODID, DUP_PRODID, INV_EVENT, INV_CREATEDT} ErrorCode;

//Represents iCalendar Date-time
typedef struct dt {
    //YYYYMMDD
    char date[9]; 
    //hhmmss
    char time[7]; 
    //indicates whether this is UTC time
    bool    UTC;  
} DateTime;

//Represents a generic iCalendar property
typedef struct prop {
    //Property name.  We will assume that the property name, even if malformed, does not exceed 200 bytes
    char    propName[200]; 
    //Property description.  We use a C99 flexible array member, which we will discuss in class.
    char    propDescr[]; 
} Property;

//Represents an iCalendar alarm component
typedef struct alarm {
    //Alarm action.  We will assume that the action, even if malformed, does not exceed 1000 bytes
    char    action[200];
    //Alarm trigger.
    char*   trigger;
    //Additional alarm properties.  All objects in the list will be of type Property.  It may be empty.
    List    properties;
} Alarm;

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

//Represents an iCalendar object
typedef struct ical {
    //iCalendar version
    float   version;
    //Product ID.  We will assume that the UserID, even if malformed, does not exceed 1000 bytes
    char    prodID[1000];
    //Reference to an event.  We will assume that every calendar object will have an event.
    Event* event;
} Calendar;

Calendar * initCal (float ver, char * id, Event * event){
    Calendar * newCal = malloc(sizeof(Calendar));
    newCal->version = ver;
    newCal->prodID = id;
    newCal->event = event;
    return newCal;
}

DateTime * initDT (char * str){

    DateTime * newDT = malloc(sizeof(DateTime));
    char * date = malloc(sizeof(char)*9);
    char * time = malloc(sizeof(char)*7);

    for (int i=0;i<8;i++){

    }

    newDT->
    newDT->
    newDT->

}


ErrorCode parseEvent (FILE * fp, Event ** eventPrt){


    List properties = initalizeList();
    List alarms = initalizeList();

    /* Keeps track of whether or DTSTAMP and UID 
    have been declared. There must be only one
    DTSTAMP declared and only one UID declared */
    int checkUID = 0;
    int checkDT = 0;
    /* hold the values of the DTSTAMP and UID */
    char * evUID;
    DateTime * evDT;
    /* Parsing stuff */
    char buffer[75];
    fgets(buffer, 75, fp);
    char * token = strtok(buffer, ":; \t");
    /* calendar loop */
    while (1){
        if (strcmp(token,"UID")==0||strcmp(token,"uid")==0){
            if (checkVer == 0){ /* make sure this is the only declaration of the UID */
                token = strtok(NULL, ":;\n"); 
                evUID = malloc (sizeof(char)*strlen(token));
                strcpy(evUID, token);
                checkVer = 1;
            } else {
                if (checkID == 1)
                    free(calID);
                return DUP_VER;
            }
        } else if (strcmp(token,"DTSTAMP")==0||strcmp(token,"dtstamp")==0){
            if (checkID == 0){ /* make sure this is the only declaration of the product ID */
                token = strtok(NULL, ":;\n");
                
                checkID = 1;
            } else {
                free(calID);
                return DUP_PRODID;
            }
        } else if (strcmp(token,"BEGIN")==0){
            token  = strtok(NULL, ":;\n");
            if (strcmp(token,"VCALENDAR")==0){
                return INV_CAL;
            }
            if (checkID == 1 && checkVer == 1){
                Event ** eventPrt = malloc(sizeof(Event*));
                ErrorCode = ecode = parseEvent(fp, eventPrt);
            }

        } else if (strcmp(token,"COMMENT")==0){

        } else if (strcmp(token,"END")==0){

        } else {
            return INV_EVENT;
        }
        fgets(buffer, 75, fp);
        token = strtok(buffer, ":; \t");
    }


    return OK;
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
    /* Parsing stuff */
    char buffer[75];
    fgets(buffer, 75, fp);
    char * token = strtok(buffer, ":; \t");
    /* calendar loop */
    while (1){
        if (strcmp(token,"VERSION")==0){
            if (checkVer == 0){ /* make sure this is the only declaration of the version */
                token = strtok(NULL, ":;\n"); 
                calVer = atof(token);
                checkVer = 1;
            } else {
                if (checkID == 1)
                    free(calID);
                return DUP_VER;
            }
        } else if (strcmp(token,"PRODID")==0){
            if (checkID == 0){ /* make sure this is the only declaration of the product ID */
                token = strtok(NULL, ":;\n");
                calID = malloc (sizeof(char)*strlen(token));
                strcpy(calID, token);
                checkID = 1;
            } else {
                free(calID);
                return DUP_PRODID;
            }
        } else if (strcmp(token,"BEGIN")==0){
            token  = strtok(NULL, ":;\n");
            if (strcmp(token,"VCALENDAR")==0){
                return INV_CAL;
            }
            if (checkID == 1 && checkVer == 1){
                Event ** eventPrt = malloc(sizeof(Event*));
                ErrorCode = ecode = parseEvent(fp, eventPrt);
            }

        } else if (strcmp(token,"COMMENT")==0){

        } else if (strcmp(token,"END")==0){

        } else {
            return
        }{

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