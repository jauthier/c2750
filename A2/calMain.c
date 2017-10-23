#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "LinkedList.h"
#include "CalendarParser.h"


int main(int argc, char const *argv[]){
	

	char fileName[40] = "megaCal1.ics";
	Calendar ** obj = malloc(sizeof(Calendar *));
	IcalErrorCode ec = createCalendar(fileName, obj);
	char * errMes = printError(ec);
	printf("%s\n", errMes);
	free(errMes);

	if (ec == OK){
		IcalErrorCode err = validateCalendar(*obj);
		char * errMesg = printError(ec);
		printf("%s\n", errMesg);
		free(errMesg);
		deleteCalendar(*obj);
	}


	free(obj);

	return 0;
}