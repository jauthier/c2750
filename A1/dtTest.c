#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


//Represents iCalendar Date-time
typedef struct dt {
    //YYYYMMDD
    char date[9]; 
    //hhmmss
    char time[7]; 
    //indicates whether this is UTC time
    bool    UTC;  
} DateTime;

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

    int len = strlen(dt->date)+strlen(dt->time)+5;
    char * dtStr = malloc(sizeof(char)*len);
    sprintf(dtStr, "%s, %s", dt->date, dt->time);
    return dtStr;
}

void deleteDT(DateTime * toDelete){
    free(toDelete);
}

int main(int argc, char const *argv[]){

    char str[16] = "20170821121900z";

    DateTime * dt = initDT(str);

    char * hold = printDT(dt);
    printf("%s\n", hold);
    free(hold);

    deleteDT(dt);
    
    return 0;
}