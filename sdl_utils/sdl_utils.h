#pragma once

#include <SDL.h>
#include "render_configuration.h"

struct gridpoint
{
    int x;
    int y;
};

void sdl_draw_grid(render_config& r, int unit_size = 50);
gridpoint screen_to_grid(render_config& r, int x, int y);
