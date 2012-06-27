#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

#include "imgcurses_image.h"

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
	  for(int i = 0; i < width * height; i++) {
	    img.data[i].r = data[i*4+2];
	    img.data[i].g = data[i*4+1];
	    img.data[i].b = data[i*4+0];
	  }
	  
	  free(data);
	}
  
  fclose(f);
  
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


