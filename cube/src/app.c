#include "app.h"
#include "utils.h"
#include "bounding_box.h"
#include "focus_mode.h"

#include <stdio.h>
#include <SDL2/SDL_image.h>
#include <GL/gl.h>

void init_app(App *app, int width, int height)
{
    int error_code;
    int inited_loaders;

    app->is_running = false;

    error_code = SDL_Init(SDL_INIT_EVERYTHING);
    if (error_code != 0)
    {
        printf("[ERROR] SDL initialization error: %s\n", SDL_GetError());
        return;
    }

    app->window = SDL_CreateWindow(
        "Grafika Beadandó",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        width, height,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if (app->window == NULL)
    {
        printf("[ERROR] Unable to create the application window!\n");
        return;
    }

    inited_loaders = IMG_Init(IMG_INIT_PNG);
    if (inited_loaders == 0)
    {
        printf("[ERROR] IMG initialization error: %s\n", IMG_GetError());
        return;
    }

    app->gl_context = SDL_GL_CreateContext(app->window);
    if (app->gl_context == NULL)
    {
        printf("[ERROR] Unable to create the OpenGL context!\n");
        return;
    }

    init_opengl();
    reshape(width, height);

    init_camera(&(app->camera));
    init_scene(&(app->scene));

    app->is_running = true;
}

void init_opengl()
{
    glShadeModel(GL_SMOOTH);

    glEnable(GL_NORMALIZE);
    glEnable(GL_AUTO_NORMAL);

    glClearColor(0.1, 0.1, 0.1, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_DEPTH_TEST);

    glClearDepth(1.0);

    glEnable(GL_TEXTURE_2D);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
}

void reshape(GLsizei width, GLsizei height)
{
    int x, y, w, h;
    double ratio;

    ratio = (double)width / height;
    if (ratio > VIEWPORT_RATIO)
    {
        w = (int)((double)height * VIEWPORT_RATIO);
        h = height;
        x = (width - w) / 2;
        y = 0;
    }
    else
    {
        w = width;
        h = (int)((double)width / VIEWPORT_RATIO);
        x = 0;
        y = (height - h) / 2;
    }

    glViewport(x, y, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(
        -.08, .08,
        -.06, .06,
        .1, 50);
}

void handle_app_events(App *app)
{
    GLint m_viewport[4];
    glGetIntegerv(GL_VIEWPORT, m_viewport);
    int width = m_viewport[2];
    int height = m_viewport[3];
    SDL_Event event;
    static Focused_Object_Data object;
    static bool is_mouse_down = false;
    static int mouse_x = 0;
    static int mouse_y = 0;
    static float previous_normalized_mouse_x = 0;
    static float previous_normalized_mouse_y = 0;
    float current_normalized_mouse_x = 0;
    float current_normalized_mouse_y = 0;
    int x;
    int y;
    if (is_focus_active())
    {
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_KEYDOWN:
                switch (event.key.keysym.scancode)
                {
                case SDL_SCANCODE_Q:
                    exit_focus_mode(&(app->scene), &object);
                    break;
                case SDL_SCANCODE_F1:
                    app->camera.is_preview_visible = !app->camera.is_preview_visible;
                break;
                case SDL_SCANCODE_X:
                    set_state_xyz(STATE_X);
                    break;
                case SDL_SCANCODE_Y:
                    set_state_xyz(STATE_Y);
                    break;
                case SDL_SCANCODE_Z:
                    set_state_xyz(STATE_Z);
                    break;
                case SDL_SCANCODE_R:
                    set_state_input(STATE_ROTATE);
                    break;
                case SDL_SCANCODE_M:
                    set_state_input(STATE_MOVE);
                    break;
                case SDL_SCANCODE_S:
                    set_state_input(STATE_SCALE);
                    break;
                case SDL_SCANCODE_N:
                    set_state_input(STATE_NONE);
                    break;
                case SDL_SCANCODE_L:
                    set_state_input(STATE_LIGHT);
                    break;
                case SDL_SCANCODE_BACKSPACE:
                    reset_object_by_state(&object);
                    break;
                case SDL_SCANCODE_KP_PLUS:
                    set_lighting_1(&(app->scene), 0.1, 0, 0, 0);
                    break;
                case SDL_SCANCODE_KP_MINUS:
                    set_lighting_1(&(app->scene), -0.1, 0, 0, 0);
                    break;
                default:
                    break;
                }
            default:
                break;
            case SDL_MOUSEBUTTONDOWN:
                SDL_GetMouseState(&x, &y);
                previous_normalized_mouse_x = (2.0 * x) / width - 1.0;
                previous_normalized_mouse_y = 1.0 - (2.0 * y) / height;
                is_mouse_down = true;
                break;
            case SDL_MOUSEMOTION:
                if (is_mouse_down)
                {
                    SDL_GetMouseState(&x, &y);
                    current_normalized_mouse_x = (2.0 * x) / width - 1.0;
                    current_normalized_mouse_y = 1.0 - (2.0 * y) / height;
                    float distance = p2_dist(previous_normalized_mouse_x, previous_normalized_mouse_y,
                                             current_normalized_mouse_x, current_normalized_mouse_y);
                    set_state_value(distance * sign(current_normalized_mouse_y - previous_normalized_mouse_y));
                    update_object_by_state(&object, &(app->scene));
                    previous_normalized_mouse_x = current_normalized_mouse_x;
                    previous_normalized_mouse_y = current_normalized_mouse_y;
                }
                break;
            case SDL_MOUSEBUTTONUP:
                is_mouse_down = false;
                break;
            }
        }
        return;
    }

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_KEYDOWN:
            switch (event.key.keysym.scancode)
            {
            case SDL_SCANCODE_ESCAPE:
                app->is_running = false;
                break;
            case SDL_SCANCODE_F1:
                app->camera.is_preview_visible = !app->camera.is_preview_visible;
                break;
            case SDL_SCANCODE_W:
                set_camera_speed(&(app->camera), 1);
                break;
            case SDL_SCANCODE_S:
                set_camera_speed(&(app->camera), -1);
                break;
            case SDL_SCANCODE_A:
                set_camera_side_speed(&(app->camera), 1);
                break;
            case SDL_SCANCODE_D:
                set_camera_side_speed(&(app->camera), -1);
                break;
            case SDL_SCANCODE_Q:
                set_camera_vertical_speed(&(app->camera), 1);
                break;
            case SDL_SCANCODE_E:
                set_camera_vertical_speed(&(app->camera), -1);
                break;
            case SDL_SCANCODE_KP_PLUS:
                set_lighting(0.1);
                break;
            case SDL_SCANCODE_KP_MINUS:
                set_lighting(-0.1);
                break;
            case SDL_SCANCODE_LCTRL:
                if (SDL_ShowCursor(SDL_QUERY) == SDL_DISABLE)
                {
                    SDL_ShowCursor(SDL_ENABLE);
                    SDL_SetRelativeMouseMode(SDL_FALSE);
                }
                else
                {
                    SDL_SetRelativeMouseMode(SDL_TRUE);
                    SDL_ShowCursor(SDL_DISABLE);
                }
                break;
            case SDL_SCANCODE_F:
                for (int i = 0; i < app->scene.object_count; i++)
                {
                    if (point_is_in_box(app->camera.position, app->scene.objects[i].bounding_box))
                    {
                        enter_focus_mode(&(app->camera), &(app->scene), i, &object);
                        break;
                    }
                }
                break;
            case SDL_SCANCODE_LSHIFT:
                multiply_camera_speed(&(app->camera), 2.4f);
                break;
            default:
                break;
            }
            break;
        case SDL_KEYUP:
            switch (event.key.keysym.scancode)
            {
            case SDL_SCANCODE_W:
            case SDL_SCANCODE_S:
                set_camera_speed(&(app->camera), 0);
                break;
            case SDL_SCANCODE_A:
            case SDL_SCANCODE_D:
                set_camera_side_speed(&(app->camera), 0);
                break;
            case SDL_SCANCODE_Q:
            case SDL_SCANCODE_E:
                set_camera_vertical_speed(&(app->camera), 0);
                break;
            case SDL_SCANCODE_LSHIFT:
                multiply_camera_speed(&(app->camera), 1.0f);
                break;
            default:
                break;
            }
            break;
        /*case SDL_MOUSEBUTTONDOWN:
            SDL_GetMouseState(&x, &y);
            if (SDL_ShowCursor(SDL_QUERY) == SDL_ENABLE)
            {

            }
            break;*/
        case SDL_MOUSEMOTION:
            SDL_GetMouseState(&x, &y);
            if (SDL_ShowCursor(SDL_QUERY) == SDL_DISABLE)
            {
                rotate_camera(&(app->camera), mouse_x - x, mouse_y - y);
            }

            mouse_x = x;
            mouse_y = y;
            break;
        /*case SDL_MOUSEBUTTONUP:
            is_mouse_down = false;
            break;*/
        case SDL_WINDOWEVENT:
            if (event.window.event == SDL_WINDOWEVENT_RESIZED)
            {
                reshape(event.window.data1, event.window.data2);
            }
            break;

        case SDL_QUIT:
            app->is_running = false;
            break;
        default:
            break;
        }
    }
}

void update_app(App *app)
{
    double current_time;
    double elapsed_time;
    double delta_time;
    static double previous_time;
    current_time = (double)SDL_GetTicks() / 1000;
    // delta time might be equal to elapsed time
    delta_time = current_time - previous_time;
    previous_time = current_time;
    elapsed_time = current_time - app->uptime;
    app->uptime = current_time;

    update_camera(&(app->camera), elapsed_time);
    update_scene(&(app->scene), delta_time, current_time);
}

void render_app(App *app)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);

    glPushMatrix();
    set_view(&(app->camera));
    render_scene(&(app->scene));
    glPopMatrix();

    if (app->camera.is_preview_visible)
    {
        show_texture_preview(&(app->scene), is_focus_active());
    }

    SDL_GL_SwapWindow(app->window);
}

void destroy_app(App *app)
{
    if (app->gl_context != NULL)
    {
        SDL_GL_DeleteContext(app->gl_context);
    }

    if (app->window != NULL)
    {
        SDL_DestroyWindow(app->window);
    }

    SDL_Quit();
}
