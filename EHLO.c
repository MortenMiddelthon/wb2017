#include <ncurses.h>
#include <string.h>

int main() {
	char mesg[] = "Elgbert ruler!";
	int row, col;

	initscr();
	getmaxyx(stdscr, row, col);
	mvprintw(row/2, (col-strlen(mesg))/2, "%s", mesg);
	mvprintw(row-2,0, "This screen has %d rows and %d cols\n", row, col);
	printw("Try resizing\n");
	refresh();
	getch();
	endwin();
	return 0;
}
