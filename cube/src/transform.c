#include "transform.h"

#include <obj/model.h>
#include <GL/gl.h>
#include <stdlib.h>

void init_transform(Transform *transform)
{
    fill_v(&(transform->position), 0, 0, 0);
    fill_v(&(transform->rotation_angles), 0, 0, 0);
    transform->scale = 1;
}

void rotate_transform(Transform *transform, float x, float y, float z)
{
    add_to_v(&(transform->rotation_angles), x, y, z);
}

void rotate_transform_v(Transform *transform, vec3 vec)
{
    add_v_to_v(&(transform->rotation_angles), vec);
}

void translate_transform(Transform *transform, float x, float y, float z, Box *bounding_box)
{
    add_to_v(&(transform->position), x, y, z);
    update_bounding_box(bounding_box, transform->position);
}

void translate_transform_v(Transform *transform, vec3 vec, Box *bounding_box)
{
    add_v_to_v(&(transform->position), vec);
    update_bounding_box(bounding_box, transform->position);
}

void set_transform_position(Transform *transform, float x, float y, float z, Box *bounding_box)
{
    fill_v(&(transform->position), x, y, z);
    update_bounding_box(bounding_box, transform->position);
}

void set_transform_position_v(Transform *transform, vec3 vec, Box *bounding_box)
{
    fill_v_v(&(transform->position), vec);
    update_bounding_box(bounding_box, transform->position);
}

void set_transform_rotation_v(Transform *transform, vec3 vec)
{
    fill_v_v(&(transform->rotation_angles), vec);
}

void set_transform_rotation(Transform *transform, float x, float y, float z)
{
    fill_v(&(transform->rotation_angles), x, y, z);
}

void set_transform_scale(Transform *transform, float scale)
{
    transform->scale = scale;
}
