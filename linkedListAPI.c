/*
* LinkedListAPI.c
* Author: Jessica Authier
* 2017/09/11
*/

#include "LinkedListAPI.h"

List initializeList(char* (*printFunction)(void *toBePrinted),void (*deleteFunction)(void *toBeDeleted),int (*compareFunction)(const void *first,const void *second)){
    
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
    
    if (list.head == NULL)
        return;

    Node * current = list.head;
    Node * next = current->next;
    while (next != NULL){
        list.deleteData(current->data);
        free(current);
        current = next;
        next = current->next;
    }
    list.deleteData(current->data);
    free(current);
}

//TEST
void insertSorted(List *list, void *toBeAdded){
    
}

//TEST
void* deleteDataFromList(List *list, void *toBeDeleted){
    //create an iterator
    ListIterator * iter = createIterator(list);

    /* find the node */
    while (iter->data != toBeAdded){
        iter = nextElement(iter);
    }
    /* save the next and previous*/
    Node * next = iter->next;
    Node * previous = iter->previous;
    /* delete */
    list.deleteData(iter->data);
    free(iter);
    /* reassemble the list */
    previous->next = next;
    next->previous = previous;
}

void* getFromFront(List list){
    return list.head->data;
}

void* getFromBack(List list){
    return list.tail->data;
}

//TEST
char* toString(List list){
    //create an iterator
    ListIterator * iter = createIterator(list);
    while (iter != NULL){
        list.printData(iter->data);
        printf("\n");
        iter = nextElement(iter);
    }
}

//TEST
ListIterator createIterator(List list){
    ListIterator newIter = malloc(sizeof(ListIterator));
    newIter->current = list.head;
    return newIter;
}

//TEST
void* nextElement(ListIterator* iter){
    return iter->current->next->data;
}
