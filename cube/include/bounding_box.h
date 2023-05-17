#ifndef BOUNDING_BOX_H
#define BOUNDING_BOX_H

#include "utils.h"
#include <obj/model.h>
/*
        7-------6
       /|      /|
      4-+-----5 |
      | |     | |   y
      | 3-----+-2   | z
      |/      |/    |/
      0-------1     +--x
      source of this cube: https://poita.org/2014/04/27/cube-vertex-numbering.html
*/
typedef struct Box
{
    vec3 base_points[8];
    vec3 base_min_max[2];
    vec3 points[8];
    vec3 min_max[2];
} Box;

/*
    Initializes a bounding box according to the model's vertices
*/
void init_bounding_box(Box *box, Model *model);

/*
    Updates a bounding box according to the vector's position
*/
void update_bounding_box(Box *box, vec3 vector);

/*
    Determines if the given point is in the bounding box
*/
int point_is_in_box(vec3 point, Box *box);

#endif