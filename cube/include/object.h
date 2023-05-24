#ifndef OBJECT_H
#define OBJECT_H

#include "transform.h"
#include <obj/model.h>
#include <GL/gl.h>

typedef struct Object
{
    char name[64];
    Model *model;
    Transform *transform;
    Box *bounding_box;
    GLuint texture_id;
    int draw_model;
} Object;

/**
 * Initialize an empty Object
 */
void init_object(Object *obj);

/**
 * Fill an object with a base value transform and bounding box (an OBJ should be fed to the model before this)
 */
void fill_object(Object *obj);

#endif