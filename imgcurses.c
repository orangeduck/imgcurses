#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <ncurses.h>

#include "imgcurses_color.h"
#include "imgcurses_image.h"
#include "imgcurses_config.h"
#include "imgcurses_ascii.h"
#include "imgcurses_view.h"

static image_t img = {0};
static image_t img_charset = {0};
static charset_t charset = {0};
static float zoom = 1.0;
static int offset_x = 0;
static int offset_y = 0;

static int view_index = 0;

#define views_count 4
static view_t views_list[views_count];

static bool running = true;

static void start_color_pairs() {

  static int index = 0;
  
  for(int back = 0; back < 8; back++)
  for(int fore = 0; fore < 8; fore++) {
    init_pair(index, back, fore);
    index++;
  }

}

void event() {
  
  int key = getch();
  
  switch(key) {
    case KEY_LEFT:  offset_x -= 1.64; break;
    case KEY_RIGHT: offset_x += 1.64; break;
    case KEY_UP:    offset_y -= 1.64; break;
    case KEY_DOWN:  offset_y += 1.64; break;
    
    case KEY_BACKSPACE:
      offset_x = 0.0;
      offset_y = 0.0;
      zoom = 1.0;
    break;
    
    case ']':
      zoom += 0.1;
    break;
    case '[':
      zoom -= 0.1;
    break;
    
    case 'm':
      view_index++;
      if (view_index == views_count) { view_index = 0; }
    break;
    
    case 'q': running = false; break;
    
  }
  
  zoom = zoom <= 0.1 ? 0.1 : zoom;
  zoom = zoom >= 5.0 ? 5.0 : zoom;
  
}

void render() {
  
  int max_x, max_y;
  getmaxyx(stdscr, max_y, max_x);
  
  view_t view = views_list[view_index];

  for(int x = 0; x < max_x; x++) {
    for(int y = 0; y < max_y-1; y++) {
      view.view_func(img, charset, x, y, offset_x, offset_y, zoom);
    } 
  }
  
  printw("%i x %i pixels  %0.1f kB  %i%% ", img.width, img.height, image_filesize(img), (int)roundf(zoom * 100));
  
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
  charset = charset_load(img_charset, pixel_ubuntu);
  
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
