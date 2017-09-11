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
    newNode.data = data;
    next = NULL;
    previous = NULL;
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
	List newList = initializeList(&printELement, &deleteElement, &compareElement);
	


	int num = 4;
	Node * node1 = initializeNode(&num);

    newList.printData(node1->data);    
    return 0;
}
