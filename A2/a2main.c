#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curses.h>

/*#include "CalendarParser.h"
#include "LinkedListAPI.h"
*/

void mainScr(WINDOW * win){

	wmove(win, 4, 4);
	waddstr(win, "1. Read iCalendar File");
	wrefresh(win);
	
	wmove(win, 6, );
	waddstr(win, "2. Display File");
	wrefresh(win);

	wmove(win, 8, 4);
	waddstr(win, "3. Create iCalendar");
	wrefresh(win);

	wmove(win, 10, 4);
	waddstr(win, "4. Exit");
	wrefresh(win);
}



int main(int argc, char const *argv[]){

	//ncurses initalization
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);

	// create a new window
	int nlines = COLS - 3;
	int ncols = LINES - 3;
	int y0 = 1;
	int x0 = 1;
	WINDOW * win = newwin(nlines, ncols, y0, x0);
	mainScr(win);

	delwin(win);
	return 0;
}
