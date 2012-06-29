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
bool mask_get(mask_t mask, int u, int v);

void mask_print(mask_t mask);

float mask_coverage(mask_t mask);

static const int charset_char_count = 95;
static const char* charset_chars = ""
  " !\"#$%&'()*+,-./0123456789:;<=>?@"
  "ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`"
  "abcdefghijklmnopqrstuvwxyz{|}~";

typedef struct {
  int num_masks;
  float* coverages;
  mask_t* masks;
} charset_t;

charset_t charset_load(image_t setimg, pixconf_t pixconf);
void charset_delete(charset_t set);

char charset_char_for_coverage(charset_t set, float coverage);

char charset_image_sub_match(charset_t set, color_t texel, color_info_t info, image_t img, int u1, int v1, int u2, int v2);

#endif
