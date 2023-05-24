#include "utils.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include <stdlib.h>
#include <string.h>

double degree_to_radian(double degree)
{
	return degree * M_PI / 180.0;
}

int sign(float value)
{
	if (value < 0)
	{
		return -1;
	}
	else if (value > 0)
	{
		return 1;
	}
	return 0;
}

int is_obj(char *file_name)
{
	if (strstr(file_name, ".obj") == NULL)
	{
		return 0;
	}
	return 1;
}

void min_v(float *x_min, float *y_min, float *z_min, float x, float y, float z)
{
	if (*x_min > x)
	{
		*x_min = x;
	}
	if (*y_min > y)
	{
		*y_min = y;
	}
	if (*z_min > z)
	{
		*z_min = z;
	}
}

void max_v(float *x_max, float *y_max, float *z_max, float x, float y, float z)
{
	if (*x_max < x)
	{
		*x_max = x;
	}
	if (*y_max < y)
	{
		*y_max = y;
	}
	if (*z_max < z)
	{
		*z_max = z;
	}
}

void fill_v(vec3 *vector, float x, float y, float z)
{
	vector->x = x;
	vector->y = y;
	vector->z = z;
}

void fill_v_v(vec3 *vector, vec3 fill_with_v)
{
	vector->x = fill_with_v.x;
	vector->y = fill_with_v.y;
	vector->z = fill_with_v.z;
}

void add_to_v(vec3 *vector, float x, float y, float z)
{
	vector->x += x;
	vector->y += y;
	vector->z += z;
}

void multiply_v(vec3 *vector, float value)
{
	vector->x *= value;
	vector->y *= value;
	vector->z *= value;
}

void add_v_to_v(vec3 *vector1, vec3 vector2)
{
	vector1->x += vector2.x;
	vector1->y += vector2.y;
	vector1->z += vector2.z;
}

float p2_dist(float x1, float y1, float x2, float y2)
{
	return sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
}