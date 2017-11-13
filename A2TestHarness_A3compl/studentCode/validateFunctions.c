#include "validateFunctions.h"

/* ----- For validating the properties ----- */
/* returns 1 if the property is valid and 0 if it is not*/

int calProp(List propList, Property * prop){
    char * propName = toUpper(prop->propName);
    if (strcmp(propName,"CALSCALE")==0||strcmp(propName,"METHOD")==0||strcmp(propName,"CALSCALE")==0){ //only one
        //check the list for duplicates
        int check = isMultiple(propList, (void*)prop, custCompareProp);
        free(propName);
        if (check != 1)
            return 0;
        else 
            return 1;
    } else {
        if (strcmp(propName,"PRODID")==0){
            free(propName);
            return 3;
        }
        if (strcmp(propName,"VERSION")==0){
            free(propName);
            return 4;
        }
        free(propName);
        return 0;
    }
}

int eventProp(List propList, Property * prop){
    char * propName = toUpper(prop->propName);
     if (strcmp(propName,"CLASS")==0||strcmp(propName,"CREATED")==0||
        strcmp(propName,"DESCRIPTION")==0||strcmp(propName,"GEO")==0||strcmp(propName,"LAST-MOD")==0||
        strcmp(propName,"LOCATION")==0||strcmp(propName,"ORGANIZER")==0||strcmp(propName,"SEQ")==0||
        strcmp(propName,"PRIORITY")==0||strcmp(propName,"STATUS")==0||strcmp(propName,"SUMMARY")==0||
        strcmp(propName,"TRANSP")==0||strcmp(propName,"URL")==0||strcmp(propName,"RECURID")==0||
        strcmp(propName,"RRULE")==0||strcmp(propName,"LAST-MODIFIED")==0){

        //check the list for duplicates
        int check = isMultiple(propList, (void*)prop, custCompareProp);
        free(propName);
        if (check == 1)
            return 1; // the prop is in the list only once
        else 
            return 0;
    } else if (strcmp(propName,"DURATION")==0){
        //check the list for the other
        Property * temp = initProperty("DTEND","temp");
        int check = isMultiple(propList, (void*)temp, custCompareProp);
        // check for doubles
        int check2 = isMultiple(propList, (void*)prop, custCompareProp);
        free(temp);
        free(propName);
        if (check == 1 && check2 == 1)
            return 1;
        else 
            return 0;
    } else if (strcmp(propName,"DTEND")==0){
        //check the list for the other
        Property * temp = initProperty("DURATION","temp");
        int check = isMultiple(propList, (void*)temp, custCompareProp);
        // check for doubles
        int check2 = isMultiple(propList, (void*)prop, custCompareProp);
        free(temp);
        free(propName);
        if (check == 1 && check2 == 1)
            return 1;
        else 
            return 0;
    } else if (strcmp(propName,"ATTACH")==0||strcmp(propName,"ATTENDEE")==0||strcmp(propName,"CATEGORIES")==0||
        strcmp(propName,"COMMENT")==0||strcmp(propName,"CONTACT")==0||strcmp(propName,"EXDATE")==0||
        strcmp(propName,"RSTATUS")==0||strcmp(propName,"RELATED")==0||strcmp(propName,"RESOURCES")==0||
        strcmp(propName,"RDATE")==0){
        free(propName);
        return 1;
    } else {
        free(propName);
        return 0;
    }
}

int alarmProp(List propList, Property * prop){
    char * propName = toUpper(prop->propName);
    if (strcmp(propName,"ATTACH")==0||strcmp(propName,"DESCRIPTION")==0||strcmp(propName,"SUMMARY")==0){
        int check = isMultiple(propList, (void*)prop, custCompareProp);
        free(propName);
        if (check == 1)
            return 1;
        else 
            return 0;
    } else if (strcmp(propName,"DURATION")==0){
        //check the list for REPEAT
        Property * temp = initProperty("REPEAT","temp");
        int check = isMultiple(propList, (void*)temp, custCompareProp);
        // check for doubles
        int check2 = isMultiple(propList, (void*)prop, custCompareProp);
        free(temp);
        free(propName);
        if (check == 1 && check2 == 1)
            return 1;
        else 
            return 0;
    }else if(strcmp(propName,"REPEAT")==0){
        //check the list for DURATION
        Property * temp = initProperty("DURATION","temp");
        int check = isMultiple(propList, (void*)temp, custCompareProp);
        // check for doubles
        int check2 = isMultiple(propList, (void*)prop, custCompareProp);
        free(temp);
        free(propName);
        if (check == 1 && check2 == 1)
            return 1;
        else 
            return 0;
    } else {
        free(propName);
        return 0;
    }
}

/* ----- For checking if all the required components are included and not NULL ----- */

ICalErrorCode validateEvent(Event * event){
    //check uid
    if (event->UID == NULL)
        return INV_EVENT;
    //check datetime Creation
    if (event->creationDateTime.date == NULL)
        return INV_CREATEDT;
    if (event->creationDateTime.time == NULL)
        return INV_CREATEDT;
    //check datetime Start
    if (event->startDateTime.date == NULL)
        return INV_EVENT;
    if (event->startDateTime.time == NULL)
        return INV_EVENT;
    //check properties
    ICalErrorCode ec = validateProperties(event->properties, eventProp);
    if (ec != OK)
        return INV_EVENT;
    //check alarms
    ec = validateAlarms(event->alarms);
    if (ec != OK)
        return ec;
    return OK;
}

ICalErrorCode validateProperties(List propList, int (*checkFunc)(List list, Property * prop)){

    Node * hold = propList.head;
    while(hold != NULL){
        if(hold->data == NULL)
            return INV_CAL; 
        if(((Property *)hold->data)->propName == NULL)
            return INV_CAL;
        if(((Property *)hold->data)->propDescr == NULL)
            return INV_CAL;
        if (int check = checkFunc(propList, (Property *)hold->data) != 1){
            if (check == 3)
                return DUP_PRODID;
            if (check == 4)
                return DUP_VER;
            return INV_CAL;
        }
        hold = hold->next;
    }
    return OK;
}

ICalErrorCode validateAlarms(List alarmList){
    Node * hold = alarmList.head;
    while(hold != NULL){
        if(hold->data == NULL)
            return INV_ALARM; 
        if(((Alarm *)hold->data)->action == NULL)
            return INV_ALARM;
        if(((Alarm *)hold->data)->trigger == NULL)
            return INV_ALARM;
        ICalErrorCode ec = validateProperties(((Alarm *)hold->data)->properties, alarmProp);
        if (ec != OK)
            return INV_ALARM;
        hold = hold->next;
    }
    return OK;

}
