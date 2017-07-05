#include <stdio.h>
#include <ncurses.h>
#include <string.h>
#include <json-c/json.h>
#include <unistd.h>

WINDOW *create_newwin(int height, int width, int starty, int startx);
void print_in_window(WINDOW *win, int starty, int startx, int width, char *string);
void update_main(WINDOW *win, int max_col, int max_row);
void destroy_win(WINDOW *local_win);
void fetch_updates_main(WINDOW *win);

int main() {
	WINDOW *main_window;
	WINDOW *side_window;
	int startx, starty, width, height;
	int ch, row, col;

	initscr();			/* Start curses mode 		*/
	start_color();
	cbreak();
	noecho();
	curs_set(1);
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
	side_window = create_newwin(row, col*0.3-1, 0, (col*0.7)+1);
	print_in_window(side_window, 1, 2, 20, "Suppe");
	/*
	do {
		update_main(main_window, col*0.7-1, row );
		ch = getch();
	} while(ch != 'x');
	*/
	fetch_updates_main(main_window);
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

	/*
	length = strlen(string);
	temp = (width - length)/ 2;
	x = startx + (int)temp;
	*/
	mvwprintw(win, y, x, "%s", string);
	box(win, 0, 0);
	wrefresh(win);
}

void update_main(WINDOW *win, int max_col, int max_row) {
	char command[] = "/usr/bin/wget -O - -q http://wb.lastfriday.no/json/checkins";
	int count;
	size_t len = 0;
	char *jsonString = NULL;
	enum json_type type;
	FILE *output;

	output = popen(command, "r");
	count = 1;
	wclear(win);
	while( getline(&jsonString, &len, output) != -1 && count < max_row-2) {
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
			if(strlen(outputString) > max_col-1 ) {
				count++;
			}
			count++;
			if(strlen(comment) > 0) {
				sprintf(outputString, "\t\"%s\"", comment);
				print_in_window(win, count, 2, 0, outputString);
				count++;
			}
			usleep(100000);
		}
	}
	print_in_window(win, count, 2, 0, "......");
	pclose(output);
}

void fetch_updates_main(WINDOW *win) {
	int x, y, c, line_count;
	int delay = 10000; // Delay between printing each character
	// Where to fetch JSON
	char command[] = "/usr/bin/wget -O - -q http://wb.lastfriday.no/json/checkins";
	char testString[] = "This is a long string we want to print each character at a time....";
	size_t len = 0;
	char *jsonString = NULL;
	enum json_type type;
	FILE *output;

	// Set colours and font type
	init_pair(1, COLOR_GREEN, COLOR_BLACK);
	wattron(win, COLOR_PAIR(1));
	box(win, 0, 0); // Redraw window border with correct colour

	// Get window dimensions
	getmaxyx(win, y , x);

	output = popen(command, "r"); // Read from command pipe

	// wclear(win); // Clear text from window

	// Read JSON output one line at a time
	line_count = 1;
	while( getline(&jsonString, &len, output) != -1 && line_count < y-4) {
		json_object * jobj = json_tokener_parse(jsonString);
		// Check if we have valid JSON objects before proceding
		if(jobj != NULL) {
			enum json_type type;
			char outputString[400];
			char *username = NULL;
			char *beer = NULL;
			char *brewery = NULL;
			char *comment = NULL;
			char *rating = NULL;
			// Set checkin variables from JSON objects
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
			// wattron(win, A_BOLD | COLOR_PAIR(1));
			mvwaddch(win, line_count,1, ' ');
			wattroff(win, A_BOLD);
			waddch(win, 'U'); usleep(delay); wrefresh(win);
			waddch(win, 's'); usleep(delay); wrefresh(win);
			waddch(win, 'e'); usleep(delay); wrefresh(win); 
			waddch(win, 'r'); usleep(delay); wrefresh(win); 
			waddch(win, ' '); usleep(delay); wrefresh(win); 
			wattron(win, A_BOLD);
			for(c = 0; c < strlen(username); c++) {
				waddch(win, username[c]);
				usleep(delay);
				wrefresh(win);
			}
			wattroff(win, A_BOLD);
			waddch(win, ' '); usleep(delay); wrefresh(win);
			waddch(win, 'r'); usleep(delay); wrefresh(win);
			waddch(win, 'a'); usleep(delay); wrefresh(win);
			waddch(win, 't'); usleep(delay); wrefresh(win);
			waddch(win, 'e'); usleep(delay); wrefresh(win);
			waddch(win, 'd'); usleep(delay); wrefresh(win);
			waddch(win, ' '); usleep(delay); wrefresh(win);

			wattron(win, A_BOLD);
			for(c = 0; c < strlen(beer); c++) {
				waddch(win, beer[c]);
				usleep(delay);
				wrefresh(win);
			}
			wattroff(win, A_BOLD);
			waddch(win, ' '); usleep(delay); wrefresh(win);
			waddch(win, 'b'); usleep(delay); wrefresh(win);
			waddch(win, 'y'); usleep(delay); wrefresh(win);
			waddch(win, ' '); usleep(delay); wrefresh(win);

			wattron(win, A_BOLD);
			for(c = 0; c < strlen(brewery); c++) {
				waddch(win, brewery[c]);
				usleep(delay);
				wrefresh(win);
			}
			waddch(win, ' '); usleep(delay); wrefresh(win);
			for(c = 0; c < strlen(rating); c++) {
				waddch(win, rating[c]);
				usleep(delay);
				wrefresh(win);
			}
			wattroff(win, A_BOLD);
			if(strlen(comment) > 0) {
				waddch(win, '\n'); usleep(delay); wrefresh(win);
				waddch(win, '\t'); usleep(delay); wrefresh(win);
				waddch(win, '"'); usleep(delay); wrefresh(win);
				for(c = 0; c < strlen(comment); c++) {
					waddch(win, comment[c]);
					usleep(delay);
					wrefresh(win);
				}
				waddch(win, '"'); usleep(delay); wrefresh(win);
				line_count++;
			}
			// End line
			waddch(win, '\n');
			box(win, 0, 0); // Redraw window border with correct colour
			wrefresh(win);
			line_count++;

		}
	}
	/*
	for(c = 0; c < strlen(testString); c++) {
		waddch(win, testString[c]);
		usleep(50000);
		wrefresh(win);
	}
	*/
	mvwaddch(win, line_count,2, '.'); usleep(delay); wrefresh(win);
	waddch(win, '.'); usleep(delay); wrefresh(win);
	waddch(win, '.'); usleep(delay); wrefresh(win);
	waddch(win, '.'); usleep(delay); wrefresh(win);
	waddch(win, '.'); usleep(delay); wrefresh(win);
	waddstr(win, " Ready "); wrefresh(win);
}
