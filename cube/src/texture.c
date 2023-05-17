#include "texture.h"
#include "model_utils.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

GLuint load_texture(char *filename)
{
    SDL_Surface *surface;
    GLuint texture_name;

    surface = IMG_Load(filename);

    glGenTextures(1, &texture_name);
    glBindTexture(GL_TEXTURE_2D, texture_name);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, surface->w, surface->h, 0, GL_RGB, GL_UNSIGNED_BYTE, (Pixel *)(surface->pixels));

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return texture_name;
}

void load_textures(Object *objects, int number_of_objects)
{
    DIR *d;
    struct dirent *dir;
    char textures_directory[] = "assets/textures/";
    d = opendir(textures_directory);
    if (d)
    {
        while ((dir = readdir(d)) != NULL)
        {
            int texture_bind_index = is_valid_texture_name(dir->d_name, objects, number_of_objects);
            if (texture_bind_index != -1)
            {
                char full_file_name[200];
                strcpy(full_file_name, textures_directory);
                strcat(full_file_name, dir->d_name);
                objects[texture_bind_index].texture_id = load_texture(full_file_name);
                printf(" %s ", full_file_name);
            }
        }
        closedir(d);
    }
}

int is_valid_texture_name(char *name, Object *objects, int number_of_objects)
{
    for (int i = 0; i < number_of_objects; i++)
    {
        if (strstr(name, objects[i].name) != NULL)
        {
            return i;
        }
    }
    return -1;
}
