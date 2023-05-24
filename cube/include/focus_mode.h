#ifndef FOCUS_MODE_H
#define FOCUS_MODE_H

#include <obj/model.h>
#include "scene.h"

typedef struct Focused_Object_Data
{
    Transform *transform;
    Box *bounding_box;
} Focused_Object_Data;

typedef enum focused_state_xyz
{
    STATE_X,
    STATE_Y,
    STATE_Z
} focused_state_xyz;

typedef enum focused_state_input
{
    STATE_ROTATE,
    STATE_MOVE,
    STATE_SCALE,
    STATE_LIGHT,
    STATE_NONE
} focused_state_input;

typedef struct Focus_Input_Data
{
    focused_state_xyz xyz;
    focused_state_input input;
    vec3 *vec;
    vec3 *norm_vec;
    float value;
} Focus_Input_Data;
/**
 * Enters focus mode, disables every object except the focused one and input handling changes
 */
void enter_focus_mode(Camera *camera, Scene *scene, int index_of_object, Focused_Object_Data *object);

/**
 * Exits focus mode, back to normal behaviour
 */
void exit_focus_mode(Scene *scene, Focused_Object_Data *object);

/**
 * Stores the focused object's tranform data so that it can revert back to its original state
 */
void save_object_data(Focused_Object_Data *object);

/**
 * Loads the focused object's transform data
 */
void load_object_data(Focused_Object_Data *object);

/**
 * Return the status of the focus
 */
int is_focus_active();

/**
 * Set the current focus input state
 */
void set_state_input(focused_state_input input);

/**
 * Set the current focus xyz state
 */
void set_state_xyz(focused_state_xyz xyz);

/**
 * Set the current value to the focus data
 */
void set_state_value(float value);

/**
 * Updates the vectors in the state data based on xyz state and value
 */
void update_state();

/**
 * Resets the selected input state to 0
 */
void reset_object_by_state(Focused_Object_Data *object);

/**
 * Updates the object by the global focus state data
 */
void update_object_by_state(Focused_Object_Data *object, Scene *scene);

#endif