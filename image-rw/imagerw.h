#ifndef IMAGERW_H
#define IMAGERW_H

#include <stdio.h>

typedef struct {
  unsigned char *data;
  int width;
  int height;
  int depth;
  int maxval;
  int channels;
} ImageData;

/**
 * Loads an image from a file and returns an ImageData structure containing the image information.
 * The function returns NULL if the image could not be loaded.
 *
 * The function supports PNG, JPG/JPEG, BMP, PSD, TGA, GIF, PIC, PNM (.ppm and .pgm) formats based on the file extension. 
 *
 * @param[in] filename The name of the image file to load.
 * @param[out] image A pointer to an ImageData structure that will be filled with the image information.
 * @return 1 if the image was loaded successfully, 0 otherwise.
 */
extern int load_image(const char *filename, ImageData *image);

/**
 * Saves an image to a file using the specified format.
 * The function returns true if the image was saved successfully, false otherwise.
 *
 * The function supports PNG, JPG/JPEG, BMP formats based on the file extension.
 *
 * @param[in] filename The name of the file to save the image to.
 * @param[in] image The image data structure containing the image information.
 * @return 1 if the image was saved successfully, 0 otherwise.
 */
extern int save_image(const char *filename, const ImageData *image);

/**
 * Gets the pixel color at the specified position in the image.
 * The position is specified as a linear index (0 to width*height-1).
 * The function returns the color value as an unsigned char.
 *
 * @param[in] pos The position in the image data array in linear index.
 * @param[in] image The image data structure containing the image information.
 * @return The color value at the specified position in the image data array. Returns NULL if the position is out of bounds.
 */
extern unsigned char get_image_col_byte(int pos, const ImageData *image);

/**
 * Sets the pixel color at the specified position in the image.
 * The position is specified as a linear index (0 to width*height-1).
 * The function returns true if the operation was successful, false otherwise.
 * 
 * @param[in] pos The position in the image data array in linear index.
 * @param[in,out] image The image data structure containing the image information.
 * @param[in] value The color value to set at the specified position in the image data array.
 * @return 1 if the operation was successful, 0 otherwise.
 */
extern int set_image_col_byte(int pos, ImageData *image, unsigned char value);

/**
 * Cleans up and frees the image data buffer.
 * This function should be called when the image is no longer needed.
 *
 * @param[in,out] image The image data structure containing the image information.
 */
extern void cleanup_free_buffer(ImageData *image);

#endif /* IMAGERW_H */