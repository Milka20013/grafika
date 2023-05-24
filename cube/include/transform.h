#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "utils.h"
#include "bounding_box.h"

typedef struct Transform
{
    vec3 position;
    vec3 rotation_angles;
    float scale;
} Transform;

/*
    Create the transfrom with base values
*/
void init_transform(Transform *transform);

/*
    Rotate the transfrom by degrees in x,y,z
*/
void rotate_transform(Transform *transform, float x, float y, float z);

/*
    Rotate the transfrom by degrees in a vector
*/
void rotate_transform_v(Transform *transform, vec3 vec);

/*
    Move the transfrom in the direction, as much as the length of the direction and update the bounding box
*/
void translate_transform(Transform *transform, float x, float y, float z, Box *bounding_box);

/*
    Move the transfrom in the direction, as much as the length of the direction and update the bounding box
*/
void translate_transform_v(Transform *transform, vec3 vec, Box *bounding_box);

/*
    Override the transform's position and update the bounding box
*/
void set_transform_position(Transform *transform, float x, float y, float z, Box *bounding_box);

/*
    Override the transform's position and update the bounding box
*/
void set_transform_position_v(Transform *transform, vec3 vec, Box *bounding_box);

/*
    Override the transform's rotation
*/
void set_transform_rotation(Transform *transform, float x, float y, float z);

/*
    Override the transform's rotation with a vector
*/
void set_transform_rotation_v(Transform *transform, vec3 vec);

/*
    Override the transform's scale
*/
void set_transform_scale(Transform *transform, float scale);

#endif