FLAGS = -g -Wall -std=c11
FILES = calMain.c CalerndarParser.c LinkedList.c

all :
	gcc $(FILES) $(FLAGS) -o runCal

