#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>

#include <png.h>
#include <jpeglib.h>

#include "imgcurses.h"

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

static image_t image_load_jpeg(const char* filename) {
  
	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;
	
	JSAMPROW row_pointer[1];
	
  FILE* f = fopen(filename, "rb");
	if (f == NULL) {
    printf("Cannot open file %s\n", filename);
    exit(1);
	}
	
	cinfo.err = jpeg_std_error(&jerr);
  
	jpeg_create_decompress(&cinfo);
	jpeg_stdio_src(&cinfo, f);
	jpeg_read_header(&cinfo, TRUE);
	
	int width = cinfo.image_width;
	int height = cinfo.image_height;
	int channels = cinfo.num_components;
	
	jpeg_start_decompress(&cinfo);

	unsigned char* raw_image = malloc(width * height * channels);
	row_pointer[0] = malloc(width * 3);
	
	int loc = 0;
	while (cinfo.output_scanline < height) {
    jpeg_read_scanlines(&cinfo, (JSAMPARRAY)row_pointer, 1);
    for(int x=0; x < width * channels; x++) {
      raw_image[loc++] = row_pointer[0][x];
    }
	}
	
	jpeg_finish_decompress(&cinfo);
	jpeg_destroy_decompress(&cinfo);
	free(row_pointer[0]);
	
	fclose(f);
  
  image_t img;
  img.width = width;
  img.height = height;
  img.data = (color_t*)raw_image;
  
  return img;
}

static image_t image_load_png(const char* filename) {
  
  FILE* f = fopen(filename, "rb");
	if (f == NULL) {
    printf("Cannot open file %s\n", filename);
    exit(1);
	}
	
	unsigned char header[8];
  fread(header, 1, 8, f);
  
  if (png_sig_cmp(header, 0, 8)) {
    printf("Error loading file %s. Bad header.\n", filename);
    exit(1);
  }
  
  png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);;
  png_infop info_ptr = png_create_info_struct(png_ptr);
  
  png_init_io(png_ptr, f);
  png_set_sig_bytes(png_ptr, 8);
  png_read_info(png_ptr, info_ptr);

  int width = png_get_image_width(png_ptr, info_ptr);
  int height = png_get_image_height(png_ptr, info_ptr);
  int color_type = png_get_color_type(png_ptr, info_ptr);
  int bit_depth = png_get_bit_depth(png_ptr, info_ptr);
  
  int num_passes = png_set_interlace_handling(png_ptr);
  png_read_update_info(png_ptr, info_ptr);

  png_bytep* row_data = malloc(sizeof(png_bytep) * height);
  for (int y=0; y<height; y++) {
    row_data[y] = malloc(png_get_rowbytes(png_ptr,info_ptr));
  }
  
  png_read_image(png_ptr, row_data);

  fclose(f);
  
  image_t img;
  img.width = width;
  img.height = height;
  img.data = malloc(width * height * sizeof(color_t));
  
  int channels;
  if (png_get_color_type(png_ptr, info_ptr) == PNG_COLOR_TYPE_RGBA) {
    channels = 4;
  } else if (png_get_color_type(png_ptr, info_ptr) == PNG_COLOR_TYPE_RGB) {
    channels = 3;
  } else {
    printf("Error loading file %s. Unknown color type.\n", filename);
    exit(1);
  }
  
  for (int y=0; y < height; y++) {
    for (int x=0; x < width; x++) {
      png_byte* row = row_data[y];
      png_byte* ptr = &(row[x*channels]);
      image_set(img, x, y, color_new(ptr[0], ptr[1], ptr[2]));
    }
  }
  
  for (int y=0; y<height; y++) {
    free(row_data[y]);
  }
  free(row_data);
  
  return img;
  
}

static image_t image_load_tga(const char* filename) {
  

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
  img.data = malloc(width * height * sizeof(color_t));
  
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

image_t image_load(const char* filename) {

  char* ext = strrchr(filename, '.');
  
  if (ext == NULL) {
    printf("Cannot load file %s\n", filename);
    exit(1);
  }
  
  if (strcmp(ext, ".tga") == 0) { return image_load_tga(filename); }
  if (strcmp(ext, ".png") == 0) { return image_load_png(filename); }
  if (strcmp(ext, ".jpeg") == 0) { return image_load_jpeg(filename); }
  if (strcmp(ext, ".jpg") == 0) { return image_load_jpeg(filename); }
  
  printf("Cannot load file %s. Unknown file extension %s.\n", filename, ext);
  exit(1);

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

