/*
* BasicFunctions.h
* CIS 2750
* Assignment 3
* Author: Jessica Authier, 0849720
* 2017/11/16
* 
* This file contains the functions to create, compare, 
* print and destroy Alarms, Properties, DT object, Events and Cal objects.
*/

#ifndef BASICFUNCTIONS_H
#define BASICFUNCTIONS_H

#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "LinkedListAPI.h"
#include "CalendarParser.h"

int pGetLength(void * data);
int getEventPropLength(void * data, int evNum);
int getEventAlarmLength(void * data, int evNum);
char * initCalWrapper(Calendar ** objPtr, char * prodID, float version);
char * createAddEvent(Calendar ** obj, char * UID, char * dtCreation, char * dtStart);
char * displayA(Calendar ** obj, int evNum);
char * displayP(Calendar ** obj, int evNum);
char * getStartDT(Calendar ** obj, int evNum);
char * getLocation(Calendar ** obj, int evNum);
char * getOrganizer(Calendar ** obj, int evNum);
char * toUpper(char * str);
Property * initProperty(char * name, char * descr);
char * printProperty(void * toBePrinted);
int compareProperty(const void * first,const void * second);
bool custCompareProp(const void * first,const void * second);
void deleteProperty(void * toDelete);
Alarm * initAlarm(char * action, char * trigger, List propList);
char * printAlarm(void * toBePrinted);
int compareAlarm(const void * first,const void * second);
bool custCompareAlarm(const void * first,const void * second);
void deleteAlarm(void * toDelete);
DateTime * initDT (char * str);
char * printDT(DateTime * dt);
void deleteDT(DateTime * toDelete);
Calendar * initCal (float ver, char * id, List eventList, List propList);
char* printCalendar(const Calendar* obj);
Event * initEvent (char * uID, DateTime * dt, DateTime * dtStart, List propList, List alarmList);
void deleteEvent (void * toDelete);
char * printEvent(void * event);
int compareEvent(const void * first,const void * second);

#endif
