#include "focus_mode.h"
#include "utils.h"

#include <stdio.h>
#include <SDL2/SDL_image.h>

Transform SAVED_OBJECT_TRANSFORM;
Focus_Input_Data INPUT_DATA;
int FOCUS_MODE = 0;
float FOCUS_POINT_X = 5;

void enter_focus_mode(Camera *camera, Scene *scene, int index_of_object, Focused_Object_Data *object)
{
    SDL_ShowCursor(SDL_ENABLE);
    SDL_SetRelativeMouseMode(SDL_FALSE);
    FOCUS_MODE = 1;
    INPUT_DATA.vec = malloc(sizeof(vec3));
    INPUT_DATA.norm_vec = malloc(sizeof(vec3));
    object->transform = scene->objects[index_of_object].transform;
    object->bounding_box = scene->objects[index_of_object].bounding_box;
    save_object_data(object);
    init_camera(camera);
    FOCUS_POINT_X = 5;
    float size_x = object->bounding_box->points[4].x - object->bounding_box->points[0].x;
    if (size_x > FOCUS_POINT_X)
    {
        FOCUS_POINT_X = size_x;
    }

    set_transform_position(object->transform, 5, 0, 0, object->bounding_box);
    for (int i = 0; i < scene->object_count; i++)
    {
        if (i != index_of_object)
        {
            scene->objects[i].draw_model = 0;
        }
    }
}

void exit_focus_mode(Scene *scene, Focused_Object_Data *object)
{
    FOCUS_MODE = 0;
    load_object_data(object);

    for (int i = 0; i < scene->object_count; i++)
    {
        scene->objects[i].draw_model = 1;
    }
}

void save_object_data(Focused_Object_Data *object)
{
    fill_v_v(&(SAVED_OBJECT_TRANSFORM.position), object->transform->position);
    fill_v_v(&(SAVED_OBJECT_TRANSFORM.rotation_angles), object->transform->rotation_angles);
    SAVED_OBJECT_TRANSFORM.scale = object->transform->scale;
}

void load_object_data(Focused_Object_Data *object)
{
    set_transform_position_v(object->transform, SAVED_OBJECT_TRANSFORM.position, object->bounding_box);
    set_transform_rotation_v(object->transform, SAVED_OBJECT_TRANSFORM.rotation_angles);
    set_transform_scale(object->transform, SAVED_OBJECT_TRANSFORM.scale);
}

int is_focus_active()
{
    return FOCUS_MODE;
}

void set_state_input(focused_state_input input)
{
    INPUT_DATA.input = input;
}

void set_state_xyz(focused_state_xyz xyz)
{
    INPUT_DATA.xyz = xyz;
    update_state();
}

void set_state_value(float value)
{
    INPUT_DATA.value = value;
    update_state();
}

void update_state()
{
    switch (INPUT_DATA.xyz)
    {
    case STATE_X:
        fill_v(INPUT_DATA.vec, INPUT_DATA.value, 0.0, 0.0);
        fill_v(INPUT_DATA.norm_vec, 1.0, 0.0, 0.0);
        break;
    case STATE_Y:
        fill_v(INPUT_DATA.vec, 0.0, INPUT_DATA.value, 0.0);
        fill_v(INPUT_DATA.norm_vec, 0.0, 1.0, 0.0);
        break;
    case STATE_Z:
        fill_v(INPUT_DATA.vec, 0.0, 0.0, INPUT_DATA.value);
        fill_v(INPUT_DATA.norm_vec, 0.0, 0.0, 1.0);
        break;
    default:
        break;
    }
}

void reset_object_by_state(Focused_Object_Data *object)
{
    switch (INPUT_DATA.input)
    {
    case STATE_SCALE:
        set_transform_scale(object->transform, 1);
        break;
    case STATE_ROTATE:
        set_transform_rotation(object->transform, 0, 0, 0);
        break;
    case STATE_MOVE:
        set_transform_position(object->transform, FOCUS_POINT_X, 0, 0, object->bounding_box);
        break;
    default:
        break;
    }
}

void update_object_by_state(Focused_Object_Data *object, Scene *scene)
{
    static float scale = 1;
    vec3 rotation_vector;
    switch (INPUT_DATA.input)
    {
    case STATE_SCALE:
        set_transform_scale(object->transform, scale + INPUT_DATA.value);
        scale += INPUT_DATA.value;
        break;
    case STATE_ROTATE:
        fill_v_v(&rotation_vector, *INPUT_DATA.vec);
        multiply_v(&rotation_vector, 180);
        rotate_transform_v(object->transform, rotation_vector);
        break;
    case STATE_MOVE:
        translate_transform_v(object->transform, *INPUT_DATA.vec, object->bounding_box);
        break;
    case STATE_LIGHT:
        set_lighting_1(scene, 0, INPUT_DATA.vec->x, INPUT_DATA.vec->y, INPUT_DATA.vec->z);
        break;
    default:
        break;
    }
}