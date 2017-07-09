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
	WINDOW *main_window;
	WINDOW *main_frame;
	WINDOW *side_window;
	WINDOW *side_frame;
	WINDOW *bottom_window;
	WINDOW *bottom_frame;

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
	main_frame = create_newwin(row, col*0.6, 0,0, 1);
	main_window = create_newwin(row-2, (col*0.6)-2, 1,1, 0);
	side_frame = create_newwin(row - 5, (col*0.4)-1, 0, (col*0.6)+1, 1);
	side_window = create_newwin(row-7, (col*0.4)-3, 1, (col*0.6)+2, 0);
	bottom_frame = create_newwin(5, (col*0.4)-1, row-5,  (col*0.6)+1, 1);
	bottom_window = create_newwin(3, (col*0.4)-3, row-4,  (col*0.6)+2, 0);

	while(1) {
		print_in_window(bottom_window, 1, 2, 20, "WB2017: Incoming checkins ....");
		fetch_updates_main(main_window);
		sleep(1);
		print_in_window(bottom_window, 1, 2, 20, "WB2017: Incoming ratings ....");
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

void fetch_updates_main(WINDOW *win) {
	int x, y, c, line_count;
	int delay = 10000; // Delay between printing each character
	// Where to fetch JSON
	char command[] = "/usr/bin/wget -O - -q http://wb.lastfriday.no/json/checkins";
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

	output = popen(command, "r"); // Read from command pipe

	wclear(win); // Clear text from window

	// Print some Russian....
	/*
	mvwaddstr(win, 1,1, "Pivo dlya lyudey!\n");
	wrefresh(win);
	sleep(3);
	*/

	// Read JSON output one line at a time
	line_count = 1;
	mvwaddch(win, 0,0, ' ');
	while( getline(&jsonString, &len, output) != -1 && getcury(win) < y-4) {
		json_object * jobj = json_tokener_parse(jsonString);
		// Check if we have valid JSON objects before proceding
		if(jobj != NULL) {
			enum json_type type;
			char outputString[400];
			char username[256];
			char beer[256];
			char brewery[256];
			char comment[256];
			char rating[256];
			char date[256];
			// Set checkin variables from JSON objects
			json_object_object_foreach(jobj, key, val) {
				if(!strcmp("user_name", key)) {
					strcpy(username, json_object_get_string(val));
				}
				else if(!strcmp("beer_name", key)) {
					strcpy(beer, json_object_get_string(val));
				}
				else if(!strcmp("brewery_name", key)) {
					strcpy(brewery, json_object_get_string(val));
				}
				else if(!strcmp("checkin_comment", key)) {
					strcpy(comment, json_object_get_string(val));
				}
				else if(!strcmp("rating", key)) {
					strcpy(rating, json_object_get_string(val));
				}
				else if(!strcmp("date", key)) {
					strcpy(date, json_object_get_string(val));
				}
			}
			// wattron(win, A_BOLD | COLOR_PAIR(1));
			mvwaddch(win, getcury(win),0, ' ');
			wattroff(win, A_BOLD);
			for(c = 0; c < strlen(date); c++) {
				waddch(win, date[c]);
				usleep(delay);
				wrefresh(win);
			}
			waddch(win, ' '); usleep(delay); wrefresh(win);
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
			/*
			for(c = 0; c < strlen(beer); c++) {
				waddch(win, beer[c]);
				usleep(delay);
				wrefresh(win);
			}
			*/
			waddstr(win, beer); usleep(delay); wrefresh(win);
			wattroff(win, A_BOLD);
			waddch(win, ' '); usleep(delay); wrefresh(win);
			waddch(win, 'b'); usleep(delay); wrefresh(win);
			waddch(win, 'y'); usleep(delay); wrefresh(win);
			waddch(win, ' '); usleep(delay); wrefresh(win);

			wattron(win, A_BOLD);
			/*
			for(c = 0; c < strlen(brewery); c++) {
				waddch(win, brewery[c]);
				usleep(delay);
				wrefresh(win);
			}
			*/
			waddstr(win, brewery); usleep(delay); wrefresh(win);
			waddch(win, ' '); usleep(delay); wrefresh(win);
			wattron(win, A_BOLD | A_UNDERLINE);
			for(c = 0; c < strlen(rating); c++) {
				waddch(win, rating[c]);
				usleep(delay);
				wrefresh(win);
			}
			if((strlen(beer)+strlen(brewery)+40) > x) {
				line_count++;
			}
			wattroff(win, A_BOLD | A_UNDERLINE);
			if(strlen(comment) > 0) {
				waddch(win, '\n'); usleep(delay); wrefresh(win);
				waddch(win, '\t'); usleep(delay); wrefresh(win);
				waddch(win, '"'); usleep(delay); wrefresh(win);
				if(strlen(comment) > 50) {
					waddnstr(win, comment, 30); usleep(delay); wrefresh(win);
					waddch(win, '.'); usleep(delay); wrefresh(win);
					waddch(win, '.'); usleep(delay); wrefresh(win);
					waddch(win, '.'); usleep(delay); wrefresh(win);
					waddch(win, '"'); usleep(delay); wrefresh(win);
					waddstr(win, " yada yada."); usleep(delay); wrefresh(win);

				}
				else {
					waddstr(win, comment); usleep(delay); wrefresh(win);
					waddch(win, '"'); usleep(delay); wrefresh(win);
				}
				/*
				for(c = 0; c < strlen(comment); c++) {
					waddch(win, comment[c]);
					usleep(delay);
					wrefresh(win);
				}
				*/
				line_count++;
			}
			// End line
			waddch(win, '\n');
			//box(win, 0, 0); // Redraw window border with correct colour
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
	/*
	mvwaddch(win, line_count,2, '.'); usleep(delay); wrefresh(win);
	waddch(win, '.'); usleep(delay); wrefresh(win);
	waddch(win, '.'); usleep(delay); wrefresh(win);
	waddch(win, '.'); usleep(delay); wrefresh(win);
	waddch(win, '.'); usleep(delay); wrefresh(win);
	waddstr(win, " Ready "); wrefresh(win);
	*/
	pclose(output);
}

void fetch_updates_side(WINDOW *win) {
	int x, y, c, line_count;
	int delay = 10000; // Delay between printing each character
	// Where to fetch JSON
	char beers[] = "/usr/bin/wget -O - -q http://wb.lastfriday.no/json/topbeers";
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

	output = popen(beers, "r"); // Read from command pipe

	wclear(win); // Clear text from window

	// Read JSON output one line at a time
	line_count = 1;
	wattron(win, A_BOLD| A_UNDERLINE);
	mvwaddstr(win, 1,1, "Top 5 beers: \n");
	wattroff(win, A_UNDERLINE);
	while( getline(&jsonString, &len, output) != -1 && getcury(win) < y-4) {
		json_object * jobj = json_tokener_parse(jsonString);
		// Check if we have valid JSON objects before proceding
		if(jobj != NULL) {
			enum json_type type;
			char outputString[400];
			char beer[256];
			char average[256];
			char ratings[256];
			// Set checkin variables from JSON objects
			json_object_object_foreach(jobj, key, val) {
				if(!strcmp("beer", key)) {
					strcpy(beer, json_object_get_string(val));
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
			waddch(win, 'e'); usleep(delay); wrefresh(win);
			waddch(win, 'e'); usleep(delay); wrefresh(win); 
			waddch(win, 'r'); usleep(delay); wrefresh(win); 
			waddch(win, ' '); usleep(delay); wrefresh(win); 
			wattron(win, A_BOLD);
			waddstr(win, beer); usleep(delay); wrefresh(win);
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

	line_count = getcury(win);
	// waddch(win, '.'); usleep(delay); wrefresh(win);
	pclose(output);
	sleep(2);
	output = popen(breweries, "r"); // Read from command pipe

	wattron(win, A_BOLD| A_UNDERLINE);
	mvwaddstr(win, getcury(win)+1, 1, "Top 5 breweries: \n");
	wattroff(win, A_UNDERLINE);
	while( getline(&jsonString, &len, output) != -1 && getcury(win) < y-4) {
		json_object * jobj = json_tokener_parse(jsonString);
		// Check if we have valid JSON objects before proceding
		if(jobj != NULL) {
			enum json_type type;
			char outputString[400];
			char brewery[256];
			char average[10];
			char ratings[20];
			// Set checkin variables from JSON objects
			json_object_object_foreach(jobj, key, val) {
				if(!strcmp("brewery", key)) {
					strcpy(brewery, json_object_get_string(val));
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
