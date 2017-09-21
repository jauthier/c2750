#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "LinkedListAPI.h"


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


Alarm * initAlarm(char * action, char * trigger, List propList){
    Alarm * newAlarm = malloc(sizeof(Alarm));
    newAlarm->trigger = malloc(sizeof(char)*(strlen(trigger)+1));
    strcpy(newAlarm->action, action);
    strcpy(newAlarm->trigger,trigger);
    newAlarm->properties = propList;
    return newAlarm;
}

//Property * initProperty(){}






int main(int argc, char const *argv[]){
    
    return 0;
}