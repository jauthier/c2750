/*
** readFile.c
** Author: Jessica Authier
** Date Modified: 10/11/2017
** This file contains the functions that takes a file and 
** puts each line of the file into a list
*/
#include "readFile.h"

char * printLine(void * toBePrinted){
    char * hold = (char *)toBePrinted;
    char * str = malloc(sizeof(char)*(strlen(hold)+1));
    strcpy(str,hold);
    return str;
}

void deleteLine(void * toBeDeleted){
    char * hold = (char *)toBeDeleted;
    free(hold);
}

int compareLine(const void *first,const void *second){
    char * str1 = (char *)first;
    char * str2 = (char *)second;
    return strcmp(str1, str2);
}

int isWhitespace (char * str){
    if (str == NULL)
        return -1;
    int len = strlen(str);
    int i = 0;
    for (i=0;i<len;i++){
        if (str[i] != ' ' && str[i] != '\t' && str[i] != '\n')
            return 0;
    }
    return 1;
}

int checkMultiLine (char * secondLine){
    /* if the line starts with a space or a tab */
    if (secondLine[0] == ' ' || secondLine[0] == '\t'){
        return 1;
    } 
    return 0;
}

int fileToList(char * fileName, List * list){

    // open the file
    FILE * fp = fopen(fileName, "r");
    if (fp == NULL)
        return 0;

    // create a list
    *list = (initializeList(printLine, deleteLine, compareLine));

    // iterate through the list putting the lines in the list
    char current[LINELEN], next[LINELEN];

    char * hold = fgets(current,LINELEN,fp);
    while (hold != NULL){
        hold = fgets(next, LINELEN, fp);
        if (isWhitespace(current) == 0){ // if the line is only whitespace it is skipped

            char * holdCurrent = strtok(current, "\n"); // remove the newline
            int len = strlen(holdCurrent) + 1;
            char * value = malloc(sizeof(char)*len);
            strcpy(value, holdCurrent);

            /* ---- check for multi lines ---- */
            int multi;
            if (hold != NULL)
                multi = checkMultiLine(next);
            else
                multi = 0;
            /* check if the line after the next line is also a multi line */
            while (multi == 1){
                char buffer[75];
                hold = fgets(buffer, 75, fp);
                if (hold != NULL)
                    multi = checkMultiLine(buffer);
                else 
                    multi = 0;

                /* realloc and add the next line to the end of value */
                char * temp = strtok(next, "\n");
                temp++;
                len = len + strlen(temp);
                value = realloc(value, len);
                strcat(value, temp);
                strcpy(next, buffer);
            }
            insertBack(list, (void *)value);
        }
        strcpy(current, next);
    }

    // close the file
    fclose(fp);
    return 1;
}
