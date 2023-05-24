#ifndef MODEL_UTILS_H
#define MODEL_UTILS_H
#include "object.h"
#include <obj/load.h>
#include <obj/draw.h>
/**
 * Loads all the models and their names from assets/models folder to the objects array
 */
void load_models(Object *objects);

/**
 * Draws all the models of the object array
 */
void draw_models(Object *objects, int number_of_objects);

/**
 * Counts all the models in the models folder
 */
int count_models();

#endif