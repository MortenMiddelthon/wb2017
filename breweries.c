#include <stdio.h>
#include <ncursesw/curses.h>
#include <string.h>
#include <json-c/json.h>
#include <unistd.h>
#include <locale.h>
#include <wchar.h>

WINDOW *create_newwin(int height, int width, int starty, int startx, int box);
void print_in_window(WINDOW *win, int starty, int startx, int width, char *string);
void destroy_win(WINDOW *local_win);
void fetch_updates_main(WINDOW *win);
void fetch_updates_side(WINDOW *win);

int main() {
	WINDOW *side_window;
	WINDOW *side_frame;

	int ch, row, col;
	char *locale;
	locale = setlocale(LC_ALL, "");

	initscr();			/* Start curses mode 		*/
	start_color();
	cbreak();
	noecho();
	curs_set(1);
	keypad(stdscr, TRUE);		/* I need that nifty F1 	*/
	getmaxyx(stdscr, row, col);
	init_pair(1, COLOR_GREEN, COLOR_BLACK);
	attron(A_BOLD | COLOR_PAIR(1));

	refresh();
	side_frame = create_newwin(row, col, 0,0, 1);
	side_window = create_newwin(row-2, col-2, 1,1, 0);

	while(1) {
		fetch_updates_side(side_window);
		sleep(10);
	}
	getch();
	endwin();			/* End curses mode		  */
	return 0;
}
WINDOW *create_newwin(int height, int width, int starty, int startx, int box)
{	WINDOW *local_win;
	init_pair(1, COLOR_GREEN, COLOR_BLACK);

	local_win = newwin(height, width, starty, startx);
	if(box == 1) {
		box(local_win, 0 , 0);
	}
	wattrset(local_win, A_BOLD | COLOR_PAIR(1));
	wrefresh(local_win);

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
	//box(win, 0, 0);
	wrefresh(win);
}

void fetch_updates_side(WINDOW *win) {
	int x, y, c, line_count;
	int delay = 10000; // Delay between printing each character
	// Where to fetch JSON
	char breweries[] = "/usr/bin/wget -O - -q http://wb.lastfriday.no/json/topbreweries";
	size_t len = 0;
	char *jsonString = NULL;
	enum json_type type;
	FILE *output;

	// Set colours and font type
	init_pair(1, COLOR_GREEN, COLOR_BLACK);
	wattron(win, COLOR_PAIR(1));
	//box(win, 0, 0); // Redraw window border with correct colour

	// Get window dimensions
	getmaxyx(win, y , x);
	wclear(win); // Clear text from window

	// Read JSON output one line at a time
	line_count = 1;
	wattron(win, A_BOLD| A_UNDERLINE);
	output = popen(breweries, "r"); // Read from command pipe

	wattron(win, A_BOLD| A_UNDERLINE);
	mvwaddstr(win, getcury(win)+1, 1, "Top 10 breweries: \n");
	wattroff(win, A_UNDERLINE);
	while( getline(&jsonString, &len, output) != -1 && getcury(win) < y-4) {
		json_object * jobj = json_tokener_parse(jsonString);
		// Check if we have valid JSON objects before proceding
		if(jobj != NULL) {
			enum json_type type;
			char outputString[400];
			char brewery[256];
			char country[256];
			char average[10];
			char ratings[20];
			// Set checkin variables from JSON objects
			json_object_object_foreach(jobj, key, val) {
				if(!strcmp("brewery", key)) {
					strcpy(brewery, json_object_get_string(val));
				}
				else if(!strcmp("country", key)) {
					strcpy(country, json_object_get_string(val));
				}
				else if(!strcmp("average", key)) {
					strcpy(average, json_object_get_string(val));
				}
				else if(!strcmp("ratings", key)) {
					strcpy(ratings, json_object_get_string(val));
				}
			}
			// wattron(win, A_BOLD | COLOR_PAIR(1));
			mvwaddch(win, getcury(win),1, ' ');
			wattroff(win, A_BOLD);
			waddch(win, 'B'); usleep(delay); wrefresh(win);
			waddch(win, 'r'); usleep(delay); wrefresh(win);
			waddch(win, 'e'); usleep(delay); wrefresh(win); 
			waddch(win, 'w'); usleep(delay); wrefresh(win); 
			waddch(win, 'e'); usleep(delay); wrefresh(win); 
			waddch(win, 'r'); usleep(delay); wrefresh(win); 
			waddch(win, 'y'); usleep(delay); wrefresh(win); 
			waddch(win, ' '); usleep(delay); wrefresh(win); 
			wattron(win, A_BOLD);
			waddstr(win, brewery); usleep(delay); wrefresh(win);
			wattroff(win, A_BOLD);

			waddch(win, ' '); usleep(delay); wrefresh(win); 
			waddch(win, 'f'); usleep(delay); wrefresh(win);
			waddch(win, 'r'); usleep(delay); wrefresh(win); 
			waddch(win, 'o'); usleep(delay); wrefresh(win); 
			waddch(win, 'm'); usleep(delay); wrefresh(win); 
			waddch(win, ' '); usleep(delay); wrefresh(win); 
			wattron(win, A_BOLD);
			waddstr(win, country); usleep(delay); wrefresh(win);
			wattroff(win, A_BOLD);

			waddch(win, '\n'); usleep(delay); wrefresh(win);
			waddch(win, '\t'); usleep(delay); wrefresh(win);
			waddch(win, 'a'); usleep(delay); wrefresh(win);
			waddch(win, 'v'); usleep(delay); wrefresh(win);
			waddch(win, 'e'); usleep(delay); wrefresh(win);
			waddch(win, 'r'); usleep(delay); wrefresh(win);
			waddch(win, 'a'); usleep(delay); wrefresh(win);
			waddch(win, 'g'); usleep(delay); wrefresh(win);
			waddch(win, 'e'); usleep(delay); wrefresh(win);
			waddch(win, ' '); usleep(delay); wrefresh(win);

			wattron(win, A_BOLD);
			for(c = 0; c < strlen(average); c++) {
				waddch(win, average[c]);
				usleep(delay);
				wrefresh(win);
			}
			wattroff(win, A_BOLD);
			waddch(win, '\n'); usleep(delay); wrefresh(win);
			waddch(win, '\t'); usleep(delay); wrefresh(win);
			waddch(win, 'r'); usleep(delay); wrefresh(win);
			waddch(win, 'a'); usleep(delay); wrefresh(win);
			waddch(win, 't'); usleep(delay); wrefresh(win);
			waddch(win, 'i'); usleep(delay); wrefresh(win);
			waddch(win, 'n'); usleep(delay); wrefresh(win);
			waddch(win, 'g'); usleep(delay); wrefresh(win);
			waddch(win, 's'); usleep(delay); wrefresh(win);
			waddch(win, ' '); usleep(delay); wrefresh(win);

			wattron(win, A_BOLD);
			for(c = 0; c < strlen(ratings); c++) {
				waddch(win, ratings[c]);
				usleep(delay);
				wrefresh(win);
			}
			// End line
			waddch(win, '\n');
			//box(win, 0, 0); // Redraw window border with correct colour
			wrefresh(win);
			line_count++;

		}
	}

	pclose(output);
}
