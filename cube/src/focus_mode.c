#include "focus_mode.h"
#include "utils.h"

#include <stdio.h>
#include <SDL2/SDL_image.h>

Transform SAVED_OBJECT_TRANSFORM;
Focus_Input_Data INPUT_DATA;
int FOCUS_MODE = 0;

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
    fill_v(&(SAVED_OBJECT_TRANSFORM.position), object->transform->position.x, object->transform->position.y, object->transform->position.z);
    fill_v(&(SAVED_OBJECT_TRANSFORM.rotation), object->transform->rotation.x, object->transform->rotation.y, object->transform->rotation.z);
    SAVED_OBJECT_TRANSFORM.angle = object->transform->angle;
    SAVED_OBJECT_TRANSFORM.scale = object->transform->scale;
}

void load_object_data(Focused_Object_Data *object)
{
    set_transform_position(object->transform,
                           SAVED_OBJECT_TRANSFORM.position.x, SAVED_OBJECT_TRANSFORM.position.y, SAVED_OBJECT_TRANSFORM.position.z,
                           object->bounding_box);
    set_transform_rotation(object->transform,
                           SAVED_OBJECT_TRANSFORM.rotation.x,
                           SAVED_OBJECT_TRANSFORM.rotation.y,
                           SAVED_OBJECT_TRANSFORM.rotation.z,
                           0);
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

void update_object_by_state(Focused_Object_Data *object)
{
    static float scale = 1;
    switch (INPUT_DATA.input)
    {
    case STATE_SCALE:
        set_transform_scale(object->transform, scale + INPUT_DATA.value);
        scale += INPUT_DATA.value;
        break;
    case STATE_ROTATE:
        rotate_transform(object->transform, INPUT_DATA.norm_vec->x, INPUT_DATA.norm_vec->y, INPUT_DATA.norm_vec->z, INPUT_DATA.value * 180);
        break;
    case STATE_MOVE:
        translate_transform(object->transform, INPUT_DATA.vec->x, INPUT_DATA.vec->y, INPUT_DATA.vec->z, object->bounding_box);
        break;
    default:
        break;
    }
}