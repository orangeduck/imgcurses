#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include "imgcurses_color.h"

color_t color_new(unsigned char r, unsigned char g, unsigned char b) {
  color_t col;
  col.r = r;
  col.g = g;
  col.b = b;
  return col;
}

color_t color_black() {
  color_t col;
  col.r = 0;
  col.g = 0;
  col.b = 0;
  return col;
}

color_t color_add(color_t c1, color_t c2) {
  color_t col;
  col.r = c1.r + c2.r;
  col.g = c1.g + c2.g;
  col.b = c1.b + c2.b;
  return col;
}

color_t color_sub(color_t c1, color_t c2) {
  color_t col;
  col.r = c1.r - c2.r;
  col.g = c1.g - c2.g;
  col.b = c1.b - c2.b;
  return col;
}

color_t color_mul(color_t c1, float x) {
  color_t col;
  col.r = c1.r * x;
  col.g = c1.g * x;
  col.b = c1.b * x;
  return col;
}

color_t color_div(color_t c1, float x) {
  color_t col;
  col.r = c1.r / x;
  col.g = c1.g / x;
  col.b = c1.b / x;
  return col;
}

int color_difference(color_t c1, color_t c2) {
  return abs(c1.r - c2.r) + 
         abs(c1.g - c2.g) + 
         abs(c1.b - c2.b);
}

int closest_color_index(color_config_t config, color_t color) {
  
  int best_diff = 9999;
  int best_index = 0;
  
  for (int i = 0; i < 8; i++) {
    
    int diff = color_difference(color, config[i]);
    
    if (diff < best_diff) {
      best_diff = diff;
      best_index = i;
    }
  }
  
  return best_index;
  
}


