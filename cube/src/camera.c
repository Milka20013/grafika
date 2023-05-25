#include "camera.h"

#include <GL/gl.h>

#include <math.h>

void init_camera(Camera *camera)
{
    fill_v(&(camera->position), 0.0, 0.0, 1.0);
    fill_v(&(camera->rotation), 0.0, 0.0, 0.0);
    fill_v(&(camera->speed), 0.0, 0.0, 0.0);

    camera->is_preview_visible = false;
    camera->global_speed = 1.0f;
}

void update_camera(Camera *camera, double time)
{
    double angle;
    double side_angle;

    angle = degree_to_radian(camera->rotation.z);
    side_angle = degree_to_radian(camera->rotation.z + 90.0);

    camera->position.x += cos(angle) * camera->speed.y * camera->global_speed * time;
    camera->position.y += sin(angle) * camera->speed.y * camera->global_speed * time;
    camera->position.x += cos(side_angle) * camera->speed.x * camera->global_speed * time;
    camera->position.y += sin(side_angle) * camera->speed.x * camera->global_speed * time;

    camera->position.z += time * camera->speed.z * camera->global_speed;
}

void set_view(const Camera *camera)
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glRotatef(-(camera->rotation.x + 90), 1.0, 0, 0);
    glRotatef(-(camera->rotation.z - 90), 0, 0, 1.0);
    glTranslatef(-camera->position.x, -camera->position.y, -camera->position.z);
}

void rotate_camera(Camera *camera, double horizontal, double vertical)
{
    camera->rotation.z += horizontal;
    camera->rotation.x += vertical;

    if (camera->rotation.z < 0)
    {
        camera->rotation.z += 360.0;
    }

    if (camera->rotation.z > 360.0)
    {
        camera->rotation.z -= 360.0;
    }

    if (camera->rotation.x < 0)
    {
        camera->rotation.x += 360.0;
    }

    if (camera->rotation.x > 360.0)
    {
        camera->rotation.x -= 360.0;
    }
}

void set_camera_speed(Camera *camera, double speed)
{
    camera->speed.y = speed;
}

void set_camera_side_speed(Camera *camera, double speed)
{
    camera->speed.x = speed;
}

void set_camera_vertical_speed(Camera *camera, double speed)
{
    camera->speed.z = speed;
}

void multiply_camera_speed(Camera *camera, float value)
{
    camera->global_speed = value;
}


