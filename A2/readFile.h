#ifndef READFILE_H
#define READFILE_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "LinkedListAPI.h"

#define LINELEN 76

/*
** Returns the the line in the form of a string that must be freed
*/
char * printLine(void * toBePrinted);

/*
** Frees the memory used by the line
*/
void deleteLine(void * toBeDeleted);

/*
** Comapres two lines using strcmp.
** return: < 1 if first < second, > 1 if first > second, 0 if first = second 
*/
int compareLine(const void *first,const void *second);

/*
** Checks if the line only contains whitespace.
** return: 1 if all white space 0 if not
*/
int isWhitespace (char * str);

/*
** Checks if secondLine is part of the firstLine.
** return: 1 if multi line and 0 if not
*/
int checkMultiLine (char * secondLine);

/*
** parameters: fileName - the name of the file
** 			   list - a pointer to the list for the file
**
** The contents of the file is read and each line of the file
** is added to the list
**
** return: 1 on success and 0 on failure
*/
int fileToList(char * fileName, List * list);

#endif
