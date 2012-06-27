#ifndef imgcurses_ascii_h
#define imgcurses_ascii_h

#include <stdbool.h>

#include "imgcurses_image.h"
#include "imgcurses_config.h"

typedef struct {
  int width;
  int height;
  bool* data;
} mask_t;

void mask_delete(mask_t mask);
void mask_set(mask_t mask, int u, int v, bool val);

static const int charset_char_count = 95;
static const char* charset_chars = ""
  " !\"#%&'()*+,-./0123456789:;<=>?@"
  "ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`"
  "abcdefghijklmnopqrstuvwxyz{|}~";

typedef struct {
  int num_masks;
  mask_t* masks;
} charset_t;

charset_t charset_load(image_t setimg, pixconf_t pixconf);
void charset_delete(charset_t set);


#endif
