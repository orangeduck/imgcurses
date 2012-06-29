#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

#include "imgcurses_image.h"

void image_swap_red_blue(image_t img) {
  for(int x = 0; x < img.width; x++)
  for(int y = 0; y < img.height; y++) {
    color_t col = image_get(img, x, y);
    int r = col.r;
    col.r = col.b;
    col.b = r;
    image_set(img, x, y, col);
  }
}

void image_flip_vertical(image_t img) {

  int x, y;
  for (x = 0; x < img.width; x++)
  for (y = 0; y < img.height / 2; y++) {
     
     color_t top = image_get(img, x, y);
     color_t bottom = image_get(img, x, (img.height-1) - y);
     
     image_set(img, x, y, bottom);
     image_set(img, x, (img.height-1) - y, top);
  }

}

image_t image_load(const char* filename) {
  
  char* ext = strrchr(filename, '.');
  
  if ( ext == NULL || strcmp(ext, ".tga") != 0) {
    printf("Can only load .tga files!\n");
    exit(1);
  }
  
  FILE* f = fopen(filename, "rb");
	if (f == NULL) {
    printf("Cannot open file %s\n", filename);
    exit(1);
	}
	
  uint16_t width, height, depth;
	
	/* Seek to the width */
	fseek(f, 12, SEEK_SET);
	fread(&width, sizeof(uint16_t), 1, f);
	
	/* Seek to the height */
	fseek(f, 14, SEEK_SET);
	fread(&height, sizeof(uint16_t), 1, f);
	
	/* Seek to the depth */
	fseek(f, 16, SEEK_SET);
	fread(&depth, sizeof(uint16_t), 1, f);
  
  image_t img;
  img.width = width;
  img.height = height;
  img.data = malloc(height * width * sizeof(color_t));
  
	if (depth == 24) {
	  
	  fseek(f, 18, SEEK_SET);
	  fread(img.data, width * height * sizeof(color_t), 1, f);
  
	} else {
	  
    unsigned char* data = malloc(width * height * 4);
    
    fseek(f, 18, SEEK_SET);
    fread(data, width * height * 4, 1, f);
	  
	  /* Swap red blue */
	  for(int x = 0; x < width; x++)
	  for(int y = 0; y < height; y++) {
	    color_t c = color_new(data[x*4 + y*4*width + 0],
	                          data[x*4 + y*4*width + 1],
	                          data[x*4 + y*4*width + 2]);
	    image_set(img, x, y, c);
	  }
	  
	  free(data);
	}
  
  fclose(f);
  
  image_swap_red_blue(img);
  image_flip_vertical(img);
  
  return img;
}

void image_delete(image_t img) {
  free(img.data);
}

color_t image_get(image_t img, int u, int v) {
  if (u < 0) { return color_black(); }
  if (v < 0) { return color_black(); }
  if (u > img.width) { return color_black(); }
  if (v > img.height) { return color_black(); }
  return img.data[u + v * img.width];
}

void image_set(image_t img, int u, int v, color_t val) {
  if (u < 0) { return; }
  if (v < 0) { return; }
  if (u > img.width) { return; }
  if (v > img.height) { return; }
  img.data[u + v * img.width] = val;
}

bool image_sub_contained(image_t img, int u1, int v1, int u2, int v2) {
  
	return !( u1 > img.width || u2 < 0 || v1 > img.height || v2 < 0);
  
}

color_t image_sub_get(image_t img, int u1, int v1, int u2, int v2) {
  
  if ((u1 > img.width) && (v1 > img.height)) { return color_black(); }
  
  int r = 0;
  int g = 0;
  int b = 0;
  
  int count = 0;
  
  for(int x = u1; x < u2; x++)
  for(int y = v1; y < v2; y++) {
    color_t col = image_get(img, x, y);
    r += col.r; g += col.g; b += col.b;
    count++;
  }
  
  r /= count; g /= count; b /= count;
  
  return color_new(r, g, b); 
}

float image_filesize(image_t img) {
  
  int bytes = img.width * img.height * 4;
  return (float)bytes / 1000; 
  
}

