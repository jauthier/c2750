FLAGS = -Wall -std=c11 -fPIC -shared

all: list

list: LinkedListAPI.c
	gcc $(FLAGS) LinkedListAPI.c -o liblist.so

cal:
	gcc $(FLAGS) *.c -o libcal.so
