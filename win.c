#include <stdio.h>
#include <ncurses.h>
#include <string.h>
#include <json-c/json.h>

WINDOW *create_newwin(int height, int width, int starty, int startx);
void print_in_window(WINDOW *win, int starty, int startx, int width, char *string);
void update_main(WINDOW *win, int max_col, int max_row);
void destroy_win(WINDOW *local_win);

int main() {
	WINDOW *main_window;
	WINDOW *side_window;
	int startx, starty, width, height;
	int ch, row, col;

	initscr();			/* Start curses mode 		*/
	start_color();
	cbreak();
	curs_set(0);
	keypad(stdscr, TRUE);		/* I need that nifty F1 	*/
	getmaxyx(stdscr, row, col);
	init_pair(1, COLOR_GREEN, COLOR_BLACK);
	attron(A_BOLD | COLOR_PAIR(1));

	height = 3;
	width = 10;
	starty = (LINES - height) / 2;	/* Calculating for a center placement */
	startx = (COLS - width) / 2;	/* of the window		*/
	refresh();
	main_window = create_newwin(row, col*0.7, 0,0);
	side_window = create_newwin(row, col*0.3, 0, (col*0.7)+1);

	print_in_window(main_window, 1, 2, 20, "Elgbert ruler!!!");
	print_in_window(side_window, 1, 2, 20, "Suppe");
	getch();
	update_main(main_window, col*0.7 - 10, row );
	getch();
	endwin();			/* End curses mode		  */
	return 0;
}
WINDOW *create_newwin(int height, int width, int starty, int startx)
{	WINDOW *local_win;
	init_pair(1, COLOR_GREEN, COLOR_BLACK);
	wattron(local_win, A_BOLD | COLOR_PAIR(1));

	local_win = newwin(height, width, starty, startx);
	box(local_win, 0 , 0);		/* 0, 0 gives default characters 
					 * for the vertical and horizontal
					 * lines			*/
	wrefresh(local_win);		/* Show that box 		*/

	return local_win;
}

void print_in_window(WINDOW *win, int starty, int startx, int width, char *string)
{	int length, x, y;
	float temp;
	init_pair(1, COLOR_GREEN, COLOR_BLACK);
	wattron(win, A_BOLD | COLOR_PAIR(1));

	if(win == NULL)
		win = stdscr;
	getyx(win, y, x);
	if(startx != 0)
		x = startx;
	if(starty != 0)
		y = starty;
	if(width == 0)
		width = 80;

	length = strlen(string);
	temp = (width - length)/ 2;
//	x = startx + (int)temp;
	mvwprintw(win, y, x, "%s", string);
	box(win, 0, 0);
	wrefresh(win);
}

void update_main(WINDOW *win, int max_col, int max_row) {
	char command[] = "/usr/bin/wget -O - -q http://wb.lastfriday.no/json/checkins";
	char read_line[max_col - 4];
	int count;
	size_t len = 0;
	char *jsonString = NULL;
	enum json_type type;
	FILE *output;

	output = popen(command, "r");
	count = 0;
	while( getline(&jsonString, &len, output) != -1 && count < max_row-4) {
		json_object * jobj = json_tokener_parse(jsonString);
		if(jobj != NULL) {
			enum json_type type;
			char outputString[400];
			char *username = NULL;
			char *beer = NULL;
			char *brewery = NULL;
			char *comment = NULL;
			char *rating = NULL;
			json_object_object_foreach(jobj, key, val) {
				if(!strcmp("user_name", key)) {
					username = json_object_get_string(val);
				}
				else if(!strcmp("beer_name", key)) {
					beer = json_object_get_string(val);
				}
				else if(!strcmp("brewery_name", key)) {
					brewery = json_object_get_string(val);
				}
				else if(!strcmp("checkin_comment", key)) {
					comment = json_object_get_string(val);
				}
				else if(!strcmp("rating", key)) {
					rating = json_object_get_string(val);
				}
			}
			sprintf(outputString, "User %s rated %s by %s: %s", username, beer, brewery, rating);
			print_in_window(win, count, 2, 0, outputString);
			count++;
			if(strlen(comment) > 0) {
				sprintf(outputString, "\t\"%s\"", comment);
				print_in_window(win, count, 2, 0, outputString);
				count++;
			}
		}
	}
	pclose(output);
}
