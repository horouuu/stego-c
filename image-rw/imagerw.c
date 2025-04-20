#include "imagerw.h"

int load_image(const char *filename, ImageData *image) {
  image->data =
      stbi_load(filename, &image->width, &image->height, &image->channels, 0);
  if (image->data == NULL) {
    fprintf(stderr, "Error loading image: %s\n", stbi_failure_reason());
    return 0;
  }
  return 1;
}

int save_image(const char *filename, ImageData *image) {
  // Determine the file extension
  const char *str = strrchr(filename, '.');
  const char *ext = NULL;
  while (str != NULL) {
    ext = str + 1;
    str = strchr(ext, '.');
  }
  if (ext == NULL) {
    fprintf(stderr, "Error: No file extension found in filename\n");
    return 0;
  }
  if (strcmp(ext, "png") == 0) {
    if (!stbi_write_png(filename, image->width, image->height, image->channels,
                        image->data, 0)) {
      fprintf(stderr, "Error saving PNG image: %s\n", stbi_failure_reason());
      return 0;
    }
  } else if (strcmp(ext, "jpg") == 0 || strcmp(ext, "jpeg") == 0) {
    if (!stbi_write_jpg(filename, image->width, image->height, image->channels,
                        image->data, 100)) {
      fprintf(stderr, "Error saving JPG image: %s\n", stbi_failure_reason());
      return 0;
    }
  } else if (strcmp(ext, "bmp") == 0) {
    if (!stbi_write_bmp(filename, image->width, image->height, image->channels,
                        image->data)) {
      fprintf(stderr, "Error saving BMP image: %s\n", stbi_failure_reason());
      return 0;
    }
  } else {
    fprintf(stderr, "Error: Unsupported file format: %s\n", ext);
    return 0;
  }
}

unsigned char get_image_col_byte(int pos, ImageData *image) {
  if (pos < 0 || pos >= image->width * image->height * image->channels) {
    fprintf(stderr, "Error: Position out of bounds\n");
    return 0;
  }
  return image->data[pos];
}

int set_image_col_byte(int pos, ImageData *image, unsigned char value) {
  if (pos < 0 || pos >= image->width * image->height * image->channels) {
    fprintf(stderr, "Error: Position out of bounds\n");
    return 0;
  }
  image->data[pos] = value;
  return 1;
}

void cleanup_free_buffer(ImageData *image) {
  if (image->data) {
    stbi_image_free(image->data);
    image->data = NULL;
  }
}