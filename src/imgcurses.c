#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <ncurses.h>

#include "imgcurses.h"

#define BUFF_STAT_DIRTY 0
#define BUFF_STAT_CLEAN 1
#define BUFF_STAT_COPY 2

typedef struct {
  char status;
  short copy_x, copy_y;
} buff_stat;

static chtype* front_buffer = NULL;
static chtype* back_buffer = NULL;

static chtype* curr_buffer = NULL;
static chtype* alt_buffer = NULL;

static buff_stat* status_buffer = NULL;

static int buffer_width = 0;
static int buffer_height = 0;

static color_config_t config;

static void flip_buffer() {
  
  for(int x = 0; x < buffer_width; x++) {
    for(int y = 0; y < buffer_height; y++) {
      mvaddch(y, x, curr_buffer[x + buffer_width * y]);
    } 
  }
  
  if (curr_buffer == front_buffer) {
    curr_buffer = back_buffer;
    alt_buffer = front_buffer;
  } else if (curr_buffer == back_buffer) {
    curr_buffer = front_buffer;
    alt_buffer = back_buffer;
  }
  
}

static void update_buffer() {
  
  view_t view = views_list[view_index];
  chtype chattr;
  
  for(int x = 0; x < buffer_width; x++) {
    for(int y = 0; y < buffer_height; y++) {
  
      buff_stat stat = status_buffer[x + buffer_width * y];
      
      switch(stat.status) {
        case BUFF_STAT_DIRTY:
          
          chattr = view.view_func(img, charset_default, &config, x, y, offset_x, offset_y, zoom);
          
          curr_buffer[x + buffer_width * y] = chattr;
          status_buffer[x + buffer_width * y].status = BUFF_STAT_CLEAN;
        break;
        
        case BUFF_STAT_COPY:
          
          chattr = alt_buffer[stat.copy_x + buffer_width * stat.copy_y];
          
          curr_buffer[x + buffer_width * y] = chattr;
          status_buffer[x + buffer_width * y].status = BUFF_STAT_CLEAN;
          
        break;
      }
      
    }
  }
  
}

static void dirty_buffer() {
  memset(status_buffer, 0, sizeof(buff_stat) * buffer_width * buffer_height);
}

static void offset_buffer(int offset_x, int offset_y) {
  
  for(int x = 0; x < buffer_width; x++) {
    for(int y = 0; y < buffer_height; y++) {
      
      int g_offset_x = x + offset_x;
      int g_offset_y = y + offset_y;
      
      if ((g_offset_x < 0) || (g_offset_x >= buffer_width) || 
          (g_offset_y < 0) || (g_offset_y >= buffer_height)) {
          
        status_buffer[x + buffer_width * y].status = BUFF_STAT_DIRTY;  
        
      } else {
      
        status_buffer[x + buffer_width * y].status = BUFF_STAT_COPY;
        status_buffer[x + buffer_width * y].copy_x = g_offset_x;
        status_buffer[x + buffer_width * y].copy_y = g_offset_y;
        
      }

    }
  }
  
}

static void render() {
  
  int max_x, max_y;
  getmaxyx(stdscr, max_y, max_x);
  
  if ((max_x != buffer_width) || (max_y-1 != buffer_height)) {
    
    buffer_width = max_x;
    buffer_height = max_y-1;
    
    front_buffer = realloc(front_buffer, sizeof(chtype) * buffer_width * buffer_height);
    back_buffer = realloc(back_buffer, sizeof(chtype) * buffer_width * buffer_height);
    
    curr_buffer = front_buffer;
    alt_buffer = back_buffer;
    
    status_buffer = realloc(status_buffer, sizeof(buff_stat) * buffer_width * buffer_height);
    dirty_buffer();
  }


  update_buffer();
  flip_buffer();
  
  for(int x = 0; x < max_x; x++) { mvaddch(max_y - 1, x, ' '); }
  
  view_t view = views_list[view_index];
  mvprintw(max_y - 1, 0, "%i x %i pixels  %0.1f kB  %i%% ", img.width, img.height, image_filesize(img), (int)roundf(zoom * 100));
  mvprintw(max_y - 1, max_x - strlen(view.view_name), "%s", view.view_name);
  
}

static bool running = true;

static void move_cursor_left()  { offset_x -= 2; offset_buffer(-2, 0); }
static void move_cursor_right() { offset_x += 2; offset_buffer( 2, 0); }
static void move_cursor_up()    { offset_y -= 2; offset_buffer(0, -2); }
static void move_cursor_down()  { offset_y += 2; offset_buffer(0,  2); }

static void event() {

  int key = getch();
  
  switch(key) {
    case KEY_LEFT:  move_cursor_left(); break;
    case KEY_RIGHT: move_cursor_right(); break;
    case KEY_UP:    move_cursor_up(); break;
    case KEY_DOWN:  move_cursor_down(); break;

    /* vi key-bindings */
    case 'h': move_cursor_left(); break;
    case 'j': move_cursor_down(); break;
    case 'k': move_cursor_up(); break;
    case 'l': move_cursor_right(); break;
    
    case KEY_BACKSPACE:
      offset_x = 0.0;
      offset_y = 0.0;
      zoom = 1.0;
      dirty_buffer(); 
    break;
    
    case ']':
      if (zoom >= 4.9) break;
      zoom += 0.1; dirty_buffer();
    break;
    
    case '[':
      if (zoom <= 0.1) break;
      zoom -= 0.1; dirty_buffer();
    break;
    
    case 'm':
      view_index++;
      if (view_index == VIEW_COUNT) { view_index = 0; }
      dirty_buffer();
    break;
    
    case 'q':
      running = false;
    break;
    
  }
  
}

static void start_color_pairs() {

  static int index = 0;
  
  for(int back = 0; back < 8; back++)
  for(int fore = 0; fore < 8; fore++) {
    init_pair(index, back, fore);
    index++;
  }

}


static void start_color_config() {
  
  short r, g, b;
  
  for(int i = 0; i < 8; i++) {
    color_content(i, &r, &g, &b);
    config[i] = color_new(255 * ((float)r / 1000.0f), 
                          255 * ((float)g / 1000.0f), 
                          255 * ((float)b / 1000.0f));         
  } 
  
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
  
  initscr();
  
	if(!has_colors()) {
    endwin();
    printf("Your terminal does not support colors!\n");
    exit(1);
  }
  
  start_color();
  start_color_pairs();
  start_color_config();
  
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
  
  return 0;
}
