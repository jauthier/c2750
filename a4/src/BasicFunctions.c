/*
* BasicFunctions.c
* CIS 2750
* Assignment 3
* Author: Jessica Authier, 0849720
* 2017/11/16
* 
* This file contains the functions to create, compare, 
* print and destroy Alarms, Properties, DT object, Events and Cal objects.
*/

#include "BasicFunctions.h"

/*-------- wrapper functions used by pyhton --------*/

int getEventLength(void * data){
	Calendar * cal = (Calendar*)data;
	return getLength(cal->events);
	
}

int getEventPropLength(void * data, int evNum){
	Calendar * cal = (Calendar*)data;
	Node * hold = cal->events.head;
	for(int i=0;i<evNum;i++){
		hold = hold->next;
	}
	return getLength(((Event*)hold->data)->properties)+3;
}

int getEventAlarmLength(void * data, int evNum){
	Calendar * cal = (Calendar*)data;
	Node * hold = cal->events.head;
	for(int i=0;i<evNum;i++){
		hold = hold->next;
	}
	return getLength(((Event*)hold->data)->alarms);
}

char * getSummary(void * data, int evNum){
	Calendar * cal = (Calendar*)data;
	Node * hold = (Node*)cal->events.head;
	for(int i=0;i<evNum;i++){
		hold = hold->next;
	}
	Event * event = (Event *)hold->data;
	char * summary;
	Property * temp = initProperty("SUMMARY","temp");
	Property * ret = findElement(event->properties, custCompareProp, temp);
	if (ret == NULL){
		return "NULL";
	} else {
		summary = malloc(sizeof(char)*(strlen((((Property*)ret)->propDescr)+1)));
		strcpy(summary,((Property*)ret)->propDescr);
		return summary;
	}
}

char * initCalWrapper(Calendar ** objPtr, char * prodID, float version){
	//check the parameters
	if (prodID == NULL)
		return "Invalid Product ID";
	if (version != 2)
		return "Invalid Version";
	
	Calendar * obj;
	List propList = initializeList(printProperty,deleteProperty,compareProperty);
	List eventList = initializeList(printEvent,deleteEvent,compareEvent);
	
	obj = initCal(version, prodID, eventList, propList);
	*objPtr = obj;
	return "OK";
}

char * createAddEvent(Calendar ** obj, char * UID, char * dtCreation, char * dtStart){
	/*turn the dt strings into DateTime objects*/
	if (dtCreation == NULL)
		return "Invalid Date-Time";
	DateTime * creationDT = initDT(dtCreation);
	if (creationDT == NULL)
		return "Invalid Date-Time";
	DateTime * startDT;
	if (dtStart == NULL)
		startDT = initDT(dtCreation);
	else 
		startDT = initDT(dtStart);
	if (startDT == NULL)
		return "Invalid Date-Time";
		
	List propList = initializeList(printProperty,deleteProperty,compareProperty);
	List alarmList = initializeList(printAlarm,deleteAlarm,compareAlarm);
	
	Event * event = initEvent(UID,creationDT,startDT,propList,alarmList);
	if (event == NULL){
		clearList(&propList);
		clearList(&alarmList);
		return "Invalid Event";
	}
	insertBack(&((*obj)->events) ,(void *)event);
	return "OK";
}

ICalErrorCode writeToFileWrapper(Calendar ** obj, char * filename){
	//if the calendar is valid then save
	printf("%s",filename);
	ICalErrorCode err = writeCalendar(filename, *obj);
	return err;
}

char * displayA(Calendar ** obj, int evNum){
	Event * event;
	Node * hold = (*obj)->events.head;
	for(int i=0;i<evNum;i++){
		hold = hold->next;
	}
	event = hold->data;
	char * str = toString(event->alarms);
	if (str == NULL)
		return "NULL";
	return str;
}

char * displayP(Calendar ** obj, int evNum){
	Event * event;
	Node * hold = (*obj)->events.head;
	for(int i=0;i<evNum;i++){
		hold = hold->next;
	}
	event = hold->data;
	char * str = toString(event->properties);
	if (str == NULL)
		return "NULL";
	return str;
}

/* returns a string of the form "<Date> <Time> <UTC>" */
char * getStartDT(Calendar ** obj, int evNum){
	Event * event;
	Node * hold = (*obj)->events.head;
	for(int i=0;i<evNum;i++){
		hold = hold->next;
	}
	event = hold->data;
	char * str = printDT(&(event->startDateTime));
	return str;
}

char * getLocation(Calendar ** obj, int evNum){
	Event * event;
	Node * hold = (*obj)->events.head;
	for(int i=0;i<evNum;i++){
		hold = hold->next;
	}
	event = hold->data;
	
	Property * temp = initProperty("LOCATION","temp");
	Property * ret = findElement(event->properties, custCompareProp, temp);
	if (ret == NULL)
		return "NULL";
	else
		return printProperty(ret);
	
}

char * getOrganizer(Calendar ** obj, int evNum){
	Event * event;
	Node * hold = (*obj)->events.head;
	for(int i=0;i<evNum;i++){
		hold = hold->next;
	}
	event = hold->data;
	
	Property * temp = initProperty("ORGANIZER","temp");
	Property * ret = findElement(event->properties, custCompareProp, temp);
	if (ret == NULL)
		return "NULL";
	else
		return printProperty(ret);
}

/*--------------------------------*/

char * toUpper(char * str){
    if (str == NULL)
        return NULL;
    int len = strlen(str);

    char * result =  malloc(sizeof(char)*(len+1));
    int i = 0;
    for (i = 0; i < len; ++i){
        result[i] = toupper(str[i]);
    }
    result[len] = '\0';
    return result;
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
    sprintf(str,"      %s:%s", toPrint->propName,toPrint->propDescr);
    return str;
}

int compareProperty(const void * first,const void * second){
    char * name1 = toUpper(((Property *)first)->propName);
    char * name2 = toUpper(((Property *)second)->propName);
    int check = strcmp(name1, name2);
    free(name1);
    free(name2);
    return check;
}

bool custCompareProp(const void * first,const void * second){
    char * name1 = toUpper(((Property *)first)->propName);
    char * name2 = toUpper(((Property *)second)->propName);
    int check = strcmp(name1, name2);
    free(name1);
    free(name2);
    if (check == 0)
        return true;
    else
        return false;
}

void deleteProperty(void * toDelete){
    free((Property *)toDelete);
}

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
    int len = strlen(toPrint->action)+strlen(toPrint->trigger)+50;
    if (list != NULL)
        len = len + strlen(list);
    char * str = malloc(sizeof(char)*len);

    if (list == NULL)
        sprintf(str,"      Action:%s\n      Trigger:%s\n", toPrint->action,toPrint->trigger);
    else 
        sprintf(str,"      Action:%s\n      Trigger:%s\n      Properties:\n%s\n", toPrint->action,toPrint->trigger,list);

    free(list);
    return str;
}

int compareAlarm(const void * first,const void * second){
    char * action1 = ((Alarm *)first)->action;
    char * action2 = ((Alarm *)second)->action;
    return strcmp(action1, action2);
}

bool custCompareAlarm(const void * first,const void * second){
    char * action1 = ((Alarm *)first)->action;
    char * action2 = ((Alarm *)second)->action;
    if (strcmp(action1, action2)==0)
        return true;
    else
        return false;
}

void deleteAlarm(void * toDelete){

    free(((Alarm *)toDelete)->trigger);
    clearList(&(((Alarm *)toDelete)->properties));
    free((Alarm *)toDelete);
}

/* ------------------------DateTime------------------------ */
DateTime * initDT (char * str){

    DateTime * newDT = malloc(sizeof(DateTime));

    char date[9] = "";
    char time[7] = "";

    if (strlen(str) == 16)
        newDT->UTC = true;
    else
        newDT->UTC = false;
    int i = 0;
    for (i=0;i<8;i++){
        date[i] = str[i];
    }
    if (str[8] != 'T'){
        free(newDT);
        return NULL;
    }
    int j = 0;
    i = 8;
    for (i=9;i<15;i++){
        time[j] = str[i];
        j++;
    }
    strcpy(newDT->date, date);
    strcpy(newDT->time, time);
    return newDT;
}

char * printDT(DateTime * dt){

    int len = strlen(dt->date)+strlen(dt->time)+10;
    char * dtStr = malloc(sizeof(char)*len);
    sprintf(dtStr, "%s, %s", dt->date, dt->time);
    if (dt->UTC == true)
        strcat(dtStr," UTC");
    return dtStr;
}

void deleteDT(DateTime * toDelete){
    free(toDelete);
}

/* ------------------------Calendar------------------------ */
Calendar * initCal (float ver, char * id, List eventList, List propList){
    Calendar * newCal = malloc(sizeof(Calendar));
    newCal->version = ver;
    strcpy(newCal->prodID, id);
    newCal->events = eventList;
    newCal->properties = propList;
    return newCal;
}

char* printCalendar(const Calendar* obj){
    if (obj == NULL)
        return NULL;
    char * str;
    char * event = toString(obj->events);
    int len = strlen(obj->prodID) + strlen(event) + 50; 
    str = malloc(sizeof(char)*len);
    sprintf(str, "Calendar\n  Product ID: %s\n  Version: %0.1f\n%s", obj->prodID, obj->version,event);
    free(event);
    return str;
}

/* ------------------------Event------------------------ */
Event * initEvent (char * uID, DateTime * dt, DateTime * dtStart, List propList, List alarmList){
    Event * newEvent = malloc(sizeof(Event));
    strcpy(newEvent->UID,uID);
    newEvent->creationDateTime = *dt;
    newEvent->startDateTime = *dtStart;
    newEvent->properties = propList;
    newEvent->alarms = alarmList;
    return newEvent;
}

void deleteEvent (void * toDelete){

    clearList(&(((Event*)toDelete)->properties));
    clearList(&(((Event*)toDelete)->alarms));
    free((Event *)toDelete);
}

char * printEvent(void * event){
    Event * evt = (Event *) event;
    char * str;
    char * dt = printDT(&(evt->creationDateTime));
    char * list1 = toString(evt->properties);
    char * list2 = toString(evt->alarms);
    int len = strlen(dt) + strlen(evt->UID) + 80;
    if (list1 != NULL)
        len = len +  strlen(list1);
    if (list2 != NULL)
        len = len + strlen(list2);
    str = malloc(sizeof(char)*len);
    if (list1 != NULL && list2 != NULL)
        sprintf(str,"  Event\n    UID: %s\n    Date and Time of Creation: %s\n    Alarms:\n%s    Properties:\n%s",evt->UID,dt,list2,list1);
    else if (list1 != NULL && list2 == NULL)
        sprintf(str,"  Event\n    UID: %s\n    Date and Time of Creation: %s\n    Properties:\n%s",evt->UID,dt,list1);
    else if (list1 == NULL && list2 != NULL)
        sprintf(str,"  Event\n    UID: %s\n    Date and Time of Creation: %s\n    Alarms:\n%s",evt->UID,dt,list2);
    else 
        sprintf(str,"  Event\n    UID: %s\n    Date and Time of Creation: %s\n",evt->UID,dt);

    free(dt);
    free(list1);
    free(list2);
    return str;
}

int compareEvent(const void * first,const void * second){
    return 0;
}
