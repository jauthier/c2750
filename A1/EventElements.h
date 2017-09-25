#ifndef EVENTELEMENTS_H
#define EVENTELEMENTS_H

#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "LinkedListAPI.h"
#include "CalendarParser.h"

Alarm * initAlarm(char * action, char * trigger, List propList);

char * printAlarm(void * toBePrinted);

int compareAlarm(const void * first,const void * second);

void deleteAlarm(void * toDelete);

Property * initProperty(char * name, char * descr);

char * printProperty(void * toBePrinted);

int compareProperty(const void * first,const void * second);

void deleteProperty(void * toDelete);

/*
 * Takes a string of the form: "20170921132500z", where the first 8
 * characters represent the date (year/month/day) and the last 7
 * characters represent the time (hour:minute:seconds and z if the 
 * the time zone is UTC)
 */
DateTime * initDT (char * str);

char * printDT(DateTime * dt);

void deleteDT(DateTime * toDelete);

#endif