#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct student{
	char * lastName;
	char * firstName;
	int id;
}Student;

void createStudent(char * last, char * first, int id){
	Student * newStudent = malloc(sizeof(Student));
	newStudent->lastName = malloc(sizeof(char)*(strlen(last)+1));
	strcpy(newStudent->lastName, last);
	newStudent->firstName = malloc(sizeof(char)*(strlen(first)+1));
	strcpy(newStudent->firstName, first);
	newStudent->id = id;
	return newStudent;
}

void deleteStudent(void *toBeDeleted){
	Student * s = (Student *)toBeDeleted;
	free(s->last);
	free(s->first);
	free(s);
}

int compareStudent(void *first, void *second){
	Student * s1 = (Student *)first;
	Student * s2 = (Student *)second;

	int c1 = strcmp(s1->lastName,s2->lastName);
	if (c1 == 0){
		int c2 = strcmp(s1->firstName,s2->firstName)
		if (c2 == 0){
			if (s1->id == s2->id)
				return 0;
			 else if (s1->id < s2->id)
				return -1;
			 else 
				return 1;
		}
		return c2;
	}
	return c1;
}

char * printStudent(void *toBePrinted){
	Student * s = (Student *)toBePrinted;
	int len = strlen(s->lastName) + strlen(s->firstName) + 20;
	char * str = malloc(sizeof(char)*len);
	sprintf(str, "%s, %s\nID:%d\n", s->lastName, s->firstName, s->id);
	return str;
}

int main(int argc, char const *argv[]){
	
	/* create students */
	Student * s1 = createStudent(Authier, Jessica, 12345);
	Student * s2 = createStudent(Brown, Amber, 43172);
	Student * s3 = createStudent(Carls, James, 83672);
	Student * s4 = createStudent(Zenz, Luke, 38923);

	/* create a list */
	List list = initalizeList(printStudent, deleteStudent, compareStudent);

	/* print empty list */
	char * toPrint1 = toString(list);
	if (toPrint1 == NULL){
		printf("Print empty list: SUCCESS\n");
	} else {
		printf("Print empty list: FAIL\n");
	}

	/* print null list */
	List nullList;
	char * toPrint2 = toString(nullList);
	if (toPrint1 == NULL){
		printf("Print null list: SUCCESS\n");
	} else {
		printf("Print null list: FAIL\n");
	}

	/* add to front of empty */
	insertFront(list, s1);
	int len = getLength(list);
	if (len == 1){
		printf("Add front and length: SUCCESS\n");
	} else {
		printf("Add front and length: FAIL\n");
	}


	return 0;
}