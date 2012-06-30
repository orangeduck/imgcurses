#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <ncurses.h>

#include "imgcurses.h"

static bool running = true;

static void start_color_pairs() {

  static int index = 0;
  
  for(int back = 0; back < 8; back++)
  for(int fore = 0; fore < 8; fore++) {
    init_pair(index, back, fore);
    index++;
  }

}

static void event() {
  
  int key = getch();
  
  switch(key) {
    case KEY_LEFT:  offset_x -= 2.0; break;
    case KEY_RIGHT: offset_x += 2.0; break;
    case KEY_UP:    offset_y -= 2.0; break;
    case KEY_DOWN:  offset_y += 2.0; break;
    
    case KEY_BACKSPACE:
      offset_x = 0.0;
      offset_y = 0.0;
      zoom = 1.0;
    break;
    
    case ']':
      if (zoom >= 4.9) break;
      zoom += 0.1;
    break;
    case '[':
      if (zoom <= 0.1) break;
      zoom -= 0.1;
    break;
    
    case 'm':
      view_index++;
      if (view_index == VIEW_COUNT) { view_index = 0; }
    break;
    
    case 'q': running = false; break;
    
  }
  
}

static void render() {
  
  int max_x, max_y;
  getmaxyx(stdscr, max_y, max_x);
  
  view_t view = views_list[view_index];

  for(int x = 0; x < max_x; x++) {
    for(int y = 0; y < max_y-1; y++) {
      view.view_func(img, charset, x, y, offset_x, offset_y, zoom);
    } 
  }
  
  for(int x = 0; x < max_x; x++) { mvaddch(max_y - 1, x, ' '); }
  
  mvprintw(max_y - 1, 0, "%i x %i pixels  %0.1f kB  %i%% ", img.width, img.height, image_filesize(img), (int)roundf(zoom * 100));
  mvprintw(max_y - 1, max_x - strlen(view.view_name), "%s", view.view_name);
  
}

int main(int argc, char** argv) {	
  
  if (argc != 2) {
    printf("Usage: imgcurses <filename>\n");
    exit(0);
  }
  
  views_list[0] = view_solid;
  views_list[1] = view_value;
  views_list[2] = view_color;
  views_list[3] = view_detail;
  
  img = image_load(argv[1]);
  img_charset = image_load("ubuntu_charset.tga");
  charset = charset_load(img_charset);
  
  initscr();
  
	if(!has_colors()) {
    endwin();
    printf("Your terminal does not support color\n");
    exit(1);
  }
  
  start_color();
  start_color_pairs();
  
  cbreak();
  keypad(stdscr, TRUE);
  noecho();
  curs_set(0);
  
  while (running) {
    
    render();
    refresh();
    event();
    
  }
  
  endwin();
  
  image_delete(img);
  image_delete(img_charset);
  
  return 0;
}
