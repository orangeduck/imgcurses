#ifndef imgcurses_config_h
#define imgcurses_config_h

#include "imgcurses_color.h"


/*
** Color Configurations in order:
**
** BLACK RED GREEN YELLOW 
** BLUE MAGENTA CYAN WHITE
*/

static color_config_t colors_default = {
  {{  0,   0,   0}}, {{  0,   0, 255}}, 
  {{  0, 255,   0}}, {{  0, 255, 255}}, 
  {{255,   0,   0}}, {{255,   0, 255}}, 
  {{255, 255,   0}}, {{255, 255, 255}}
};

static color_config_t colors_ubuntu = {
  {{ 46,  52,  54}}, {{204,   0,   0}}, 
  {{78,   154,  6}}, {{196, 160,   0}}, 
  {{ 52, 101, 164}}, {{117,  80, 123}}, 
  {{  6, 152, 154}}, {{211, 215, 207}}
};


/* Pixel configurations */

typedef struct {
  int width, height;
} pixconf_t; 

static pixconf_t pixel_default = {7, 14};
static pixconf_t pixel_ubuntu = {7, 14};


#endif
