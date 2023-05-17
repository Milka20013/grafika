#include "transform.h"

#include <obj/model.h>
#include <GL/gl.h>
#include <stdlib.h>

void init_transform(Transform *transform)
{
    fill_v(&(transform->position), 0, 0, 0);
    fill_v(&(transform->rotation), 0, 0, 0);
    transform->scale = 1;
}

void rotate_transform(Transform *transform, float x, float y, float z, float angle)
{
    fill_v(&(transform->rotation), x, y, z);
    transform->angle += angle;
}

void translate_transform(Transform *transform, float x, float y, float z, Box *bounding_box)
{
    add_to_v(&(transform->position), x, y, z);
    update_bounding_box(bounding_box, transform->position);
}

void set_transform_position(Transform *transform, float x, float y, float z, Box *bounding_box)
{
    fill_v(&(transform->position), x, y, z);
    update_bounding_box(bounding_box, transform->position);
}

void set_transform_rotation(Transform *transform, float x, float y, float z, float angle)
{
    fill_v(&(transform->rotation), x, y, z);
    transform->angle = angle;
}

void set_transform_scale(Transform *transform, float scale)
{
    transform->scale = scale;
}
