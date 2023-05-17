#include "scene.h"
#include "utils.h"
#include "model_utils.h"

#include <stdlib.h>
#include <GL/gl.h>

void init_scene(Scene *scene)
{

    // loading all the models and initializing transforms and bounding boxes to all of them
    int object_count = count_models();
    scene->objects = malloc(sizeof(Object) * object_count);
    scene->object_count = object_count;
    for (int i = 0; i < object_count; i++)
    {
        init_object(&(scene->objects[i]));
    }
    load_models(scene->objects);
    load_textures(scene->objects, scene->object_count);
    for (int i = 0; i < object_count; i++)
    {
        fill_object(&(scene->objects[i]));
    }

    // positioning the objects on a diagonal
    for (int i = 0; i < object_count; i++)
    {
        set_transform_position(scene->objects[i].transform, i * 2, i * 2, 0, scene->objects[i].bounding_box);
    }

    scene->material.ambient.red = 0.0;
    scene->material.ambient.green = 0.0;
    scene->material.ambient.blue = 0.0;

    scene->material.diffuse.red = 1.0;
    scene->material.diffuse.green = 1.0;
    scene->material.diffuse.blue = 1.0;

    scene->material.specular.red = 0.0;
    scene->material.specular.green = 0.0;
    scene->material.specular.blue = 0.0;

    scene->material.shininess = 0.0;
}

void set_lighting()
{
    // light0 is a global light

    float ambient_light[] = {0.0f, 0.0f, 0.0f, 1.0f};
    float diffuse_light[] = {1.0f, 1.0f, 1.0, 1.0f};
    float specular_light[] = {0.0f, 0.0f, 0.0f, 1.0f};
    float position[] = {0.0f, 0.0f, 10.0f, 1.0f};

    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient_light);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse_light);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular_light);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
}

void set_material(const Material *material)
{
    // set a material of a model

    float ambient_material_color[] = {
        material->ambient.red,
        material->ambient.green,
        material->ambient.blue};

    float diffuse_material_color[] = {
        material->diffuse.red,
        material->diffuse.green,
        material->diffuse.blue};

    float specular_material_color[] = {
        material->specular.red,
        material->specular.green,
        material->specular.blue};

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient_material_color);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse_material_color);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular_material_color);

    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &(material->shininess));
}

void update_scene(Scene *scene, double delta_time, double current_time)
{
}

void render_scene(const Scene *scene)
{
    set_material(&(scene->material));
    set_lighting();
    draw_origin();
    draw_models(scene->objects, scene->object_count);
}

void draw_origin()
{
    glBegin(GL_LINES);

    glColor3f(1, 0, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(1, 0, 0);

    glColor3f(0, 1, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 1, 0);

    glColor3f(0, 0, 1);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, 1);

    glEnd();
}