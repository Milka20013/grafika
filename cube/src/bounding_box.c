#include "bounding_box.h"
#include "transform.h"
#include "utils.h"
#include <stdlib.h>
#include <stdio.h>

void init_bounding_box(Box *box, Model *model)
{
    // mins and maxes
    float x[] = {model->vertices[0].x, model->vertices[0].x};
    float y[] = {model->vertices[0].y, model->vertices[0].y};
    float z[] = {model->vertices[0].z, model->vertices[0].z};
    for (int i = 0; i < model->n_vertices + 1; i++)
    {
        min_v(&x[0], &y[0], &z[0], model->vertices[i].x, model->vertices[i].y, model->vertices[i].z);
        max_v(&x[1], &y[1], &z[1], model->vertices[i].x, model->vertices[i].y, model->vertices[i].z);
    }
    for (int i = 0; i < 2; i++)
    {
        fill_v(&(box->min_max[i]), x[i], y[i], z[i]);
        fill_v(&(box->base_min_max[i]), x[i], y[i], z[i]);
    }

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            fill_v(&(box->points[i + j * 4]), x[j], y[j], z[j]);
            fill_v(&(box->base_points[i + j * 4]), x[j], y[j], z[j]);
        }
    }
}

void update_bounding_box(Box *box, vec3 vector)
{
    vec3 *tmp;
    tmp = malloc(sizeof(vec3));
    fill_v(tmp, 0, 0, 0);
    for (int i = 0; i < 2; i++)
    {
        add_v_to_v(tmp, box->base_min_max[i]);
        add_v_to_v(tmp, vector);
        fill_v(&(box->min_max[i]), tmp->x, tmp->y, tmp->z);
        fill_v(tmp, 0, 0, 0);
    }
    for (int i = 0; i < 8; i++)
    {
        add_v_to_v(tmp, box->base_points[i]);
        add_v_to_v(tmp, vector);
        fill_v(&(box->points[i]), tmp->x, tmp->y, tmp->z);
        fill_v(tmp, 0, 0, 0);
    }
}

int point_is_in_box(vec3 point, Box *box)
{
    if (box->min_max[0].x <= point.x && box->min_max[1].x >= point.x)
    {
        if (box->min_max[0].y <= point.y && box->min_max[1].y >= point.y)
        {
            if (box->min_max[0].z <= point.z && box->min_max[1].z >= point.z)
            {
                return 1;
            }
        }
    }
    return 0;
}
