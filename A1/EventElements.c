/*
 * EventElements.c
 *
 * CIS2750
 * Assignment 2
 * Author: Jessica Authier, 0848720
 * Last Modified: 2017/09/21
 * 
 * This file contains functions that create, manipulate and destroy
 * the elements that can be found in an Event object.
 */

#include "EventElements.h" 

/* ------------------------Alarm------------------------ */

Alarm * initAlarm(char * action, char * trigger, List propList){
    Alarm * newAlarm = malloc(sizeof(Alarm));
    newAlarm->trigger = malloc(sizeof(char)*(strlen(trigger)+1));
    strcpy(newAlarm->action, action);
    strcpy(newAlarm->trigger,trigger);
    newAlarm->properties = propList;
    return newAlarm;
}

char * printAlarm(void * toBePrinted){
    Alarm * toPrint = (Alarm *)toBePrinted;

    char * list = toString(toPrint->properties);
    int len = strlen(list)+strlen(toPrint->action)+strlen(toPrint->trigger)+10;
    char * str = malloc(sizeof(char)*len);
    sprintf(str,"%s\n%s\n%s\n", toPrint->action,toPrint->trigger,list);
    free(list);
    return str;
}
//TEST
int compareAlarm(const void * first,const void * second){
    char * action1 = ((Property *)first)->action;
    char * action2 = ((Property *)second)->action;
    return strcmp(action1, action2);
}

void deleteAlarm(void * toDelete){
    free(((Alarm *)toDelete)->trigger);
    free((Alarm *)toDelete);
}

/* ------------------------Property------------------------ */

Property * initProperty(char * name, char * descr){
    Property * newProp = malloc(sizeof(Property) + (sizeof(int)*strlen(descr)));
    strcpy(newProp->propName, name);
    strcpy(newProp->propDescr,descr);
    return newProp;
}

char * printProperty(void * toBePrinted){
    Property * toPrint = (Property *)toBePrinted;
    int len = strlen(toPrint->propName)+strlen(toPrint->propDescr)+10;
    char * str = malloc(sizeof(char)*len);
    sprintf(str,"%s\n%s\n", toPrint->propName,toPrint->propDescr);
    return str;
}
//TEST
int compareProperty(const void * first,const void * second){
    char * name1 = ((Property *)first)->propName;
    char * name2 = ((Property *)second)->propName;
    return strcmp(name1, name2);
}

void deleteProperty(void * toDelete){
    free((Property *)toDelete);
}

/* ------------------------DateTime------------------------ */

DateTime * initDT (char * str){

    DateTime * newDT = malloc(sizeof(DateTime));

    char date[9] = "";
    char time[7] = "";

    if (strlen(str) == 15)
        newDT->UTC = true;
    else
        newDT->UTC = false;
    int i = 0;
    for (i=0;i<8;i++){
        date[i] = str[i];
    }
    int j = 0;
    i = 8;
    for (i=8;i<14;i++){
        time[j] = str[i];
        j++;
    }
    strcpy(newDT->date, time);
    strcpy(newDT->time, time);
    return newDT;
}

char * printDT(DateTime * dt){

    int len = strlen(dt->date)+strlen(dt->time)+6;
    char * dtStr = malloc(sizeof(char)*len);
    sprintf(dtStr, "%s, %s", dt->date, dt->time);
    if (dt->UTC == true)
        strcat(dtStr," UTC");
    return dtStr;
}

void deleteDT(DateTime * toDelete){
    free(toDelete);
}
