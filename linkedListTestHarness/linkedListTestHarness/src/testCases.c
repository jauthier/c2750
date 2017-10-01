#include "LinkedListAPI.h"
#include "testharness.h"
#include "testcases.h"
#include <string.h>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>


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
    
    return list;
}

/*---------------------------------
 List creation
 --------------------------------*/
testRec * initializeListTest(int testNum)
{
    const int numSubs = 2;  //doesn't need to be a variable but its a handy place to keep it
    int subTest = 1;
    char feedback[300];
    List test;
    // Node * n;
    
    sprintf(feedback, "Test %d: List Initialization Test", testNum);
    testRec * rec = initRec(testNum, numSubs, feedback);
    
    /*--------
     subtest 1: initial values of List Variables
     ---------*/
    test = initializeList(testPrint, testDestroy, testCompare);
    
    // reusing test variable from previous subtest
    if(test.head == NULL)
    {
        if(test.tail == NULL)
        {
            sprintf(feedback, "Subtest %d.%d: head and tail both NULL",testNum,subTest);
            addResult(rec, SUCCESS, feedback);
            rec->subsPassed++;
            
        }
        else
        {
            sprintf(feedback, "Subtest %d.%d: List tail is not NULL  on initialization.",testNum,subTest);
            addResult(rec, FAIL, feedback);
        }
    }
    else if (test.tail != NULL)
    {
        sprintf(feedback, "Subtest %d.%d: List head and tail are not NULL on initialization.",testNum,subTest);
        addResult(rec, FAIL, feedback);
    }
    else
    {
        sprintf(feedback, "Subtest %d.%d: List head is not NULL on initialization.",testNum,subTest);
        addResult(rec, FAIL, feedback);
    }
    
    /*--------
     subtest 2: function pointer initialization
     ---------*/
    subTest++;
    if(test.deleteData != testDestroy  || test.compare !=testCompare || test.printData !=testPrint)
    {
        sprintf(feedback, "Subtest %d.%d: At least one function pointer is incorrectly assigned.",testNum,subTest);
        addResult(rec, FAIL, feedback);
    }
    else
    {
        sprintf(feedback, "Subtest %d.%d: Function pointers are correctly assigned.",testNum,subTest);
        addResult(rec, SUCCESS, feedback);
        rec->subsPassed++;
    }
    
    
    return rec;
}

testRec * initializeNodeTest(int testNum)
{
    const int numSubs = 2;  //doesn't need to be a variable but its a handy place to keep it
    int subTest = 1;
    char feedback[300];
    //List * test = NULL;
    Node * n;
    
    sprintf(feedback, "Test %d: Node Initialization Test", testNum);
    testRec * rec = initRec(testNum, numSubs, feedback);
    
    
    /*--------
     subtest 1: initializeNode
     ---------*/
    char * data = createData("test1");
    n = initializeNode(data);
    if(n == NULL)
    {
        sprintf(feedback, "Subtest %d.%d: initializeNode() returned null.  Cannot continue node creation tests.",testNum,subTest);
        addResult(rec, FAIL, feedback);
        return rec;
    }
    else
    {
        sprintf(feedback, "Subtest %d.%d: initializeNode() was not null.",testNum,subTest);
        addResult(rec, SUCCESS, feedback);
        rec->subsPassed++;
    }
    /*--------
     subtest 2: check data
     ---------*/
    subTest++;
    if(n->data != NULL)
    {
        if(testCompare((char *)(n->data),data)==0)
        {
            sprintf(feedback, "Subtest %d.%d: Node has correct data.",testNum,subTest);
            addResult(rec, SUCCESS, feedback);
            rec->subsPassed++;
        }
        else
        {
            sprintf(feedback, "Subtest %d.%d: Node data does not match test data",testNum,subTest);
            addResult(rec, FAIL, feedback);
        }
    }
    else
    {
        sprintf(feedback, "Subtest %d.%d: Node data is NULL",testNum,subTest);
        addResult(rec, FAIL, feedback);
    }
    
    return rec;
}

/*********************
 Insert Front and Back
 **********************/

testRec * insertFrontAndBackTest(int testNum)
{
    const int numSubs = 6;  //doesn't need to be a variable but its a handy place to keep it
    int subTest = 1;
    char feedback[300];
    List* test = emptyList();
    
    sprintf(feedback, "Test %d: insertFront() and insertBack() test", testNum);
    testRec * rec = initRec(testNum, numSubs, feedback);
    
    /*--------
     subtest 1:  insert Front empty list
     ---------*/
    //char * data = "bob";
    char * data = createData("sally");
    insertFront(test, data);
    if(test->head->data != NULL &&
       testCompare(test->head->data,data) == 0 &&
       testCompare(test->tail->data,data) == 0 &&
       test->head == test->tail &&
       test->head->previous == NULL &&
       test->head->next == NULL)
    {
        sprintf(feedback, "Subtest %d.%d: correctly inserted front on empty list.",testNum,subTest);
        addResult(rec, SUCCESS, feedback);
        rec->subsPassed++;
    }
    else
    {
        sprintf(feedback, "Subtest %d.%d: Data not correctly inserted at front of list",testNum,subTest);
        addResult(rec, FAIL, feedback);
    }
    
    
    /*--------
     subtest 2:  insert Front existing List
     ---------*/
    subTest++;
    test = twoList();
    Node* oldHead = test->head;
    data = createData("sally");
    insertFront(test, data);
    if(test->head->data != NULL && testCompare(test->head->data,data)==0)
    {
        if(testCompare(test->head->next->data,"kilroy")==0 &&
           testCompare(oldHead->previous->data, data) == 0)
        {
            sprintf(feedback, "Subtest %d.%d: correctly inserted front on populated list.",testNum,subTest);
            addResult(rec, SUCCESS, feedback);
            rec->subsPassed++;
        }
        else
        {
            sprintf(feedback, "Subtest %d.%d: List order destroyed on insert",testNum,subTest);
            addResult(rec, FAIL, feedback);
        }
    }
    else
    {
        sprintf(feedback, "Subtest %d.%d: Data not correctly inserted at front of populated list",testNum,subTest);
        addResult(rec, FAIL, feedback);
    }
    
    
    
    /*--------
     subtest 3:  insert Back empty list
     ---------*/
    subTest++;
    data = createData("sally");
    insertBack(test, data);
    if(test->head->data!=NULL && testCompare(test->tail->data,data)==0)
    {
        sprintf(feedback, "Subtest %d.%d: correctly inserted back on empty list.",testNum,subTest);
        addResult(rec, SUCCESS, feedback);
        rec->subsPassed++;
    }
    else
    {
        sprintf(feedback, "Subtest %d.%d: Data not correctly inserted at back of list",testNum,subTest);
        addResult(rec, FAIL, feedback);
    }
    
    
    /*--------
     subtest 4:  insert Back existing List
     ---------*/
    subTest++;
    test = twoList();
    Node* oldTail = test->tail;
    data = createData("sally");
    insertBack(test, data);
    if(test->tail->data!=NULL && testCompare(test->tail->data,data) == 0)
    {
        if(testCompare(test->tail->previous->data,"zappa") == 0 &&
           testCompare(oldTail->next->data, data) == 0)
        {
            sprintf(feedback, "Subtest %d.%d: correctly inserted back on populated list.",testNum,subTest);
            addResult(rec, SUCCESS, feedback);
            rec->subsPassed++;
        }
        else
        {
            sprintf(feedback, "Subtest %d.%d: List order destroyed on insert back",testNum,subTest);
            addResult(rec, FAIL, feedback);
        }
    }
    else
    {
        sprintf(feedback, "Subtest %d.%d: Data not correctly inserted at back of populated list",testNum,subTest);
        addResult(rec, FAIL, feedback);
    }
    
    
    /*--------
     subtest 5:  insert Back NULL list
     ---------*/
    subTest++;
    test = NULL;
    data = createData("sally");
    insertBack(test, data);
    //if it doesn't segfault
    sprintf(feedback, "Subtest %d.%d: insertBack handled NULL list.",testNum,subTest);
    addResult(rec, SUCCESS, feedback);
    rec->subsPassed++;
    
    /*--------
     subtest 6:  insert Front NULL list
     ---------*/
    subTest++;
    test = NULL;
    data = createData("sally");
    insertFront(test, data);
    //if it doesn't segfault
    sprintf(feedback, "Subtest %d.%d: insertFront handled NULL list.",testNum,subTest);
    addResult(rec, SUCCESS, feedback);
    rec->subsPassed++;
    
    return rec;
    
}

/****************
 getFront and getBack
 *****************/

testRec * getFrontAndBackTest(int testNum)
{
    const int numSubs = 4;  //doesn't need to be a variable but its a handy place to keep it
    int subTest = 1;
    char feedback[300];
    List * test = twoList();
    
    sprintf(feedback, "Test %d: getFront() and getBack() test", testNum);
    testRec * rec = initRec(testNum, numSubs, feedback);
    
    /*--------
     subtest 1:  getFront populated list
     ---------*/
    char * retrieved = (char *)getFromFront(*test);
    if(testCompare(retrieved, "kilroy")==0)
    {
        sprintf(feedback, "Subtest %d.%d: correctly retrieved data from populated list.",testNum,subTest);
        addResult(rec, SUCCESS, feedback);
        rec->subsPassed++;
    }
    else
    {
        sprintf(feedback, "Subtest %d.%d: Data not correctly retrieve from populated list",testNum,subTest);
        addResult(rec, FAIL, feedback);
    }
    
    
    /*--------
     subtest 2:  getBack populated list
     ---------*/
    subTest++;
    retrieved = (char *)getFromBack(*test);
    if(testCompare(retrieved, "zappa")==0)
    {
        sprintf(feedback, "Subtest %d.%d: correctly retrieved data from populated list.",testNum,subTest);
        addResult(rec, SUCCESS, feedback);
        rec->subsPassed++;
    }
    else
    {
        sprintf(feedback, "Subtest %d.%d: Data not correctly retrieve from populated list",testNum,subTest);
        addResult(rec, FAIL, feedback);
    }
    
    
    /*--------
     subtest 3:  getFront empty
     ---------*/
    test = emptyList();
    
    retrieved = (char *)getFromFront(*test);
    if(retrieved == NULL)
    {
        sprintf(feedback, "Subtest %d.%d: getFront correctly handled empty list.",testNum,subTest);
        addResult(rec, SUCCESS, feedback);
        rec->subsPassed++;
    }
    else
    {
        sprintf(feedback, "Subtest %d.%d: getFront did not handle empty list correctly",testNum,subTest);
        addResult(rec, FAIL, feedback);
    }
    
    
    /*--------
     subtest 4:  getBack emptylist
     ---------*/
    //reusing empty test list from previous subtest
    subTest++;
    char * ret = (char *)getFromBack(*test);
    if(ret == NULL)
    {
        sprintf(feedback, "Subtest %d.%d: getBack correctly handled empty list.",testNum,subTest);
        addResult(rec, SUCCESS, feedback);
        rec->subsPassed++;
    }
    else
    {
        sprintf(feedback, "Subtest %d.%d: getBack did not handle empty list correctly",testNum,subTest);
        addResult(rec, FAIL, feedback);
    }
    
    return rec;
}


/****************
 insert Sorted
 *****************/


testRec * insertSortedTest(int testNum)
{
    const int numSubs = 5;  //doesn't need to be a variable but its a handy place to keep it
    int subTest = 1;
    char feedback[300];
    List * test = twoList();
    
    sprintf(feedback, "Test %d: insertSorted() test", testNum);
    testRec * rec = initRec(testNum, numSubs, feedback);
    
    /*--------
     subtest 1:  insert to middle of sorted list.
     ---------*/
    char * data = createData("ming");
    insertSorted(test, data);
    
    if(testCompare(test->head->next->data, data) == 0)
    {
        if (testCompare(test->tail->previous->data, data) == 0 &&
            test->head->next->previous == test->head && test->tail->previous->next == test->tail){
            sprintf(feedback, "Subtest %d.%d: inserted to middle of sorted list.",testNum,subTest);
            addResult(rec, SUCCESS, feedback);
            rec->subsPassed++;
        }else{
            sprintf(feedback, "Subtest %d.%d: List order destroyed on insert",testNum,subTest);
            addResult(rec, FAIL, feedback);
        }
    }
    else
    {
        sprintf(feedback, "Subtest %d.%d: Did not insert sorted to middle of list",testNum,subTest);
        addResult(rec, FAIL, feedback);
    }
    
    
    /*--------
     subtest 2:  insert to back of sorted list
     ---------*/
    subTest++;
    test = twoList();
    data = createData("zyrg");
    
    Node* oldTail = test->tail;
    
    insertSorted(test, data);
    
    if(testCompare(test->tail->data, data) == 0)
    {
        if (testCompare(oldTail->next->data, data) == 0 &&
            testCompare(test->tail->previous->data, oldTail->data) == 0 &&
            test->tail->next == NULL){
            sprintf(feedback, "Subtest %d.%d: inserted to back of sorted list.",testNum,subTest);
            addResult(rec, SUCCESS, feedback);
            rec->subsPassed++;
        }else{
            sprintf(feedback, "Subtest %d.%d: List order destroyed on insert",testNum,subTest);
            addResult(rec, FAIL, feedback);
        }
        
    }
    else
    {
        sprintf(feedback, "Subtest %d.%d: Did not insert sorted to back of list",testNum,subTest);
        addResult(rec, FAIL, feedback);
    }
    
    
    /*--------
     subtest 3:  insert to front of sorted list
     ---------*/
    subTest++;
    test = twoList();
    Node* oldHead = test->head;
    data = createData("abbott");
    insertSorted(test, data);
    
    if(testCompare((char*)(test->head->data), data) == 0)
    {
        if (testCompare(oldHead->previous->data, data) == 0 &&
            testCompare(test->head->next->data, oldHead->data) == 0 &&
            test->head->previous == NULL){
            //printf("%s %s\n", (char*)data, (char*)oldHead->previous->data);
            sprintf(feedback, "Subtest %d.%d: inserted to front of sorted list.",testNum,subTest);
            addResult(rec, SUCCESS, feedback);
            rec->subsPassed++;
        }else{
            sprintf(feedback, "Subtest %d.%d: List order destroyed on insert",testNum,subTest);
            addResult(rec, FAIL, feedback);
        }
    }
    else
    {
        sprintf(feedback, "Subtest %d.%d: Did not insert sorted to front of list",testNum,subTest);
        addResult(rec, FAIL, feedback);
    }
    
    
    /*--------
     subtest 4:  insert sorted with empty list
     ---------*/
    subTest++;
    test = emptyList();
    data = createData("abbott");
    insertSorted(test, data);
    
    if(testCompare(test->head->data, data) == 0 &&
       testCompare(test->tail->data, data) == 0 &&
       test->head->previous == NULL && test->tail->next == NULL &&
       test->head == test->tail
       )
    {
        sprintf(feedback, "Subtest %d.%d: inserted single element to sorted list.",testNum,subTest);
        addResult(rec, SUCCESS, feedback);
        rec->subsPassed++;
    }
    else
    {
        sprintf(feedback, "Subtest %d.%d: insert sorted failed on empty list",testNum,subTest);
        addResult(rec, FAIL, feedback);
    }
    
    
    
    /*--------
     subtest 5:  insertSorted NULL list
     ---------*/
    subTest++;
    test = NULL;
    data = createData("sally");
    insertSorted(test, data);
    //if it doesn't segfault
    sprintf(feedback, "Subtest %d.%d: insertSorted handled NULL list.",testNum,subTest);
    addResult(rec, SUCCESS, feedback);
    rec->subsPassed++;
    
    return rec;
}


/****************
 Delete
 *****************/


testRec * deleteTest(int testNum)
{
    const int numSubs = 8;  //doesn't need to be a variable but its a handy place to keep it
    int subTest = 1;
    char feedback[300];
    List * test = threeList();
    
    sprintf(feedback, "Test %d: delete data and list test", testNum);
    testRec * rec = initRec(testNum, numSubs, feedback);
    
    /*--------
     subtest 1:  delete existing data test (front)
     ---------*/
    char * retrieved = (char *)deleteDataFromList(test, "kilroy");
    if(testCompare(retrieved, "kilroy")==0)
    {
        if(testCompare(test->head->data, "leeroy") == 0 &&
           test->head->previous == NULL)
        {
            sprintf(feedback, "Subtest %d.%d: correctly retrieved data from the front of a populated list.",testNum,subTest);
            addResult(rec, SUCCESS, feedback);
            rec->subsPassed++;
        }
        else
        {
            sprintf(feedback, "Subtest %d.%d: did not remove element from list",testNum,subTest);
            addResult(rec, FAIL, feedback);
        }
    }
    else
    {
        sprintf(feedback, "Subtest %d.%d: Data not returned after being removed from list",testNum,subTest);
        addResult(rec, FAIL, feedback);
    }
    
    
    /*--------
     subtest 2:  delete existing data test (back)
     ---------*/
    subTest++;
    test = threeList();
    retrieved = (char *)deleteDataFromList(test, "zappa");
    if(testCompare(retrieved, "zappa")==0)
    {
        if(testCompare(test->tail->data, "leeroy") == 0 &&
           test->tail->next == NULL)
        {
            sprintf(feedback, "Subtest %d.%d: correctly retrieved data from the back of a populated list.",testNum,subTest);
            addResult(rec, SUCCESS, feedback);
            rec->subsPassed++;
        }
        else
        {
            sprintf(feedback, "Subtest %d.%d: did not remove element from list",testNum,subTest);
            addResult(rec, FAIL, feedback);
        }
    }
    else
    {
        sprintf(feedback, "Subtest %d.%d: Data not returned after being removed from list",testNum,subTest);
        addResult(rec, FAIL, feedback);
    }


    /*--------
     subtest 3:  delete existing data test (middle)
     ---------*/
    subTest++;
    test = threeList();
    retrieved = (char *)deleteDataFromList(test, "leeroy");
    if(testCompare(retrieved, "leeroy")==0)
    {
        if(testCompare(test->tail->data, "zappa") == 0 &&
           testCompare(test->head->data, "kilroy") == 0 &&
           testCompare(test->head->next->data,"zappa") == 0 &&
           testCompare(test->tail->previous->data, "kilroy") == 0)
        {
            sprintf(feedback, "Subtest %d.%d: correctly retrieved data from the middle of a populated list.",testNum,subTest);
            addResult(rec, SUCCESS, feedback);
            rec->subsPassed++;
        }
        else
        {
            sprintf(feedback, "Subtest %d.%d: did not remove element from list",testNum,subTest);
            addResult(rec, FAIL, feedback);
        }
    }
    else
    {
        sprintf(feedback, "Subtest %d.%d: Data not returned after being removed from list",testNum,subTest);
        addResult(rec, FAIL, feedback);
    }
    
    /*--------
     subtest 4:  delete data that doesn't exist
     ---------*/
    subTest++;
    test = twoList();
    retrieved = (char *)deleteDataFromList(test, "beth");
    if(retrieved == NULL)
    {
        sprintf(feedback, "Subtest %d.%d: correctly handled invalid data.",testNum,subTest);
        addResult(rec, SUCCESS, feedback);
        rec->subsPassed++;
    }
    else
    {
        sprintf(feedback, "Subtest %d.%d: failed on invalid data",testNum,subTest);
        addResult(rec, FAIL, feedback);
    }

    
    /*--------
     subtest 5:  delete null data
     ---------*/
    subTest++;
    test = twoList();
    retrieved = (char *)deleteDataFromList(test, NULL);
    //if did not seg fault
    sprintf(feedback, "Subtest %d.%d: correctly handled NULL parameter.",testNum,subTest);
    addResult(rec, SUCCESS, feedback);
    rec->subsPassed++;

    /*--------
    subtest 6:  delete populated list
    ---------*/
       subTest++;
       test = twoList();
       clearList(test);
       if(test->head == NULL && test->tail == NULL)
       {
            sprintf(feedback, "Subtest %d.%d: clearList functioned correctly.",testNum,subTest);
            addResult(rec, SUCCESS, feedback);
            rec->subsPassed++;
        }
        else
        {
          sprintf(feedback, "Subtest %d.%d: did not correctly clear List",testNum,subTest);
          addResult(rec, FAIL, feedback);
        }
    
    /*--------
    subtest 7:  delete empty list
    ---------*/
       subTest++;
       test = emptyList();
       clearList(test);
       if(test->head == NULL && test->tail == NULL)
       {
            sprintf(feedback, "Subtest %d.%d: correctly handled empty list.",testNum,subTest);
            addResult(rec, SUCCESS, feedback);
            rec->subsPassed++;
        }
        else
        {
          sprintf(feedback, "Subtest %d.%d: failed on empty list",testNum,subTest);
          addResult(rec, FAIL, feedback);
        }
    
    /*--------
    subtest 8:  delete NULL list
    ---------*/
       subTest++;
       test = NULL;
       clearList(test);
       if(test == NULL)
       {
            sprintf(feedback, "Subtest %d.%d: correctly handled NULL list.",testNum,subTest);
            addResult(rec, SUCCESS, feedback);
            rec->subsPassed++;
        }
        else
        {
          sprintf(feedback, "Subtest %d.%d: failed on NULL list",testNum,subTest);
          addResult(rec, FAIL, feedback);
        }
    return rec;
}





/****************
Print
*****************/

testRec * printTest(int testNum)
{
   const int numSubs = 2;  //doesn't need to be a variable but its a handy place to keep it
   int subTest = 1;
   char feedback[300];
   char * one = NULL;
    char * two = NULL;
       char * printed;
   List * test = twoList();

    sprintf(feedback, "Test %d: print test", testNum);
    testRec * rec = initRec(testNum, numSubs, feedback);

/*--------
subtest 1:  print forwards
---------*/
   printed =  toString(*test);
   one = strstr(printed, "k");
   two = strstr(printed, "z");
   if(one !=NULL && two != NULL)
   {
      if((int)(one-printed) < (int)(two-printed))
      {
        sprintf(feedback, "Subtest %d.%d: List printed forwards in correct order.",testNum,subTest);
        addResult(rec, SUCCESS, feedback);
        rec->subsPassed++;
      }
      else
      {
         sprintf(feedback, "Subtest %d.%d: List did not print in correct order.",testNum,subTest);
         addResult(rec, FAIL, feedback);
      }

   }
   else
   {
    sprintf(feedback, "Subtest %d.%d: List did not print correctly",testNum,subTest);
    addResult(rec, FAIL, feedback);

   }
  free(printed);


/*--------
subtest 2:  print empty List
---------*/
   subTest++;
   test = emptyList();
   printed = feedback; //to get a value other than NULL
   printed =  toString(*test);
   if(printed == NULL || printed != NULL )
      {
        sprintf(feedback, "Subtest %d.%d: Empty list handled correctly.",testNum,subTest);
        addResult(rec, SUCCESS, feedback);
        rec->subsPassed++;
      }
      else
      {
         sprintf(feedback, "Subtest %d.%d: List did not handle empty list correctly",testNum,subTest);
         addResult(rec, FAIL, feedback);
      }

return rec;
}

