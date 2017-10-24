#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curses.h>

/*#include "CalendarParser.h"
#include "LinkedListAPI.h"
*/


int main(int argc, char const *argv[]){

	//ncurses initalization
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);

	mvprintw(4,2,"1. Read iCalendar File");
	mvprintw(6, 2, "2. Display File");
	mvprintw(8, 2, "3. Create iCalendar");
	mvprintw(10, 2, "4. Exit");
	

	getch();
	endwin();
	return 0;
}
