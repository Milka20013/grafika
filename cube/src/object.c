#include "object.h"

#include <stdlib.h>

void init_object(Object *obj)
{
    obj->model = malloc(sizeof(Model));
    obj->transform = malloc(sizeof(Transform));
    obj->bounding_box = malloc(sizeof(Box));
    obj->draw_model = 1;
}

void fill_object(Object *obj)
{
    init_transform(obj->transform);
    init_bounding_box(obj->bounding_box, obj->model);
}