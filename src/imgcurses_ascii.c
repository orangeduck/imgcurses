#include <stdlib.h>
#include <stdio.h>
#include <math.h>

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

bool mask_get(mask_t mask, int u, int v) {
  if (u < 0) { return false; }
  if (v < 0) { return false; }
  if (u > mask.width) { return false; }
  if (v > mask.height) { return false; }
  return mask.data[u + v * mask.width];
}

void mask_print(mask_t mask) {
  for(int y = 0; y < mask.height; y++) {
    for(int x = 0; x < mask.width; x++) {
      if (mask_get(mask, x, y)) {
        printf("#");
      } else {
        printf(".");
      }
    }
    printf("\n");
  }
}

float mask_coverage(mask_t mask) {
  float total = 0;
  
  for (int x = 0; x < mask.width; x++)
  for (int y = 0; y < mask.height; y++) {
    if (mask.data[x + y * mask.width]) {
      total += 1;
    }
  }
  
  return total / (mask.width * mask.height);
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
  cs.coverages = malloc(sizeof(float) * cs.num_masks);
  
  int width = pixconf.width;
  int height = pixconf.height;
  
  float best_coverage = 0;
  
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
    
    cs.coverages[i] = mask_coverage(cs.masks[i]);
    best_coverage = cs.coverages[i] > best_coverage ? cs.coverages[i] : best_coverage; 
  }
  
  for(int i = 0; i < cs.num_masks; i++) {
    cs.coverages[i] /= best_coverage;
  }
  
  return cs;
}

void charset_delete(charset_t set) {
  for(int i = 0; i < set.num_masks; i++) {
    mask_delete(set.masks[i]);
  }
  
  free(set.masks);
  free(set.coverages);
}

char charset_char_for_coverage(charset_t set, float coverage) {
  
  int best_id = 0;
  float best_diff = 9999;
  
  for(int i = 0; i < set.num_masks; i++) {
    
    float diff = fabs(coverage - set.coverages[i]);
    
    if (diff < best_diff) {
      best_diff = diff;
      best_id = i;
    }
  }
  
  return charset_chars[best_id];
  
}

char charset_image_sub_match(charset_t set, color_t texel, color_info_t info, image_t img, int u1, int v1, int u2, int v2) {
  
  static const float constrast = 1.25;
  
  if (info.secondary_amount < 0.5) { return ' '; }
  if (info.secondary_amount > 0.9) { return '@'; }
  
  int sub_width = (u2 - u1);
  int sub_height = (v2 - v1);
  
  color_t pri_col = colors_ubuntu[info.primary];
  color_t snd_col = colors_ubuntu[info.secondary];
  
  int best_id = 1;
  float best_score = 0;
  
  for(int i = 0; i < set.num_masks; i++) {
    
    mask_t mask = set.masks[i];
    int score = 0;
    
    for(int x = 0; x < mask.width; x++)
    for(int y = 0; y < mask.height; y++) {
      
      float u_fac = (float)x / mask.width;
      float v_fac = (float)y / mask.height;
      
      color_t pix = image_get(img, u1 + u_fac * sub_width, v1 + v_fac * sub_height);
      
      bool mask_pix = mask_get(mask, x, y);
      bool pri_pix = (color_difference(pix, pri_col) * constrast) < color_difference(pix, snd_col);
      
      score += mask_pix ^ pri_pix;
    }
    
    if (score > best_score) {
      best_id = i;
      best_score = score;
    }
  }
  
  return charset_chars[best_id];

} 

