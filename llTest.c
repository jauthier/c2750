#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct listNode{
    void *data;
    struct listNode *previous;
    struct listNode *next;
} Node;

typedef struct listHead{
    Node *head;
    Node *tail;
    void (*deleteData)(void *toBeDeleted);
    int (*compare)(const void *first,const void *second);
    char* (*printData)(void *toBePrinted);
} List;


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

void* getFromFront(List list){
    return list.head->data;
}

void* getFromBack(List list){
    return list.tail->data;
}

char * printElement(void * toBePrinted){
	// pointer to an int

    printf("%d\n", *(int *)toBePrinted);
	return "done";
}

void deleteElement(void *toBeDeleted){
	printf("delete\n");
}

int compareElement(const void *first,const void *second){
	printf("compare\n");
	return 0;
}


int main(int argc, char const *argv[]){
	List newList = initializeList(&printElement, &deleteElement, &compareElement);
	


	int num = 4;
	insertFront(&newList, &num);
    newList.printData(getFromFront);
    newList.printData(getFromBack);
    return 0;
}
