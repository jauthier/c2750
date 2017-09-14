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

typedef struct iter{
    Node* current;
} ListIterator;

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

ListIterator createIterator(List list){
    ListIterator newIter; 
    newIter.current = list.head;
    return newIter;
}

void* nextElement(ListIterator* iter){
  printf("in next element\n");
    return iter->current->next->data;
}

char* toString(List list){
    //create an iterator
    ListIterator iter = createIterator(list);
    
    if (iter.current != NULL){
      printf("in if\n");
        char * temp = list.printData(iter.current->data);
        int len = strlen(temp);
        int mem = len * 4;
        char * str = malloc (sizeof(char)*mem);
        strcpy(temp,str);
        free(temp);
        iter.current = iter.current->next;
        while (iter.current != NULL){
	  printf("in while\n");
            char * hold = list.printData(iter.current->data);
            len = len + strlen(hold) + 1;

            /*check if there is enough memory*/
            if (len > mem){
	      printf("ran out of mem\n");
	      // allocate more
                mem = len * 2;
                str = (char *) realloc(str,mem);
            }
	    printf("time to add\n");
            strcat(str, "\n");
            strcat(str, hold);
	    
            free(hold);
	    
	    iter.current = iter.current->next;
	    printf("here\n");
	}
        return str;
    }
    else
        return NULL;

}

char * printElement(void * toBePrinted){
	// pointer to a string
    int * num = (int *)toBePrinted;
    char buffer[20];
    int n = sprintf(buffer,"%d",num);
    int len = strlen(buffer);
    
    char * data = malloc(sizeof(char)*len);
    printf("printing data\n");
    strcpy(data, buffer);
	return data;
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
	


	int num1 = 4;
    int num2 = 5;
    int num3 = 7;
    int num4 = 6;
	insertFront(&newList, &num1);
    insertFront(&newList, &num2);
    insertBack(&newList, &num3);

    printf("%s\n", toString(newList));
    
    return 0;
}
