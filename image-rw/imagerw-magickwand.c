#define _CRT_SECURE_NO_DEPRECATE

#include "imagerw.h"
#include <MagickWand/MagickWand.h>
#include <string.h>

int load_image(const char *filename, ImageData *image) {
  MagickWand *mw = NULL;
  FILE *fp;
  char header[100];
  char tupltype[10];

  MagickWandGenesis();

  mw = NewMagickWand();

  if (mw == NULL) {
    MagickWandTerminus();
    return 0;
  }

  /* Read input image */
  if (MagickReadImage(mw, filename) == MagickFalse) {
    mw = DestroyMagickWand(mw);
    MagickWandTerminus();
    fprintf(stderr, "Error reading %s\n", filename);
    return 0;
  }

  /* Convert image to .pam format */
  MagickWriteImage(mw, "temp.pam");

  /* Clean up */
  if (mw)
    mw = DestroyMagickWand(mw);
  MagickWandTerminus();

  /* Load the .pam image into the ImageData structure */
  fp = fopen("temp.pam", "r");

  if (fp == NULL) {
    fprintf(stderr, "Error opening temporary PAM file\n");
    return 0;
  }

  /* Read the PAM header:
   * P7
   * WIDTH <width>
   * HEIGHT <height>
   * DEPTH 8
   * MAXVAL 255
   * TUPLETYPE RGB
   * ENDHDR
   */
  /* fgets read line by line */
  fgets(header, sizeof(header), fp);
  if (strncmp(header, "P7", 2) != 0) {
    fprintf(stderr, "Error: Not a PAM file\n");
    fclose(fp);
    return 0;
  }
  fgets(header, sizeof(header), fp);
  sscanf(header, "WIDTH %d", &image->width);
  fgets(header, sizeof(header), fp);
  sscanf(header, "HEIGHT %d", &image->height);
  fgets(header, sizeof(header), fp);
  sscanf(header, "DEPTH %d", &image->depth);
  fgets(header, sizeof(header), fp);
  sscanf(header, "MAXVAL %d", &image->maxval);
  fgets(header, sizeof(header), fp);
  sscanf(header, "TUPLETYPE %s", tupltype);
  image->channels = strlen(tupltype);
  fgets(header, sizeof(header), fp); /* Read the ENDHDR line */

  /* Print header data */
  printf("PAM Header:\n");
  printf("  Width: %d\n", image->width);
  printf("  Height: %d\n", image->height);
  printf("  Depth: %d\n", image->depth);
  printf("  Maxval: %d\n", image->maxval);
  printf("  Tupletype: %s\n", tupltype);
  printf("  Channels: %d\n", image->channels);

  return 1;
}

int save_image(const char *filename, const ImageData *image) { return 1; }

unsigned char get_image_col_byte(int pos, const ImageData *image) {

  return image->data[pos];
}

int set_image_col_byte(int pos, ImageData *image, unsigned char value) {
  return 0;
}

void cleanup_free_buffer(ImageData *image) { return; }
