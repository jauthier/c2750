#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curses.h>

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
			mvwprintw(menuWin,i+1,2,"%s",menu[i]);
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

int main(int argc, char const *argv[]){

	//ncurses initalization
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);

	int yMax, xMax;
	getmaxyx(stdscr, yMax, xMax);
	

	int choice = mainMenu(yMax, xMax);


	getch();
	endwin();
	return 0;
}