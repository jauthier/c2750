#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curses.h>

/*#include "CalendarParser.h"
#include "LinkedListAPI.h"
*/

int mainMenu(int yMax, int xMax){
	// create a menu window
	WINDOW * menuWin = newwin(1, xMax - 6, yMax - 4, 1);
	refresh();
	wrefresh(menuWin);

	int choice;
	int highlight = 0;
	char ** menu = malloc(sizeof(char *)*4);
	menu[0] = malloc(sizeof(char)*23);
	strcpy(menu[0], "1. Read iCalendar File");
	menu[1] = malloc(sizeof(char)*16);
	strcpy(menu[1],"2. Display File");
	menu[2] = malloc(sizeof(char)*20);
	strcpy(menu[2],"3. Create iCalendar");
	menu[3] = malloc(sizeof(char)*7);
	strcpy(menu[3],"4. Exit");

	while (1){

		for (int i=0;i<4;i++){
			if (highlight == i)
				wattron(menuWin, A_REVERSE);
			mvwprintw(i+1,2,"%s",menu[i]);
			wattroff(menuWin, A_REVERSE);
			
		}

		choice = wgetch(menuWin);

		switch(choice){
			case KEY_UP:
				if (highlight-1 > -1)
					highlight --;
				break;
			case KEY_DOWN:
				if (highlight+1 < 4)
					highlight++;
				break;
			default:
				break;
		}
		if (choice == 10)
			break;

	}

	for (int j=0;j<4;j++){
		free(menu[j]);
	}
	free(menu);
	delWin(menuWin);
	return highlight;
}

int readICalFIle(int yMax, int xMax){

	WINDOW * readCalWin = newwin(1, xMax - 6, yMax - 4, 1);
	refresh();
	wrefresh(readCalWin);

	//
	mvwprintw(2,2,"iCalendar Create");
	mvwprintw(4,2,"Product ID:");
	mvwprintw(6,2,"Version:");
	mvwprintw(8,2,"Event");
	mvwprintw(10,4,"UID:");
	mvwprintw(12,4,"Date-Time:");
	mvwprintw(14,4,"Alarm");
	mvwprintw(16,6,"Trigger:");
	mvwprintw(18,6,"Action:");

	/* ----- Product ID ----- */
	wmove(readCalWin, 4, 15);
	refresh();
	wrefresh(readCalWin);
	char buffer[200];
	int i = 0;
	echo();
	int c = wgetch(readCalWin);
	while(c != '\n'){
		buffer[i] = c;
		i++;
		c = wgetch(readCalWin);
	}
	noecho();
	buffer[i] = '\0';
	// save the input
	char * prodId = malloc(sizeof(char)*(strlen(buffer)+1));
	strcpy(prodId, buffer);

	/* ----- Version ----- */
	wmove(readCalWin, 6, 12);
	refresh();
	wrefresh(readCalWin);
	i = 0;
	echo();
	c = wgetch(readCalWin);
	while(c != '\n'){
		buffer[i] = c;
		i++;
		c = wgetch(readCalWin);
	}
	noecho();
	buffer[i] = '\0';
	// save the input
	char * version = malloc(sizeof(char)*(strlen(buffer)+1));
	strcpy(version, buffer);

	/* ----- UID ----- */
	wmove(readCalWin, 10, 12);
	refresh();
	wrefresh(readCalWin);
	i = 0;
	echo();
	c = wgetch(readCalWin);
	while(c != '\n'){
		buffer[i] = c;
		i++;
		c = wgetch(readCalWin);
	}
	noecho();
	buffer[i] = '\0';
	// save the input
	char * prodId = malloc(sizeof(char)*(strlen(buffer)+1));
	strcpy(prodId, buffer);

	delwin(readCalWin);
	return 0;
}

int displayCal(int yMax, int xMax){

	WINDOW * displayWin = newwin(1, xMax - 6, yMax - 4, 1);
	refresh();
	wrefresh(displayWin);

	delWin(displayWin);
	return 0;
}

int makeCal(int yMax, int xMax){

	WINDOW * makeCalWin = newwin(1, xMax - 6, yMax - 4, 1);
	refresh();
	wrefresh(makeCalWin);

	delWin(makeCalWin);
	return 0;
}

int errScr(int yMax, int xMax){

	WINDOW * errWin = newwin(1, xMax - 6, yMax - 4, 1);
	refresh();
	wrefresh(errWin);

	delWin(errWin);
	return 0;
}

int main(int argc, char const *argv[]){

	//ncurses initalization
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);

	int yMax, xMax;
	getmaxyx(stdscr, yMax, xMax);
	
	while (1){

		// start in main menu
		int choice = mainMenu(yMax, xMax);

		if (choice == 1){
			// read file
			readICalFIle(yMax, xMax);
		} else if (choice == 2){
			// Display cal using printCalendar
			displayCal(yMax, xMax);
		} else if (choice == 3){
			// create cal
			makeCal(yMax, xMax);
		} else if (choice == 4){
			// exit
			break;
		}
	}

	getch();
	endwin();
	return 0;
}
