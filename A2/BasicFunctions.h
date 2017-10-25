/*
* BasicFunctions.h
* CIS 2750
* Assignment 2
* Author: Jessica Authier, 0849720
* 2017/10/25
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

#include "LinkedListAPI.h"
#include "CalendarParser.h"


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
Event * initEvent (char * uID, DateTime * dt, List propList, List alarmList);
void deleteEvent (void * toDelete);
char * printEvent(void * event);
int compareEvent(const void * first,const void * second);

#endif