#ifndef IMAGERW_H
#define IMAGERW_H

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

typedef struct {
  unsigned char *data;
  int width;
  int height;
  int channels;
} ImageData;

/**
 * Loads an image from a file and returns an ImageData structure containing the image information.
 * The function returns NULL if the image could not be loaded.
 *
 * @param[in] filename The name of the image file to load.
 * @param[out] image A pointer to an ImageData structure that will be filled with the image information.
 * @return True if the image was loaded successfully, false otherwise.
 */
bool load_image(const char *filename, ImageData *image);

/**
 * Saves an image to a file using the specified format.
 * The function returns true if the image was saved successfully, false otherwise.
 *
 * @param[in] filename The name of the file to save the image to.
 * @param[in] image The image data structure containing the image information.
 * @return True if the image was saved successfully, false otherwise.
 */
bool save_image(const char *filename, ImageData *image);

/**
 * Gets the pixel color at the specified position in the image.
 * The position is specified as a linear index (0 to width*height-1).
 * The function returns the color value as an unsigned char.
 *
 * @param[in] pos The position in the image data array in linear index.
 * @param[in] image The image data structure containing the image information.
 * @return The color value at the specified position in the image data array. Returns NULL if the position is out of bounds.
 */
unsigned char get_image_col_byte(int pos, ImageData *image);

/**
 * Sets the pixel color at the specified position in the image.
 * The position is specified as a linear index (0 to width*height-1).
 * The function returns true if the operation was successful, false otherwise.
 * 
 * @param[in] pos The position in the image data array in linear index.
 * @param[in] image The image data structure containing the image information.
 * @param[in] value The color value to set at the specified position in the image data array.
 * @return true if the operation was successful, false otherwise.
 */
bool set_image_col_byte(int pos, ImageData *image, unsigned char value);

#endif // IMAGERW_H