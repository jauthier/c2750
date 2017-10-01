#include "testharness.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

testRec * initRec(int testNum, int numSubs, char * header)
{
	testRec * newRec = malloc(sizeof(testRec));
	newRec->numSubs = numSubs;
	newRec->feedback = malloc(sizeof(char *)*(numSubs+10)); //10 extra lines for output
	newRec->feedback[0] = malloc(sizeof(char)*(strlen(header)+1));
	strcpy(newRec->feedback[0], header);
	newRec->feedbackLen=1;
	newRec->testNum = testNum;
	newRec->subsPassed = 0;


	return newRec;
}

void addResult( testRec * rec, testResult res, char * add)
{

	rec->feedback[rec->feedbackLen] = malloc(sizeof(char)*(strlen(add)+20));

	if (res == SUCCESS)
	{
		strcpy(rec->feedback[rec->feedbackLen], GRN"SUCCESS: ");
	}
	else
	{
		strcpy(rec->feedback[rec->feedbackLen], RED"FAIL: ");
	}

	strcat(rec->feedback[rec->feedbackLen],add);
	strcat(rec->feedback[rec->feedbackLen],RESET);
	rec->feedbackLen++;
}


void printRecord (testRec * rec)
{
	int i;

	//printf("subs passed %d, subs total %d",rec->subsPassed, rec->numSubs);
	printf("\n");
	if(rec->subsPassed == rec->numSubs)
	{
		printf(GRN"%s: PASSED %d/%d tests\n"RESET,rec->feedback[0],rec->subsPassed, rec->numSubs); //header for test is always at position 0
	}
	else
	{
		printf(RED"%s: FAILED %d/%d tests\n"RESET,rec->feedback[0],rec->numSubs-rec->subsPassed, rec->numSubs); //header for test is always at position 0

	}
//printf("\n");
	for(i=1; i<rec->feedbackLen; i++)
	{
		printf("    %s\n", rec->feedback[i]);
	}

}

int getScore(testRec * rec)
{
	if(rec->subsPassed == rec->numSubs) return 1;
	else return 0;
}
