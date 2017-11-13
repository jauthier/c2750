#include "LinkedListAPI.h"
#include "testharness.h"
#include "testcases.h"
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/wait.h>


//data management functions for string data

void  testDestroy(void *data){
    free(data);
}
char * testPrint(void *toBePrinted){
    if(toBePrinted!=NULL){
        return strdup((char *)toBePrinted);
    }
    return NULL;
}
int testCompare(const void * one, const void * two)
{
    return strcmp((char*)one, (char*)two);
}

char * createData(char * input)
{
    char * dup = malloc(sizeof(char)*(strlen(input))+1);
    strcpy(dup,input);
    return dup;
}

//manual list creation to test insertions separately

List * emptyList()
{
    List * list = malloc(sizeof(List));
    list->head = NULL;
    list->tail = NULL;
    list->length = 0;
    list->compare = &testCompare;
    list->deleteData = &testDestroy;
    list->printData = &testPrint;
    return list;
}

List * twoList()
{
    List * list = emptyList();
    Node * n = malloc(sizeof(Node));
    n->data = createData("kilroy");
    n->previous = NULL;
    n->next = NULL;
    list->head = n;
    n=malloc(sizeof(Node));
    n->data = createData("zappa");
    n->next = NULL;
    list->head->next = n;
    n->previous = list->head;
    list->tail = n;
    list->length = 2;
    return list;
}

List * threeList()
{
    List * list = emptyList();
    Node * n1 = malloc(sizeof(Node));
    n1->data = createData("kilroy");
    
    Node * n2 = malloc(sizeof(Node));
    n2->data = createData("leeroy");
    
    Node * n3 =malloc(sizeof(Node));
    n3->data = createData("zappa");
    
    list->head = n1;
    list->head->next = n2;
    list->head->previous = NULL;
    
    n2->next = n3;
    n2->previous = n1;
    
    list->tail = n3;
    list->tail->previous = n2;
    list->tail->next = NULL;
    
    list->length = 3;
    
    return list;
}


/*---------------------------------
 List creation
 --------------------------------*/

/*--------
 subtest 1: initial values of List Variables
 ---------*/
SubTestRec initListTest1(int testNum, int subTest){
    List test;
    SubTestRec result;
    char feedback[300];
    test = initializeList(testPrint, testDestroy, testCompare);
    
    // reusing test variable from previous subtest
    if(test.head == NULL){
        if(test.tail == NULL){
            sprintf(feedback, "Subtest %d.%d: head and tail both NULL",testNum,subTest);
            result = createSubResult(SUCCESS, feedback);
            return result;
        }else{
            sprintf(feedback, "Subtest %d.%d: List tail is not NULL on initialization.",testNum,subTest);
            result = createSubResult(FAIL, feedback);
            return result;
        }
    }else if (test.tail != NULL){
        sprintf(feedback, "Subtest %d.%d: List head and tail are not NULL on initialization.",testNum,subTest);
        result = createSubResult(FAIL, feedback);
        return result;
    }else{
        sprintf(feedback, "Subtest %d.%d: List head is not NULL on initialization.",testNum,subTest);
        result = createSubResult(FAIL, feedback);
        return result;
    }
}

SubTestRec initListTest2(int testNum, int subTest){
    List test;
    SubTestRec result;
    char feedback[300];
    test = initializeList(testPrint, testDestroy, testCompare);
    
    // reusing test variable from previous subtest
    if(test.length == 0){
        sprintf(feedback, "Subtest %d.%d: emply list has length 0",testNum,subTest);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }else{
        sprintf(feedback, "Subtest %d.%d: emply list does not have length 0 .",testNum,subTest);
        result = createSubResult(FAIL, feedback);
        return result;
    }
}

testRec* initListTest(int testNum){
    const int numSubs = 2;  //doesn't need to be a variable but its a handy place to keep it
    int subTest = 1;
    char feedback[300];
    
    sprintf(feedback, "Test %d: List Initialization Test", testNum);
    testRec* rec = initRec(testNum, numSubs, feedback);
    
    runSubTest(testNum, subTest, rec, &initListTest1);
    subTest++;
    runSubTest(testNum, subTest, rec, &initListTest2);
    return rec;
    
}

/*---------------------------------
Get length test
--------------------------------*/

/*--------
 subtest 1: initial values of List Variables
 ---------*/
SubTestRec getLengthTest1(int testNum, int subTest){
    List* test;
    SubTestRec result;
    char feedback[300];
    test = threeList();
    
    // reusing test variable from previous subtest
    if(getLength(*test) == 3){
        sprintf(feedback, "Subtest %d.%d: getLength returns length corectly.",testNum,subTest);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }else{
        sprintf(feedback, "Subtest %d.%d: getLength does not return length corectly.",testNum,subTest);
        result = createSubResult(FAIL, feedback);
        return result;
    }
}

testRec* getLengthTest(int testNum){
    const int numSubs = 1;  //doesn't need to be a variable but its a handy place to keep it
    int subTest = 1;
    char feedback[300];
    
    sprintf(feedback, "Test %d: List Initialization Test", testNum);
    testRec* rec = initRec(testNum, numSubs, feedback);
    
    runSubTest(testNum, subTest, rec, &getLengthTest1);
    return rec;
    
}

/********************* Insert tests **********************/

/*--------
 subtest 1:  insert Front empty list
 ---------*/
SubTestRec insertTest1(int testNum, int subTest){
    char feedback[300];
    SubTestRec result;
    List* test = emptyList();
    
    char* data = createData("sally");
    insertFront(test, data);
    if(test->length == 1)
    {
        sprintf(feedback, "Subtest %d.%d: list length correctly updated on insert into empty list.",testNum,subTest);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }
    else
    {
        sprintf(feedback, "Subtest %d.%d: list length incorrectly updated on insert into empty list",testNum,subTest);
        result = createSubResult(FAIL, feedback);
        return result;
    }
}

/*--------
 subtest 2:  insert Front existing List
 ---------*/
SubTestRec insertTest2(int testNum, int subTest){
    char feedback[300];
    SubTestRec result;

    List* test = twoList();
    char* data = createData("sally");
    insertFront(test, data);
    
    if(test->length == 3)
    {
        sprintf(feedback, "Subtest %d.%d: list length correctly updated on insert into populated list.",testNum,subTest);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }
    else
    {
        sprintf(feedback, "Subtest %d.%d: list length incorrectly updated on insert into populated list",testNum,subTest);
        result = createSubResult(FAIL, feedback);
        return result;
    }
}

testRec* insertTest(int testNum){
    const int numSubs = 2;  //doesn't need to be a variable but its a handy place to keep it
    int subTest = 1;
    char feedback[300];
    
    sprintf(feedback, "Test %d: insertFront() and insertBack() test", testNum);
    testRec * rec = initRec(testNum, numSubs, feedback);
    
    runSubTest(testNum, subTest, rec, &insertTest1);
    subTest++;
    runSubTest(testNum, subTest, rec, &insertTest2);
    return rec;
}

/**************************** Delete ******************************/

/*--------
 subtest 1:  delete existing data test (front)
 ---------*/
SubTestRec deleteTest1(int testNum, int subTest){
    char feedback[300];
    SubTestRec result;
    List* test = threeList();
    
    deleteDataFromList(test, "kilroy");
    if(test->length == 2)
    {
        sprintf(feedback, "Subtest %d.%d: list length correctly updated on deletion from list.",testNum,subTest);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }
    else
    {
        sprintf(feedback, "Subtest %d.%d: list length incorrectly updated on deletion from list.",testNum,subTest);
        result = createSubResult(FAIL, feedback);
        return result;
    }
}

testRec* deleteTest(int testNum)
{
    const int numSubs = 1;  //doesn't need to be a variable but its a handy place to keep it
    int subTest = 1;
    char feedback[300];
    
    sprintf(feedback, "Test %d: insertSorted() test", testNum);
    testRec * rec = initRec(testNum, numSubs, feedback);
    
    runSubTest(testNum, subTest, rec, &deleteTest1);
    
    return rec;
}

/******************************************************************/

bool searchFunc(const void* el1, const void* el2){
    char* str1 = (char*)el1;
    char* str2 = (char*)el2;
    
    if (strcmp(str1, str2) == 0){
        return true;
    }else{
        return false;
    }
}

/********************* Find tests **********************/

/*--------
 subtest 1:  find in empty list
 ---------*/
SubTestRec searchTest1(int testNum, int subTest){
    char feedback[300];
    char searchStr[] = "sally";
    SubTestRec result;
    List* test = emptyList();
    
    void* data = findElement(*test, &searchFunc, searchStr);
    if(data == NULL)
    {
        sprintf(feedback, "Subtest %d.%d: searching empty list correctly returns NULL.",testNum,subTest);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }
    else
    {
        sprintf(feedback, "Subtest %d.%d: searching empty list does not return NULL",testNum,subTest);
        result = createSubResult(FAIL, feedback);
        return result;
    }
}

/*--------
 subtest 2:  find in list
 ---------*/
SubTestRec searchTest2(int testNum, int subTest){
    char feedback[300];
    char searchStr[] = "zappa";
    SubTestRec result;
    List* test = threeList();
    
    void* data = findElement(*test, &searchFunc, searchStr);
    if(strcmp((char*)data, "zappa") == 0)
    {
        sprintf(feedback, "Subtest %d.%d: correctly found an existing element (last) in the list.",testNum,subTest);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }
    else
    {
        sprintf(feedback, "Subtest %d.%d: failed to find an existing element in the list",testNum,subTest);
        result = createSubResult(FAIL, feedback);
        return result;
    }
}

/*--------
 subtest 3:  find in list
 ---------*/
SubTestRec searchTest3(int testNum, int subTest){
    char feedback[300];
    char searchStr[] = "kilroy";
    SubTestRec result;
    List* test = threeList();
    
    void* data = findElement(*test, &searchFunc, searchStr);
    if(strcmp((char*)data, "kilroy") == 0)
    {
        sprintf(feedback, "Subtest %d.%d: correctly found an existing element (first) in the list.",testNum,subTest);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }
    else
    {
        sprintf(feedback, "Subtest %d.%d: failed to find an existing element in the list",testNum,subTest);
        result = createSubResult(FAIL, feedback);
        return result;
    }
}

/*--------
 subtest 4:  find in list
 ---------*/
SubTestRec searchTest4(int testNum, int subTest){
    char feedback[300];
    char searchStr[] = "leeroy";
    SubTestRec result;
    List* test = threeList();
    
    void* data = findElement(*test, &searchFunc, searchStr);
    if(strcmp((char*)data, "leeroy") == 0)
    {
        sprintf(feedback, "Subtest %d.%d: correctly found an existing element in the list.",testNum,subTest);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }
    else
    {
        sprintf(feedback, "Subtest %d.%d: failed to find an existing element in the list",testNum,subTest);
        result = createSubResult(FAIL, feedback);
        return result;
    }
}

/*--------
 subtest 5:  find in list
 ---------*/
SubTestRec searchTest5(int testNum, int subTest){
    char feedback[300];
    char searchStr[] = "booger";
    SubTestRec result;
    List* test = threeList();
    
    void* data = findElement(*test, &searchFunc, searchStr);
    if(data == NULL)
    {
        sprintf(feedback, "Subtest %d.%d: correctly returned NULL after searching for non-existent element in the list.",testNum,subTest);
        result = createSubResult(SUCCESS, feedback);
        return result;
    }
    else
    {
        sprintf(feedback, "Subtest %d.%d: failed to return NULL after searching for non-existent element in the list.",testNum,subTest);
        result = createSubResult(FAIL, feedback);
        return result;
    }
}

testRec* searchTest(int testNum){
    const int numSubs = 5;  //doesn't need to be a variable but its a handy place to keep it
    int subTest = 1;
    char feedback[300];
    
    sprintf(feedback, "Test %d: insertFront() and insertBack() test", testNum);
    testRec * rec = initRec(testNum, numSubs, feedback);
    
    runSubTest(testNum, subTest, rec, &searchTest1);
    subTest++;
    runSubTest(testNum, subTest, rec, &searchTest2);
    subTest++;
    runSubTest(testNum, subTest, rec, &searchTest3);
    subTest++;
    runSubTest(testNum, subTest, rec, &searchTest4);
    subTest++;
    runSubTest(testNum, subTest, rec, &searchTest5);
    return rec;
}
