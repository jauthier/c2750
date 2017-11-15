/*
* LinkedListAPI.c
* CIS 2750
* Assignment 3
* Author: Jessica Authier, 0849720
* 2017/11/13
* 
* This file contains the functions to create, manipulate and destroy a double linked list.
*/

#include "LinkedListAPI.h"

List initializeList(char* (*printFunction)(void *toBePrinted),void (*deleteFunction)(void *toBeDeleted),int (*compareFunction)(const void *first,const void *second)){
    List newList;
    newList.head = NULL;
    newList.tail = NULL;
    newList.length = 0;
    newList.deleteData = deleteFunction;
    newList.compare = compareFunction;
    newList.printData = printFunction;
    return newList;
}

Node * initializeNode(void * data){
    Node * newNode = malloc(sizeof(Node));
    newNode->data = data;
    newNode->next = NULL;
    newNode->previous = NULL;
    return newNode;
}

void insertFront(List *list, void *toBeAdded){
    if(list == NULL)
        return;
    if (toBeAdded == NULL)
        return;
    /*create the node*/
    Node * toAdd = initializeNode(toBeAdded);
    if (list->head == NULL){
        /* 
         if the head is NULL then there are no elements and the 
         element we are adding will be the first and last
        */
        list->head = toAdd;
        list->tail = toAdd;
    } else {
        /*
         if the element is not NULL then the element we are adding
         will be added to the front (become the new head) and the 
         previous head will be updated
        */
        Node * hold = list->head; 
        toAdd->next = hold; 
        hold->previous = toAdd;
        list->head = toAdd;
    }
    list->length++;
}

void insertBack(List *list, void *toBeAdded){
    if(list == NULL)
        return;
    if (toBeAdded == NULL)
        return;
    /*create the node*/
    Node * toAdd = initializeNode(toBeAdded);
    if (list->head == NULL){
        /* 
         if the head is NULL then there are no elements and the 
         element we are adding will be the first and last
        */
        list->head = toAdd;
        list->tail = toAdd;
    } else {
        /*
         if the element is not NULL then the element we are adding
         will be added to the back (become the new tail) and the 
         previous tail will be updated
        */
        Node * hold = list->tail; 
        toAdd->previous = hold; 
        hold->next = toAdd;
        list->tail = toAdd;
    }
    list->length++;
}

void clearList(List *list){
    if(list == NULL)
        return;
    if (list->head == NULL)
        return;

    Node * current = list->head;
    Node * next = current->next;
    while (next != NULL){
        list->deleteData(current->data);
        free(current);
        current = next;
        next = current->next;
    }
    list->deleteData(current->data);
    free(current);
    list->head = NULL;
    list->tail = NULL;
    list->length = 0;
}

void insertSorted(List *list, void *toBeAdded){
    if (list == NULL )
        return;
    if (toBeAdded == NULL)
        return;
    // for the compare function should return 0 when nodes are the same
    // greater than 0 if the first node is greater than
    // less than 0 if the first node is less than 

    /*create the node*/
    Node * newNode = initializeNode(toBeAdded); 
    /*check if the list is empty*/
    if (list->head == NULL){
        list->head = newNode;
        list->tail = newNode;
        return;
    }

    Node * hold = list->head; 
    /* compare the first */
    if (list->compare(newNode->data, hold->data) < 0){
        newNode->next = hold; 
        hold->previous = newNode;
        list->head = newNode;
    } else {
        /* compare the middle nodes */
        hold = hold->next;
        while (list->compare(newNode->data, hold->data) > 0 && hold->next != NULL){
            hold = hold->next;
        }

        if (list->compare(newNode->data, hold->data) <= 0 && hold->next == NULL){
            /* add middle */
            newNode->previous = hold->previous;
            newNode->next = hold;
            (hold->previous)->next = newNode;
            hold->previous = newNode;

        } else {
            /* add end */ 
            newNode->previous = hold; 
            hold->next = newNode;
            list->tail = newNode;
        }
    }
    list->length++;
}

void* deleteDataFromList(List *list, void *toBeDeleted){
    /* if there is no list data */
    if(list == NULL)
        return NULL;
    if (list->head == NULL)
        return NULL;
    if (toBeDeleted == NULL)
        return NULL;
    /*create an iterator*/
    ListIterator iter = createIterator(*list);

    /* check the first */
    if (list->compare(toBeDeleted, iter.current->data) == 0){
        /* save the next */
        Node * next = iter.current->next;
        /* hold the data */    
        void * dataHold = iter.current->data;
        /* delete the node */
        free(iter.current);
        /* reassemble the list */
        next->previous = NULL;
        list->head = next;
        if (next->next == NULL)
            list->tail = next;
        list->length--;
        return dataHold;
    /* check the middle nodes and end node */
    } else {
        iter.current = iter.current->next;
        while (list->compare(toBeDeleted, iter.current->data) != 0 && iter.current->next != NULL){
            iter.current = iter.current->next;
        }
        /* if we found a match and are not at the end */
        if (iter.current->next != NULL && list->compare(toBeDeleted, iter.current->data) == 0){
            /* save the next and previous*/
            Node * next = iter.current->next;
            Node * previous = iter.current->previous;
            /* hold the data */
            void * dataHold = iter.current->data;
            /* delete */
            free(iter.current);
            /* reassemble the list */
            previous->next = next;
            next->previous = previous;
            list->length--;
            return dataHold;
        
        /* if we found a match and are at the last node */
        } else if (iter.current->next == NULL && list->compare(toBeDeleted, iter.current->data) == 0) {
            /* save the previous*/
            Node * previous = iter.current->previous;
            /* hold the data */
            void * dataHold = iter.current->data;
            /* delete */
            free(iter.current);
            /* reassemble the list */
            previous->next = NULL;
            list->tail = previous;
            list->length--;
            return dataHold;

        /* if we didn't find a match but are at the last node */
        } else {
            return NULL;
        }
    }
}

void* getFromFront(List list){
    if (list.head == NULL)
        return NULL;
    return list.head->data;
}

void* getFromBack(List list){
    if (list.head == NULL)
        return NULL;
    return list.tail->data;
}

char* toString(List list){
    /* create an iterator */
    ListIterator iter = createIterator(list);
    
    if (iter.current != NULL){
        char * temp = list.printData(iter.current->data);
        int len = strlen(temp);
        int mem = len * 4;
        char * str = malloc (sizeof(char)*mem);
        strcpy(str,temp);
        free(temp);
        iter.current = iter.current->next;
        while (iter.current != NULL){
            char * hold = list.printData(iter.current->data);
            len = len + strlen(hold) + 1;

            /* check if there is enough memory */
            if (len > mem){
                /* allocate more */
                mem = len * 2;
                str = (char *) realloc(str,mem);
            }
            strcat(str, "\n");
            strcat(str, hold);
        
            free(hold);
        
        iter.current = iter.current->next;
        }
        return str;
    } else {
        return NULL;
    }
}

ListIterator createIterator(List list){
    ListIterator newIter; 
    newIter.current = list.head;
    return newIter;
}

void* nextElement(ListIterator* iter){
    iter->current = iter->current->next;
    if (iter->current == NULL)
        return NULL;

    return iter->current->data;
}

int getLength(List list){
    /* empty list */
    if (list.head == NULL)
        return 0;

    return list.length;
}

void * findElement(List list, bool (*customCompare)(const void* first,const void* second), const void * searchRecord){
    Node * hold = list.head;
    while (hold != NULL){
        if (customCompare(hold->data,searchRecord) == true)
            return hold->data;
        hold = hold->next;
    }
    return NULL;
}

int isMultiple(List list, void * data, bool (*customCompare)(const void* first,const void* second)){
    Node * hold = list.head;
    int count = 0;
    while (hold != NULL){
        if (customCompare(hold->data,data) == true)
            count ++;
        hold = hold->next;
    }
    if (count > 1)
        return 2;
    else
        return count;
}
