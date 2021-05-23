#include <ncurses.h>
#include "Map.h"

// set the game base settings
void setBase(){
  initscr();
  curs_set(0);  // invisible cursor
  noecho();     // invisible keypad input
  nodelay(stdscr, 1);
  start_color();
	init_pair(1, COLOR_BLUE, COLOR_YELLOW); // background color
	init_pair(2, COLOR_RED, COLOR_GREEN); // wall color
	init_pair(3, COLOR_YELLOW, COLOR_RED);  //head color
	init_pair(4, COLOR_YELLOW, COLOR_BLUE); // growth item color
	init_pair(5, COLOR_GREEN, COLOR_MAGENTA); // poison item color
	bkgd(COLOR_PAIR(1));
}

void DrawMap(int level){
  // initialize the window
  clear();
  // according to level
  switch(level){
    case 1:
      //HEIGHT = 20; WIDTH = 60;
      break;
    case 2:
      //HEIGHT = 20; WIDTH = 60;
      attron(COLOR_PAIR(2));
      for(int i=0; i<HEIGHT/2; i++){
        mvprintw(i, 20, "1");
      }
      attroff(COLOR_PAIR(2));
      break;

    case 3:
      //
    case 4:
      //
      break;
  }
  	resize_term(HEIGHT, WIDTH);
    attron(COLOR_PAIR(2));  // border color on
  	border('1', '1', '1', '1', '2', '2', '2', '2');
  	attroff(COLOR_PAIR(2)); // off
    refresh();
}