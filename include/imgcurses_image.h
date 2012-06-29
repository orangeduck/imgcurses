#ifndef imgcurses_image_h
#define imgcurses_image_h

#include "imgcurses_color.h"

typedef struct {
  int width;
  int height;
  color_t* data;
} image_t;

image_t image_load(const char* filename);
void image_delete(image_t img);


void image_swap_red_blue(image_t img);
void image_flip_vertical(image_t img);

void image_set(image_t img, int x, int y, color_t val);
color_t image_get(image_t img, int x, int y);
color_t image_sub_get(image_t img, int u1, int v1, int u2, int v2);

bool image_sub_contained(image_t img, int u1, int v1, int u2, int v2);

float image_filesize(image_t img);

#endif
