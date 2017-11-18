FLAGS = -Wall -std=c11 -fPIC -shared -Iinclude

all: cal

cal:
	gcc $(FLAGS) src/*.c -o bin/libcal.so

clean:
	rm bin/libcal.so
