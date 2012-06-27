#ifndef imgcurses_color_h
#define imgcurses_color_h

typedef union {
  struct {
    unsigned char r, g, b;
  };
  unsigned char rgb[3];
} color_t;

typedef color_t color_config_t[8];

color_t color_new(unsigned char r, unsigned char g, unsigned char b);
color_t color_black();

color_t color_add(color_t c1, color_t c2);
color_t color_sub(color_t c1, color_t c2);

color_t color_mul(color_t c1, float x);
color_t color_div(color_t c1, float x);

int color_difference(color_t c1, color_t c2);

int closest_color_index(color_config_t config, color_t c);

#endif
