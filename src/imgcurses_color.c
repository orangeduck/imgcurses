#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include "imgcurses.h"

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

bool color_equ(color_t c1, color_t c2) {
  return (c1.r == c2.r) && (c1.g == c2.g) && (c1.b == c2.b);
}

static float blend_amount(color_t actual, color_t primary, color_t secondary) {
  
  float primary_diff = color_difference(actual, primary);
  float secondary_diff = color_difference(actual, secondary);
  
  return primary_diff / secondary_diff;
  
}

color_info_t color_info(color_t c) {
  
  int primary_diff = 9999;
  int primary_index = 0;
  
  for (int i = 0; i < 8; i++) {
    
    int diff = color_difference(c, colors_default[i]);
    
    if (diff < primary_diff) {
      primary_diff = diff;
      primary_index = i;
    }
  }
  
  int secondary_diff = 9999;
  int secondary_index = 0;
  
  for (int i = 0; i < 8; i++) {
    
    if (i == primary_index) continue;
    
    int diff = color_difference(c, colors_default[i]);
    
    if (diff < secondary_diff) {
      secondary_diff = diff;
      secondary_index = i;
    }
  }
  
  color_info_t info;
  info.primary = primary_index;
  info.secondary = secondary_index;
  
  float value = 0;
  value += c.r; value += c.g; value += c.b;
  value /= (3 * 255);
  
  info.value = value;
  info.secondary_amount = blend_amount(c, colors_default[primary_index], colors_default[secondary_index]);
  
  return info;
  
}


