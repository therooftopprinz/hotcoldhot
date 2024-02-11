// SPDX-License-Identifier: MIT

#include "lv_port_indev.h"
#include "lvgl.h"
#ifdef NXDK
#include <SDL.h>
#else
#include <SDL2/SDL.h>
#endif

static lv_indev_drv_t indev_drv_mouse;
static lv_indev_t *indev_mouse;
static lv_obj_t *mouse_cursor;
static SDL_GameController *pad = NULL;
static int mouse_x, mouse_y;
static lv_quit_event_t quit_event = LV_QUIT_NONE;
static bool mouse_event = false;
static bool mouse_pressed = false;
#ifndef MOUSE_SENSITIVITY
#define MOUSE_SENSITIVITY 50 // pixels per input poll LV_INDEV_DEF_READ_PERIOD
#endif
#ifndef MOUSE_DEADZONE
#define MOUSE_DEADZONE 10 // Percent
#endif

lv_quit_event_t lv_get_quit()
{
    return quit_event;
}

void lv_set_quit(lv_quit_event_t event)
{
    quit_event = event;
}

static void mouse_read(lv_indev_drv_t *indev_drv_gamepad, lv_indev_data_t *data)
{
    data->key = 0;

    static SDL_Event e;
    if (SDL_PollEvent(&e))
    {
        if (e.type == SDL_WINDOWEVENT)
        {
            if (e.window.event == SDL_WINDOWEVENT_CLOSE)
            {
                quit_event = true;
            }
        }

        // Parse some mouse events while we are here.
        if (e.type == SDL_MOUSEMOTION || e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP)
        {
            mouse_event = true;
        }

        if ((e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP) && e.button.button == SDL_BUTTON_LEFT)
        {
            mouse_event = true;
            mouse_pressed = (e.type == SDL_MOUSEBUTTONDOWN);
        }
    }

    //Is there more input events?
    data->continue_reading = (SDL_PollEvent(NULL) != 0);

    data->state = (mouse_pressed) ? LV_INDEV_STATE_PRESSED : LV_INDEV_STATE_RELEASED;

    // Event for a USB mouse
    if (mouse_event)
    {
        uint32_t buttons = SDL_GetMouseState(&mouse_x, &mouse_y);
        data->point.x = mouse_x;
        data->point.y = mouse_y;
        data->state |= (buttons & SDL_BUTTON_LMASK) ? LV_INDEV_STATE_PRESSED : LV_INDEV_STATE_RELEASED;
        mouse_event = false;
    }
}

void lv_port_indev_init()
{
    lv_indev_drv_init(&indev_drv_mouse);
    indev_drv_mouse.type = LV_INDEV_TYPE_POINTER;
    indev_drv_mouse.read_cb = mouse_read;
    indev_mouse = lv_indev_drv_register(&indev_drv_mouse);
    mouse_cursor = lv_img_create(lv_scr_act());
    lv_img_set_src(mouse_cursor, LV_SYMBOL_PLUS);
    lv_indev_set_cursor(indev_mouse, mouse_cursor);

    quit_event = false;
}

void lv_port_indev_deinit(void)
{
    SDL_QuitSubSystem(SDL_INIT_GAMECONTROLLER);
}
