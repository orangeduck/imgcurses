#ifndef imgcurses_h
#define imgcurses_h

#include <stdbool.h>

/*
** Constants
*/

#define CHAR_HEIGHT 14
#define CHAR_WIDTH 7

/*
** Colors
*/

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

bool color_equ(color_t c1, color_t t2);

int color_difference(color_t c1, color_t c2);

typedef struct {
  int primary;
  int secondary;
  float secondary_amount;
  float value;
} color_info_t;

color_info_t color_info(color_t c);

/*
** Color Configurations in order:
**
** BLACK RED GREEN YELLOW 
** BLUE MAGENTA CYAN WHITE
*/

static color_config_t colors_default = {
  {{ 46,  52,  54}}, {{204,   0,   0}}, 
  {{78,   154,  6}}, {{196, 160,   0}}, 
  {{ 52, 101, 164}}, {{117,  80, 123}}, 
  {{  6, 152, 154}}, {{211, 215, 207}}
};

/*
** Images
*/

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

/*
** Ascii
*/

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

charset_t charset_load(image_t setimg);
void charset_delete(charset_t set);

int charset_char_id(charset_t set, char c);
char charset_char_for_coverage(charset_t set, float coverage);

char charset_image_sub_match(charset_t set, color_t texel, color_info_t info, image_t img, int u1, int v1, int u2, int v2);

/*
** Views
*/

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

/*
** State
*/

static float zoom = 1.0;
static int offset_x = 0;
static int offset_y = 0;

static image_t img = {0};
static image_t img_charset = {0};
static charset_t charset = {0};

static int view_index = 0;

#define VIEW_COUNT 4
static view_t views_list[VIEW_COUNT];



#endif
