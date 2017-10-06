#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "LinkedListAPI.h"

#define LINELEN 75


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

int checkMultiLine (char * firstLine, char * secondLine){
    /*check if the first line reached its length limit */
    if (firstLine[strlen(firstLine) - 1] == '\n'){
        /* check if the second line has a : or ; and starts with whitespace */
        if (secondLine[0] == ' '){
            return 1;
        }
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
                multi = checkMultiLine(current, next);
            else
                multi = 0;
            /* check if the line after the next line is also a multi line */
            while (multi == 1){
                char buffer[75];
                hold = fgets(buffer, 75, fp);
                if (hold != NULL)
                    multi = checkMultiLine(next, buffer);
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
            printf("%s\n", value);
            insertFront(list, (void *)value);
        }
        strcpy(current, next);
    }

    // close the file
    fclose(fp);
    return 1;
}


int main(int argc, char const *argv[]){
    
    char * fileName = "simpleICFile.ics";
    List * list = malloc(sizeof(List));
    fileToList(fileName, list);
    printf("done reading\n");
    char * hold = toString(*list);
    printf("%s\n", hold);
    free(hold);
    clearList(list);
    free(list);

    return 0;
}
