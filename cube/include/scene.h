#ifndef SCENE_H
#define SCENE_H

#include "camera.h"
#include "texture.h"
#include "object.h"

typedef struct Scene
{
    int object_count;
    Object *objects;
    Material material;
    Object *lights;
    int light_count;
} Scene;

/**
 * Initialize the scene by loading models.
 */
void init_scene(Scene *scene);

/**
 * Set the lighting of the scene.
 */
void set_lighting(float value);

/**
 * Set the point light of the focus mode scene.
 */
void set_lighting_1(Scene *scene, float value, float x, float y, float z);

/**
 * Set the current material.
 */
void set_material(const Material *material);

/**
 * Update the scene.
 */
void update_scene(Scene *scene, double delta_time, double current_time);

/**
 * Render the scene objects.
 */
void render_scene(Scene *scene);

/**
 * Draw the origin of the world coordinate system.
 */
void draw_origin();

#endif /* SCENE_H */
