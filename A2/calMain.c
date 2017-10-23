#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "LinkedListAPI.h"
#include "CalendarParser.h"


int main(int argc, char const *argv[]){
	

	char fileName[40] = "megaCal1.ics";
	Calendar ** obj = malloc(sizeof(Calendar *));
	ICalErrorCode ec = createCalendar(fileName, obj);
	char * errMes = printError(ec);
	printf("%s\n", errMes);
	free(errMes);

	if (ec == OK){
		ICalErrorCode err = validateCalendar(*obj);
		char * errMesg = printError(err);
		printf("%s\n", errMesg);
		free(errMesg);
		printf("Deleting\n");
		deleteCalendar(*obj);
	}


	free(obj);

	return 0;
}