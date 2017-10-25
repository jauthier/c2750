#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curses.h>

#include "CalendarParser.h"

int mainMenu(int yMax, int xMax){
	// create a menu window
	WINDOW * menuWin = newwin(yMax - 1, xMax - 1, 0, 0);
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

	keypad(menuWin,TRUE);
	while (1){

		for (int i=0;i<4;i++){
			if (highlight == i)
				wattron(menuWin, A_REVERSE);
			mvwprintw(menuWin,i+2,2,"%s",menu[i]);
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
	delwin(menuWin);
	return highlight;
}

int makeCal(int yMax, int xMax){

	WINDOW * readCalWin = newwin(yMax - 1, xMax - 1, 0, 0);
	refresh();
	wrefresh(readCalWin);
	mvwprintw(readCalWin,2,2,"iCalendar Create");
	mvwprintw(readCalWin, 4,2,"Product ID:");
	mvwprintw(readCalWin, 6,2,"Version:");
	mvwprintw(readCalWin, 8,2,"Event");
	mvwprintw(readCalWin, 10,4,"UID:");
	mvwprintw(readCalWin, 12,4,"Date-Time:");
	mvwprintw(readCalWin,14,4,"Alarm");
	mvwprintw(readCalWin,16,6,"Trigger:");
	mvwprintw(readCalWin,18,6,"Action:");

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
	wmove(readCalWin, 10, 10);
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
	char * uid = malloc(sizeof(char)*(strlen(buffer)+1));
	strcpy(uid, buffer);

	/* ----- DTSTAMP ----- */
	wmove(readCalWin, 12, 16);
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
	char * dt = malloc(sizeof(char)*(strlen(buffer)+1));
	strcpy(dt, buffer);

	/* ----- Trigger ----- */
	wmove(readCalWin, 16, 16);
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
	char * trigger = malloc(sizeof(char)*(strlen(buffer)+1));
	strcpy(trigger, buffer);

	/* ----- Action ----- */
	wmove(readCalWin, 18, 15);
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
	char * action = malloc(sizeof(char)*(strlen(buffer)+1));
	strcpy(action, buffer);

	/* ----- Make iCal ----- */
	/* check for empty fields */
	if ((strlen(prodId)==0)||(strlen(version)==0)||(strlen(uid)==0)||
		(strlen(dt)==0)||(strlen(trigger)==0)||(strlen(action)==0)){

		delwin(readCalWin);
		return 0;		
	}

	int checkCal = calInit(prodId,version,uid,dt,action,trigger);

	delwin(readCalWin);
	return 1;
}

Calendar * calInit(char * prodId,  char * version, char * uid, char * dt, char * action, char * trigger){
	/* make the alarm */
	List * proplist = malloc(sizeof(List));
	*proplist = initializeList(printProperty, deleteProperty, compareProperty);
	Alarm * alarm = initAlarm(action,trigger,proplist);
	free(action);
	free(trigger);

	/* make the event */
	List * alarmlist = malloc(sizeof(List));
	*alarmlist = initializeList(printAlarm, deleteAlarm, compareAlarm);
	insertFront(alarmlist,alarm);
	DateTime * DT = initDT(dt);
	free(dt);
	if (DT == NULL){
		clearList(alarmlist);
		clearList(proplist);
		deleteDT(DT);
		free(uid);
		free(prodId);
		free(version);
		errScr("Calendar was not created: Invalid DateTime Format");
		return NULL;
	}
	Event * evt = initEvent(uid, DT,proplist,alarmlist);
	free(uid);

	/* make the calendar */
	List * eventlist = malloc(sizeof(List));
	*eventlist = initializeList(printAlarm, deleteAlarm, compareAlarm);
	insertFront(eventlist,evt);
	float ver = atof(version);
	free(version);
	if (ver <= 0){
		clearList(alarmlist);
		clearList(proplist);
		clearList(eventlist);
		free(prodId);
		errScr("Calendar was not created: Invalid Version");
		return NULL;
	}
	Calendar * cal = initCal(ver,prodId,eventlist,proplist);
	free(prodId);
	return cal;
}

int displayCal(int yMax, int xMax){

	WINDOW * displayWin = newwin(yMax - 1, xMax - 1, 0, 0);
	refresh();
	wrefresh(displayWin);

	delwin(displayWin);
	return 0;
}

int readICalFIle(int yMax, int xMax){

	WINDOW * makeCalWin = newwin(yMax - 1, xMax - 1, 0, 0);
	refresh();
	wrefresh(makeCalWin);

	delwin(makeCalWin);
	return 0;
}

int errScr(char * msg){

	int yMax, xMax;
	getmaxyx(stdscr, yMax, xMax);
	WINDOW * errWin = newwin(yMax - 1, xMax - 1, 0, 0);
	refresh();
	wrefresh(errWin);
	mvwprintw(errWin, 2,2,"%s",msg);
	delwin(errWin);
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
		clear();
		refresh();

		if (choice == 0){
			// read file
			readICalFIle(yMax, xMax);
		} else if (choice == 1){
			// Display cal using printCalendar
			displayCal(yMax, xMax);
		} else if (choice == 2){
			// create cal
			makeCal(yMax, xMax);
		} else if (choice == 3){
			// exit
			endwin();
			return 0;
		}
	}

	getch();
	endwin();
	return 0;
}