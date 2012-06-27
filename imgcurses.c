#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <locale.h>
#include <ncurses.h>

#include "imgcurses_color.h"
#include "imgcurses_image.h"
#include "imgcurses_config.h"

static image_t img = {0};
static float zoom = 1.0;
static float offset_x = 0.0;
static float offset_y = 0.0;

#define QUALITY_LOW 0
#define QUALITY_HIGH 1

static int quality = QUALITY_LOW;

void event(int key) {

  switch(key) {
    case KEY_LEFT:  offset_x -= 1.0; break;
    case KEY_RIGHT: offset_x += 1.0; break;
    case KEY_UP:    offset_y -= 1.0; break;
    case KEY_DOWN:  offset_y += 1.0; break;
    
    case KEY_BACKSPACE:
      offset_x = 0.0;
      offset_y = 0.0;
      zoom = 1.0;
    break;
    
    case ']': zoom += 0.1; break;
    case '[': zoom -= 0.1; break;
    
    case 'r':
      if (quality == QUALITY_LOW) { quality = QUALITY_HIGH; }
      else if (quality == QUALITY_HIGH) { quality = QUALITY_LOW; }
    break;
    
  }
  
  zoom = zoom <= 0.1 ? 0.1 : zoom;
  
}

void render_low() {
  
  init_pair(0, COLOR_BLACK, COLOR_BLACK);
  init_pair(1, COLOR_BLACK, COLOR_RED);
  init_pair(2, COLOR_BLACK, COLOR_GREEN);
  init_pair(3, COLOR_BLACK, COLOR_YELLOW);
  init_pair(4, COLOR_BLACK, COLOR_BLUE);
  init_pair(5, COLOR_BLACK, COLOR_MAGENTA);
  init_pair(6, COLOR_BLACK, COLOR_CYAN);
  init_pair(7, COLOR_BLACK, COLOR_WHITE);
  
  int max_x, max_y;
  getmaxyx(stdscr, max_y, max_x);
  
  for(int x = 0; x < max_x; x++)
  for(int y = 0; y < max_y-1; y++) {
    
    int u1 = (x + offset_x) * (1/zoom) * pixel_ubuntu.width;
    int v1 = (y + offset_y) * (1/zoom) * pixel_ubuntu.height;
    
    int u2 = (x+1 + offset_x) * (1/zoom) * pixel_ubuntu.width;
    int v2 = (y+1 + offset_y) * (1/zoom) * pixel_ubuntu.height;
    
    color_t texel = image_sub_get(img, u1, v1, u2, v2);
    int closest = closest_color_index(colors_ubuntu, texel);
    
    attron(COLOR_PAIR(closest));
    mvaddch(y, x, ' ');
    attroff(COLOR_PAIR(closest));
    
  } 
  
  printw("%i%% +/- ", (int)roundf(zoom * 100));
  
}

void render_high() {
  
}

int main(int argc, char** argv) {	
  
  if (argc != 2) {
    printf("Usage: imgcurses <filename>\n");
    exit(0);
  }
  
  img = image_load(argv[1]);
  
  initscr();
  
  setlocale(LC_ALL, "");
  
	if(!has_colors()) {
    endwin();
    printf("Your terminal does not support color\n");
    exit(1);
  }
  
  start_color();
  
  cbreak();
  keypad(stdscr, TRUE);
  noecho();
  
  render_low();
  refresh();
      
  while (1) {
    
    int key = getch();
    if (key == 'q') {
      break;
    }
    
    event(key);
    
    if (quality == QUALITY_LOW) {
      render_low();
    } else if (quality == QUALITY_HIGH) {
      render_high();
    }
    
    refresh();
  }
  
  endwin();
  
  image_delete(img);
  
  return 0;
}
