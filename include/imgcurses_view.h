#ifndef imgcurses_view_h
#define imgcurses_view_h

#include "imgcurses_image.h"

typedef struct {
  char* view_name;
  void (*view_func)(image_t img, charset_t charset, int x, int y, int off_x, int off_y, float zoom);
} view_t;

void view_func_solid(image_t img, charset_t charset, int x, int y, int off_x, int off_y, float zoom);
void view_func_value(image_t img, charset_t charset, int x, int y, int off_x, int off_y, float zoom);
void view_func_color(image_t img, charset_t charset, int x, int y, int off_x, int off_y, float zoom);
void view_func_detail(image_t img, charset_t charset, int x, int y, int off_x, int off_y, float zoom);

static view_t view_solid = {"Solid", view_func_solid};
static view_t view_value = {"Value", view_func_value};
static view_t view_color = {"Color", view_func_color};
static view_t view_detail = {"Detail", view_func_detail};

#endif
