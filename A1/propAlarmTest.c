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

Property * initProperty(char * name, char * descr){
    Property * newProp = malloc(sizeof(Property));
    strcpy(newProp->propName, name);
    strcpy(newProp->propDescr,descr);
    return newProp;
}

char * printAlarm(void * toBePrinted){
    Alarm * toPrint = (Alarm *)toBePrinted;

    char * list = toString(toPrint->properties);
    int len = strlen(list)+strlen(toPrint->action)+strlen(toPrint->trigger)+4;
    char * str = malloc(sizeof(char)*len);
    sprintf(str,"%s\n%s\n%s\n", toPrint->action,toPrint->trigger,list);
    free(list);
    return str;
}

char * printProperty(void * toBePrinted){
    Property * toPrint = (Property *)toBePrinted;
    int len = strlen(toPrint->propName)+strlen(toPrint->propDescr)+3;
    char * str = malloc(sizeof(char)*len);
    sprintf(str,"%s\n%s\n", toPrint->propName,toPrint->propDescr);
    return str;
}

void deleteAlarm(void * toDelete){
    free(((Alarm *)toDelete)->trigger);
    free((Alarm *)toDelete);
}

void deleteProperty(void * toDelete){
    free((Property *)toDelete);
}

int main(int argc, char const *argv[]){

    char * act = "Make Cats";
    char * trigger = "Tomorrow morning";
    
    char * propName = "Colour";
    char * propDescr = "Blue";
    Property * prop1 = initProperty(propName, propDescr);
    char * holdProp = printProperty(prop1);
    printf("%s",holdProp);
    free(holdProp);
    
    return 0;
}