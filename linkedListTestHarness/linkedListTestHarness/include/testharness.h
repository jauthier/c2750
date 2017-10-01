#include<string.h>
#ifndef _TEST_HARNESS_
#define _TEST_HARNESS_

#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define RESET "\x1B[0m"

#define SUCCESS 1
#define FAIL 0

typedef int testResult;

typedef struct testRecord
{
	int numSubs;
	int testNum;
	int subsPassed;

	char ** feedback;
	int feedbackLen;
}testRec;


testRec * initRec(int testNum, int numSubs, char * header);
void addResult( testRec * rec, testResult res, char * add);
void printRecord (testRec * rec);
int getScore(testRec * rec);

#endif

