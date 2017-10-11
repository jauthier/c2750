#include "CalendarParser.h"
#include "readFile.h"

void free1(char * value, List * list){
    clearList(list);
    free(list);
    free(value);
}


ErrorCode createCalendar(char* fileName, Calendar ** obj){
    /* call fileToList to read the file and put it in a list all multi 
    lines are unfolded and all lines with only white space are removes */
    List * list = malloc(sizeof(List));
    int fileCheck = fileToList(fileName, list);
    if (checkFile == 0){
        free(list);
        return INV_FILE;
    }

    Node * current = list->head;
    if (current == NULL){
        free(list);
        return INV_FILE;
    }

    while (current != NULL){
        char * line = (char *)current->data;
        /* make sure the line can be parsed */
        if (strchr(current,':') == NULL && strchr(current,';') == NULL){
            clearList(list);
            free(list);
            return INV_CAL;
        }

        /* parse the line */
        char * token = strtok(current, ":;\t");
        char * holdVal = strtok(NULL, "\n");
        int len = strlen(holdVal) + 1;
        char * value = malloc(sizeof(char)*len);
        strcpy(value, holdVal);

        /* this should be the beginning of the calendar object */
        if (strcmp(token, "BEGIN") == 0){
            /* if the next word is not VCALENDAR then the file is wrong
            and INV_CAL is returned */
            if (strcmp(value, "VCALENDAR") == 0){
                //ErrorCode eCode = parseCalendar(fp,obj);
                free1(value, list);
                return eCode;
            } else {
                free1(value, list);
                return INV_CAL;
            }
        } else {
            if (strcmp(token, "COMMENT") != 0){
                free1(value, list);
                return INV_CAL;
            }
        }
        free(value);
        current = current->next;
    }
    clearList(list);
    free(list);
    return INV_CAL;
}

void deleteCalendar(Calendar * obj){
    // deleteEvent(obj->event);
    // free(obj);
}

char* printCalendar(const Calendar* obj){
    char * str;
    // char * event = printEvent(obj->event);
    // int len = strlen(obj->prodID) + strlen(obj->event->UID) + strlen(event) + 40; 
    str = malloc(sizeof(char)*10);
    // sprintf(str, "Product ID: %s\nVersion: %f\n%s", obj->prodID, obj->version,event);
    // free(event);
    strcpy(str,"temp\n");
    return str;
}

const char * printError (ErrorCode err){
    if (err == INV_CAL)
        return "Invaid Calendar\n";
    if (err == OK)
        return "Ok\n";
    if (err == INV_FILE)
        return "Invaid File\n";
    if (err == INV_VER)
        return "Invaid Version\n";
    if (err == INV_PRODID)
        return "Invaid Product ID\n";
    if (err == INV_EVENT)
        return "Invaid Event\n";
    if (err == DUP_VER)
        return "Duplicate Version\n";
    if (err == DUP_PRODID)
        return "Duplicate Product ID\n";
    if (err == INV_CREATEDT)
        return "Invaid DateTime\n";
    return "Other Error\n";
}
