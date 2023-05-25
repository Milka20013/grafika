#ifndef TEXTURE_H
#define TEXTURE_H
#include "object.h"

#include <GL/gl.h>

typedef GLubyte Pixel[3];

/**
 * Load texture from file and returns with the texture name.
 */
void generate_textures(char *filename, int size, GLuint *texture_names);

/**
 * Loads a texture, and return the index of it
 */
GLuint load_texture(char *filename);
/**
 * Loads all the textures from assets/textures and assigns it to the objects
 */
void load_textures(Object *objects, int number_of_objects);

/**
 * Checks if there's an object with the name of the param name (minus the file extension)
 * Return the index of the object, or -1 if not found
 */
int is_valid_texture_name(char *name, Object *objects, int number_of_objects);

#endif /* TEXTURE_H */
