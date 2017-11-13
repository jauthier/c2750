/*
* validateFunctions.h
* CIS 2750
* Assignment 3
* Author: Jessica Authier, 0849720
* 2017/11/13
* 
* This file contains the functions to validate a calendar object.
*/

#ifndef VALIDATEFUNCTIONS_H
#define VALIDATEFUNCTIONS_H

#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "LinkedListAPI.h"
#include "CalendarParser.h"
#include "BasicFunctions.h"

int calProp(List propList, Property * prop);
int eventProp(List propList, Property * prop);
int alarmProp(List propList, Property * prop);
ICalErrorCode validateEvent(Event * event);
ICalErrorCode validateProperties(List propList, int (*checkFunc)(List list), Property * prop);
ICalErrorCode validateAlarms(List alarmList);

#endif