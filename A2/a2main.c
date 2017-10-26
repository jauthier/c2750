#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curses.h>
#include <unistd.h>

#include "BasicFunctions.h"

int mainMenu(int yMax, int xMax);
int makeCal(int yMax, int xMax);
Calendar * calInit(char * prodId,  char * version, char * uid, char * dt, char * action, char * trigger);
int saveCalendar(Calendar * cal);
int displayCal(Calendar * obj);
Calendar * readICalFIle(int yMax, int xMax);
void errScr(char * msg);
void readError(ICalErrorCode err);

int mainMenu(int yMax, int xMax){
	// create a menu window
	WINDOW * menuWin = newwin(yMax - 1, xMax - 1, 0, 0);
	refresh();
	wrefresh(menuWin);

	int choice;
	int highlight = 0;
	char ** menu = malloc(sizeof(char *)*5);
	menu[0] = malloc(sizeof(char)*23);
	strcpy(menu[0], "1. Read iCalendar File");
	menu[1] = malloc(sizeof(char)*16);
	strcpy(menu[1],"2. Display File");
	menu[2] = malloc(sizeof(char)*20);
	strcpy(menu[2],"3. Create iCalendar");
	menu[3] = malloc(sizeof(char)*35);
	strcpy(menu[3],"4. Save iCalendar Object to a FIle");
	menu[4] = malloc(sizeof(char)*7);
	strcpy(menu[4],"5. Exit");

	keypad(menuWin,TRUE);
	while (1){

		for (int i=0;i<5;i++){
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
				if (highlight+1 < 5)
					highlight++;
				break;
			default:
				break;
		}
		if (choice == 10)
			break;
	}
	for (int j=0;j<5;j++){
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

	Calendar * cal = calInit(prodId,version,uid,dt,action,trigger);
	delwin(readCalWin);
	if (cal != NULL){
		ICalErrorCode ec = validateCalendar(cal);
		if (ec == OK){
			saveCalendar(cal);
		}
	}
	return 1;
}

Calendar * calInit(char * prodId,  char * version, char * uid, char * dt, char * action, char * trigger){
	/* make the alarm */
	List proplist = initializeList(printProperty, deleteProperty, compareProperty);
	Alarm * alarm = initAlarm(action,trigger,proplist);
	free(action);
	free(trigger);

	/* make the event */
	List alarmlist = initializeList(printAlarm, deleteAlarm, compareAlarm);
	insertFront(&alarmlist,alarm);
	DateTime * DT = initDT(dt);
	free(dt);
	if (DT == NULL){
		clearList(&alarmlist);
		clearList(&proplist);
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
	List eventlist = initializeList(printAlarm, deleteAlarm, compareAlarm);
	insertFront(&eventlist,evt);
	float ver = atof(version);
	free(version);
	if (ver <= 0){
		clearList(&alarmlist);
		clearList(&proplist);
		clearList(&eventlist);
		free(prodId);
		errScr("Calendar was not created: Invalid Version");
		return NULL;
	}
	Calendar * cal = initCal(ver,prodId,eventlist,proplist);
	free(prodId);
	return cal;
}

int saveCalendar(Calendar * cal){
	/* make sure there is a calendar to save */
	if (obj == NULL){
		errScr("Calendar was not Saved: You have not entered a calendar object to save!")
		return 0;
	}

	int save = 0;
	int yMax, xMax;
	getmaxyx(stdscr, yMax, xMax);
	char * fileName;
	while (1){
		WINDOW * saveWin = newwin(yMax - 1, xMax - 1, 0, 0);
		refresh();
		wrefresh(saveWin);

		mvwprintw(saveWin,1,1,"Enter the name of the file:");
		// get user input
		wmove(saveWin, 2, 1);
		refresh();
		wrefresh(saveWin);
		char buffer[200];
		int i = 0;
		echo();
		int c = wgetch(saveWin);
		while(c != '\n'){
			buffer[i] = c;
			i++;
			c = wgetch(saveWin);
		}
		noecho();
		buffer[i] = '\0';
		// save the input
		fileName = malloc(sizeof(char)*(strlen(buffer)+1));
		strcpy(fileName, buffer);

		delwin(saveWin);
	
		//see if the file already exists
		if (access(fileName, F_OK) == 0){ // the file exists
			// ask if its okay to replace the file
			WINDOW * askWin = newwin(yMax - 1, xMax - 1, 0, 0);
			refresh();
			wrefresh(askWin);
			mvwprintw(askWin, 1,1,"The file you entered already exists, would you like to replace it?");
			int choice;
			int highlight = 0;
			char ** menu = malloc(sizeof(char *)*2);
			menu[0] = malloc(sizeof(char)*4);
			strcpy(menu[0], "Yes");
			menu[1] = malloc(sizeof(char)*3);
			strcpy(menu[1],"No");
			keypad(askWin,TRUE);
			while (1){
				for (int i=0;i<2;i++){
					if (highlight == i)
						wattron(askWin, A_REVERSE);
					mvwprintw(askWin,i+2,2,"%s",menu[i]);
					wattroff(askWin, A_REVERSE);
				}
				choice = wgetch(askWin);
				switch(choice){
					case KEY_UP:
						if (highlight-1 > -1)
							highlight --;
						break;
					case KEY_DOWN:
						if (highlight+1 < 2)
							highlight++;
						break;
					default:
						break;
				}
				if (choice == 10)
					break;
			}

			delwin(askWin);

			if (highlight == 0){
				save = 1;
				break;
			}

			// ask if user wants to enter a new file name
			WINDOW * againWin = newwin(yMax - 1, xMax - 1, 0, 0);
			refresh();
			wrefresh(againWin);
			mvwprintw(againWin, 1,1,"Would you like to try a new file name?");
			highlight = 0;
			
			while (1){
				for (int i=0;i<2;i++){
					if (highlight == i)
						wattron(againWin, A_REVERSE);
					mvwprintw(againWin,i+2,2,"%s",menu[i]);
					wattroff(againWin, A_REVERSE);
				}
				choice = wgetch(againWin);
				switch(choice){
					case KEY_UP:
						if (highlight-1 > -1)
							highlight --;
						break;
					case KEY_DOWN:
						if (highlight+1 < 2)
							highlight++;
						break;
					default:
						break;
				}
				if (choice == 10)
					break;
			}

			for (int j=0;j<2;j++){
				free(menu[j]);
			}
			free(menu);
			delwin(againWin);
			if (highlight == 1){
				break;
			}
		} else {
			save = 1;
			break;
		}
	}

	if (save = 1){
		ICalErrorCode ec = writeCalendar(fileName , obj);
		if (ec == OK)
			return 1;
	}
	return 0;
}

int displayCal(Calendar * obj){

	if ( obj == NULL){
		errScr("Cannot Display Calendar: You must read in a calendar file before it can be displayed!")
		return 0;
	}

	int yMax, xMax;
	getmaxyx(stdscr, yMax, xMax);

	char * toPrint = printCalendar(obj);
	if (toPrint == NULL){
		errScr("Cannot Display Calendar: There was an error reading the calendar.");
	} else {
		WINDOW * displayWin = newwin(yMax - 1, xMax - 1, 0, 1);
		refresh();
		wrefresh(displayWin);
		mvwprintw(displayWin, 1, 1, "%s",toPrint);
		delwin(displayWin);
	}
	return 0;
}

Calendar * readICalFIle(int yMax, int xMax){
	Calendar * obj;
	while (1){

		WINDOW * calFileWin = newwin(yMax - 1, xMax - 1, 0, 0);
		refresh();
		wrefresh(calFileWin);

		mvwprintw(calFileWin,1,1,"Enter the name of the file:");
		// get user input
		wmove(calFileWin, 2, 1);
		refresh();
		wrefresh(calFileWin);
		char buffer[200];
		int i = 0;
		echo();
		int c = wgetch(calFileWin);
		while(c != '\n'){
			buffer[i] = c;
			i++;
			c = wgetch(calFileWin);
		}
		noecho();
		buffer[i] = '\0';
		// save the input
		char * fileName = malloc(sizeof(char)*(strlen(buffer)+1));
		strcpy(fileName, buffer);
		delwin(calFileWin);

		ICalErrorCode ec = createCalendar(fileName, &obj);
		readError(ec);
		if (ec == INV_FILE){
			// ask if user wants to enter a new file name
			WINDOW * askWin = newwin(yMax - 1, xMax - 1, 0, 0);
			refresh();
			wrefresh(askWin);
			mvwprintw(askWin, 1,1,"Would you like to try a new file name?");
			int choice;
			int highlight = 0;
			char ** menu = malloc(sizeof(char *)*2);
			menu[0] = malloc(sizeof(char)*4);
			strcpy(menu[0], "Yes");
			menu[1] = malloc(sizeof(char)*3);
			strcpy(menu[1],"No");
			keypad(askWin,TRUE);
			while (1){
				for (int i=0;i<2;i++){
					if (highlight == i)
						wattron(askWin, A_REVERSE);
					mvwprintw(askWin,i+2,2,"%s",menu[i]);
					wattroff(askWin, A_REVERSE);
				}
				choice = wgetch(askWin);
				switch(choice){
					case KEY_UP:
						if (highlight-1 > -1)
							highlight --;
						break;
					case KEY_DOWN:
						if (highlight+1 < 2)
							highlight++;
						break;
					default:
						break;
				}
				if (choice == 10)
					break;
			}

			for (int j=0;j<2;j++){
				free(menu[j]);
			}
			free(menu);
			delwin(askWin);
			if (highlight == 1){
				return NULL;
			}

		} else {
			if (ec == OK)
				return obj;
			else 
				return NULL;
		}
	}
	return NULL;
}

void readError(ICalErrorCode err){
	if (err == INV_CAL)
        errScr("Calendar not Created: Invaid Calendar");
    else if (err == OK)
        errScr("The calendar was successfully created!");
    else if (err == INV_FILE)
        errScr("Calendar not Created: Invaid File");
    else if (err == INV_VER)
        errScr("Calendar not Created: Invaid Version");
    else if (err == INV_PRODID)
        errScr("Calendar not Created: Invaid Product ID");
    else if (err == INV_EVENT)
        errScr("Calendar not Created: Invaid Event");
    else if (err == DUP_VER)
        errScr("Calendar not Created: Duplicate Version");
    else if (err == DUP_PRODID)
        errScr("Calendar not Created: Duplicate Product ID");
    else if (err == INV_CREATEDT)
        errScr("Calendar not Created: Invaid Date-Time");
    else if (err == INV_ALARM)
        errScr("Calendar not Created: Invaid Alarm");
    else if (err == WRITE_ERROR)
        errScr("Calendar not Created: Write Error");
    else
        errScr("Calendar not Created: Other Error");
}

void errScr(char * msg){

	int yMax, xMax;
	getmaxyx(stdscr, yMax, xMax);
	WINDOW * errWin = newwin(yMax - 1, xMax - 1, 0, 0);
	refresh();
	wrefresh(errWin);
	mvwprintw(errWin, 2,2,"%s",msg);
	mvwprintw(errWin, 4,2,"Press any key to continue.");
	wrefresh(errWin);
	getch();
	delwin(errWin);
}

int main(int argc, char const *argv[]){

	//ncurses initalization
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);

	int yMax, xMax;
	getmaxyx(stdscr, yMax, xMax);
	Calendar * userMade = NULL;
	Calendar * fileMade = NULL;

	while (1){

		// start in main menu
		int choice = mainMenu(yMax, xMax);
		clear();
		refresh();

		if (choice == 0){
			// read file
			fileMade = readICalFIle(yMax, xMax);
		} else if (choice == 1){
			// Display cal using printCalendar
			displayCal(fileMade);
		} else if (choice == 2){
			// create cal
			userMade = makeCal(yMax, xMax);
		} else if (choice == 3){
			// save calendar
			saveCalendar(userMade);
		} else if (choice == 4){
			// exit
			endwin();
			return 0;
		}

	}

	getch();
	endwin();
	return 0;
}