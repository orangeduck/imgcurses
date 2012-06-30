#include <stdlib.h>
#include <math.h>
#include <ncurses.h>

#include "imgcurses.h"

static int color_pair_index(int back, int fore) {
  return 8 * back + fore;
}

chtype view_func_solid(image_t img, charset_t charset, int x, int y, int off_x, int off_y, float zoom) {

  int u1 = (x + off_x) * (1/zoom) * CHAR_WIDTH;
  int v1 = (y + off_y) * (1/zoom) * CHAR_HEIGHT;
  int u2 = (x+1 + off_x) * (1/zoom) * CHAR_WIDTH;
  int v2 = (y+1 + off_y) * (1/zoom) * CHAR_HEIGHT;
  
  color_t texel = image_sub_get(img, u1, v1, u2, v2);
  color_info_t info = color_info(colors_default, texel);
  
  return (' ' | COLOR_PAIR(color_pair_index(COLOR_BLACK, info.primary)));

}

chtype view_func_value(image_t img, charset_t charset, int x, int y, int off_x, int off_y, float zoom) {

  int u1 = (x + off_x) * (1/zoom) * CHAR_WIDTH;
  int v1 = (y + off_y) * (1/zoom) * CHAR_HEIGHT;
  int u2 = (x+1 + off_x) * (1/zoom) * CHAR_WIDTH;
  int v2 = (y+1 + off_y) * (1/zoom) * CHAR_HEIGHT;
  
  color_t texel = image_sub_get(img, u1, v1, u2, v2);
  color_info_t info = color_info(colors_default, texel);
  
  const float contrast = 1.3;
  info.value = pow(info.value, contrast);
  
  char c = charset_char_for_coverage(charset, info.value);
  
  if (info.value > 0.5) {
    return (c | A_BOLD | COLOR_PAIR(color_pair_index(COLOR_WHITE, COLOR_BLACK)));
  } else {
    return (c | COLOR_PAIR(color_pair_index(COLOR_WHITE, COLOR_BLACK)));
  }

}

chtype view_func_color(image_t img, charset_t charset, int x, int y, int off_x, int off_y, float zoom) {

  int u1 = (x + off_x) * (1/zoom) * CHAR_WIDTH;
  int v1 = (y + off_y) * (1/zoom) * CHAR_HEIGHT;
  int u2 = (x+1 + off_x) * (1/zoom) * CHAR_WIDTH;
  int v2 = (y+1 + off_y) * (1/zoom) * CHAR_HEIGHT;
  
  if (image_sub_contained(img, u1, v1, u2, v1)) {
  
    color_t texel = image_sub_get(img, u1, v1, u2, v2);
    color_info_t info = color_info(colors_default, texel);
    
    const float contrast = 1.25;
    info.secondary_amount = pow(info.secondary_amount, contrast);
    
    char c = charset_char_for_coverage(charset, info.secondary_amount);
    int c_id = charset_char_id(charset, c);
    
    if (charset.coverages[c_id] > 0.5) {
      return (c | A_BOLD | COLOR_PAIR(color_pair_index(info.secondary, info.primary)));
    } else {
      return (c | COLOR_PAIR(color_pair_index(info.secondary, info.primary)));
    }
    
  } else {
  
    return (' ' | COLOR_PAIR(0));
  
  }

}

chtype view_func_detail(image_t img, charset_t charset, int x, int y, int off_x, int off_y, float zoom) {

  int u1 = (x + off_x) * (1/zoom) * CHAR_WIDTH;
  int v1 = (y + off_y) * (1/zoom) * CHAR_HEIGHT;
  int u2 = (x+1 + off_x) * (1/zoom) * CHAR_WIDTH;
  int v2 = (y+1 + off_y) * (1/zoom) * CHAR_HEIGHT;
  
  if (image_sub_contained(img, u1, v1, u2, v1)) {
    
    color_t texel = image_sub_get(img, u1, v1, u2, v2);
    color_info_t info = color_info(colors_default, texel);
    
    char c = charset_image_sub_match(charset, texel, info, img, u1, v1, u2, v2);
    int c_id = charset_char_id(charset, c);
    
    if (charset.coverages[c_id] > 0.5) {
      return (c | A_BOLD | COLOR_PAIR(color_pair_index(info.secondary, info.primary)));
    } else {
      return (c | COLOR_PAIR(color_pair_index(info.secondary, info.primary)));
    }
    
  } else {
  
    return (' ' | COLOR_PAIR(0));
  
  }

}
