#include <stdlib.h>
#include <stdio.h>

#include "imgcurses_ascii.h"

void mask_delete(mask_t mask) {
  free(mask.data);
}

void mask_set(mask_t mask, int u, int v, bool val) {
  if (u < 0) { return; }
  if (v < 0) { return; }
  if (u > mask.width) { return; }
  if (v > mask.height) { return; }
  mask.data[u + v * mask.width] = val;
}

charset_t charset_load(image_t img, pixconf_t pixconf) {
  
  if (img.width != charset_char_count * pixconf.width) {
    printf("Badly formed charset image width\n");
    exit(1);
  }
  
  if (img.height != pixconf.height) {
    printf("Badly formed charset image height\n");
    exit(1);
  }
  
  charset_t cs;
  cs.num_masks = charset_char_count;
  cs.masks = malloc(sizeof(mask_t) * cs.num_masks);
  
  int width = pixconf.width;
  int height = pixconf.height;
  
  for(int i = 0; i < cs.num_masks; i++) {
    cs.masks[i].width = width;
    cs.masks[i].height = height;
    cs.masks[i].data = malloc(sizeof(bool) * width * height);
    
    for(int x = 0; x < width; x++)
    for(int y = 0; y < height; y++) {
      color_t col = image_get(img, (i*width) + x, y);
      if (col.r > 64) {
        mask_set(cs.masks[i], x, y, true);
      } else {
        mask_set(cs.masks[i], x, y, false);
      }
    }
    
  }
  
  return cs;
}

void charset_delete(charset_t set) {
  for(int i = 0; i < set.num_masks; i++) {
    mask_delete(set.masks[i]);
  }
  
  free(set.masks);
}

