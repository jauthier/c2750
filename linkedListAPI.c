/*
* LinkedListAPI.c
* CIS 2750
* Assignment 0
* Author: Jessica Authier, 0849720
* 2017/09/15
* 
* This file contains the functions to create, manipulate and destroy a double linked list.
*/

#include "LinkedListAPI.h"

List initializeList(char* (*printFunction)(void *toBePrinted),void (*deleteFunction)(void *toBeDeleted),int (*compareFunction)(const void *first,const void *second)){
    List newList;
    newList.head = NULL;
    newList.tail = NULL;
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
}

void insertBack(List *list, void *toBeAdded){
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
}

//TEST
void clearList(List *list){
    
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
}

//TEST
void insertSorted(List *list, void *toBeAdded){
    // for the compare function should return 0 when nodes are the same
    // greater than 0 if the first node is greater than
    // less than 0 if the first node is less than 

    /*create the node*/
    Node * newNode = initializeNode(toBeAdded); 
    /*check if the list is empty*/
    if (list->head == NULL){
        list->head = newNode;
        list->tail = newNode;
    }

    /* If the list is empty */
    if (list->head == NULL){
        list->head = newNode;
        list->tail = newNode;
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
        while (list->compare(newNode->data, hold->data) >= 0 && hold->next != NULL){
            hold = hold->next;
        }

        if (hold->next != NULL){
            /* add middle */
            newNode->next = hold;
            newNode->previous = hold->previous;

        } else {
            /* add end */ 
            newNode->previous = hold; 
            hold->next = newNode;
            list->tail = newNode;
        }

    }

}

//TEST
void* deleteDataFromList(List *list, void *toBeDeleted){
    /* if there is no list data */
    if (list->head == NULL){
        return NULL;
    }

    /*create an iterator*/
    ListIterator iter = createIterator(*list);



    /* find the node 
        -- handle the first and last node being removed properly
    */
    while (strcmp(iter.current->data, toBeAdded)!=0 && iter.current != NULL){
        iter.current = iter.current->next;
    }

    /* if the node doesn't exist */
    if (){
        retutrn NULL;
    }

    /* save the next and previous*/
    Node * next = iter.current->next;
    Node * previous = iter.current->previous;
    
    void * dataHold = iter.current->data;
    /* delete */
    free(iter.current);
    
    /* reassemble the list */
    previous->next = next;
    next->previous = previous;
    
    return dataHold;
}

void* getFromFront(List list){
    return list.head->data;
}

void* getFromBack(List list){
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
    return iter->current->next->data;
}
