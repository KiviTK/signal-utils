#pragma once

#include <SDL.h>
#include <SDL_ttf.h>

struct render_config
{
    int grid_unit_size = 50;
    int screen_w = 1280;
    int screen_h = 720;
    SDL_Renderer *r = nullptr;
    TTF_Font *ui_font = nullptr;

    int cam_gx = 0;
    int cam_gy = 0;

    int cam_x = 0;
    int cam_y = 0;
};