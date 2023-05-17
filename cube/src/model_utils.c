#include "model_utils.h"

#include <obj/load.h>
#include <obj/draw.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>

int count_models()
{
    DIR *d;
    struct dirent *dir;
    d = opendir("assets/models/");
    int i = 0;
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            if (is_obj(dir->d_name))
            {
                i++;
            }
        }
        closedir(d);
    }
    return i;
}

void load_models(Object *objects)
{
    DIR *d;
    struct dirent *dir;
    char models_directory[] = "assets/models/";
    d = opendir(models_directory);
    int i = 0;
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            if (is_obj(dir->d_name))
            {
                char full_file_name[200];
                strcpy(full_file_name, models_directory);
                strcat(full_file_name, dir->d_name);
                load_model(objects[i].model, full_file_name);

                // the name of the object will be the name of the file minus the .obj
                strcpy(objects[i].name, dir->d_name);
                int length = strlen(objects[i].name) - 4;
                objects[i].name[length] = 0;
                i++;
            }
        }
        closedir(d);
    }
}

void draw_models(Object *objects, int number_of_objects)
{
    // get the current matrix (which is not the identity) to reset after each object
    // so the objects will be independent from each other
    GLfloat matrix[16];
    glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
    Transform *curr_trans;
    for (int i = 0; i < number_of_objects; i++)
    {
        if (!objects[i].draw_model)
        {
            continue;
        }
        curr_trans = objects[i].transform;

        glTranslatef(curr_trans->position.x,
                     curr_trans->position.y,
                     curr_trans->position.z);
        glScalef(curr_trans->scale,
                 curr_trans->scale,
                 curr_trans->scale);
        glRotatef(curr_trans->angle,
                  curr_trans->rotation.x,
                  curr_trans->rotation.y,
                  curr_trans->rotation.z);
        glBindTexture(GL_TEXTURE_2D, objects[i].texture_id);
        draw_model(objects[i].model);
        // reset the matrix
        glLoadMatrixf(matrix);
    }
}