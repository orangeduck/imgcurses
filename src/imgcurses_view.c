#include <stdlib.h>
#include <ncurses.h>

#include "imgcurses_color.h"
#include "imgcurses_config.h"
#include "imgcurses_ascii.h"
#include "imgcurses_view.h"

static int color_pair_index(int back, int fore) {
  return 8 * back + fore;
}

void view_func_solid(image_t img, charset_t charset, int x, int y, int off_x, int off_y, float zoom) {

  int u1 = (x + off_x) * (1/zoom) * pixel_ubuntu.width;
  int v1 = (y + off_y) * (1/zoom) * pixel_ubuntu.height;
  
  int u2 = (x+1 + off_x) * (1/zoom) * pixel_ubuntu.width;
  int v2 = (y+1 + off_y) * (1/zoom) * pixel_ubuntu.height;
  
  color_t texel = image_sub_get(img, u1, v1, u2, v2);
  color_info_t info = color_info(colors_ubuntu, texel);
  
  mvaddch(y, x, ' ' | COLOR_PAIR(color_pair_index(COLOR_BLACK, info.primary)));

}

void view_func_value(image_t img, charset_t charset, int x, int y, int off_x, int off_y, float zoom) {

  int u1 = (x + off_x) * (1/zoom) * pixel_ubuntu.width;
  int v1 = (y + off_y) * (1/zoom) * pixel_ubuntu.height;
  
  int u2 = (x+1 + off_x) * (1/zoom) * pixel_ubuntu.width;
  int v2 = (y+1 + off_y) * (1/zoom) * pixel_ubuntu.height;
  
  color_t texel = image_sub_get(img, u1, v1, u2, v2);
  color_info_t info = color_info(colors_ubuntu, texel);
  
  char c = charset_char_for_coverage(charset, info.value);
  mvaddch(y, x, c | COLOR_PAIR(color_pair_index(info.primary, COLOR_BLACK)));

}

void view_func_color(image_t img, charset_t charset, int x, int y, int off_x, int off_y, float zoom) {

  int u1 = (x + off_x) * (1/zoom) * pixel_ubuntu.width;
  int v1 = (y + off_y) * (1/zoom) * pixel_ubuntu.height;
  
  int u2 = (x+1 + off_x) * (1/zoom) * pixel_ubuntu.width;
  int v2 = (y+1 + off_y) * (1/zoom) * pixel_ubuntu.height;
  
  if (image_sub_contained(img, u1, v1, u2, v1)) {
  
    color_t texel = image_sub_get(img, u1, v1, u2, v2);
    color_info_t info = color_info(colors_ubuntu, texel);
      
    char c = charset_char_for_coverage(charset, info.secondary_amount);
    mvaddch(y, x, c | COLOR_PAIR(color_pair_index(info.secondary, info.primary)));
  
  } else {
  
    mvaddch(y, x, ' ' | COLOR_PAIR(color_pair_index(COLOR_BLACK, COLOR_BLACK)));
  
  }

}

void view_func_detail(image_t img, charset_t charset, int x, int y, int off_x, int off_y, float zoom) {

  int u1 = (x + off_x) * (1/zoom) * pixel_ubuntu.width;
  int v1 = (y + off_y) * (1/zoom) * pixel_ubuntu.height;
  
  int u2 = (x+1 + off_x) * (1/zoom) * pixel_ubuntu.width;
  int v2 = (y+1 + off_y) * (1/zoom) * pixel_ubuntu.height;
  
  if (image_sub_contained(img, u1, v1, u2, v1)) {
    
    color_t texel = image_sub_get(img, u1, v1, u2, v2);
    color_info_t info = color_info(colors_ubuntu, texel);
    
    char c = charset_image_sub_match(charset, texel, info, img, u1, v1, u2, v2);
    
    mvaddch(y, x, c | COLOR_PAIR(color_pair_index(info.secondary, info.primary)));
    
  } else {
  
    mvaddch(y, x, ' ' | COLOR_PAIR(color_pair_index(COLOR_BLACK, COLOR_BLACK)));
  
  }

}
