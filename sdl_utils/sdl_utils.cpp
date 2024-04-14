
#include "sdl_utils.h"

void sdl_draw_grid(render_config& r, int unit_size)
{
    int screen_w = 1280;
    int screen_h = 720;

    SDL_SetRenderDrawColor(r.r, 0, 0, 0, 255);

    // Draw the grid using the camera position
    for (int x = 0; x < screen_w; x += r.grid_unit_size)
    {
        SDL_RenderDrawLine(r.r, x, 0, x, screen_h);
    }

    for (int y = 0; y < screen_h; y += r.grid_unit_size)
    {
        SDL_RenderDrawLine(r.r, 0, y, screen_w, y);
    }
}

gridpoint screen_to_grid(render_config &r, int x, int y)
{
    return {x / r.grid_unit_size - r.cam_x / r.grid_unit_size, y / r.grid_unit_size - r.cam_y / r.grid_unit_size};
}
