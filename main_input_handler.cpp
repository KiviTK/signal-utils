#include "main_input_handler.h"

void main_input_handler(render_config &r, SDL_Event &e)
{
    static bool is_panning = false;

    // Handle mousewheel panning of the camera
    // If the mousewheel is pressed down, enter panning mode and store the initial mouse position
    if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == SDL_BUTTON_MIDDLE)
    {
        is_panning = true;
    }

    // If the mousewheel is released, exit panning mode
    if (e.type == SDL_MOUSEBUTTONUP && e.button.button == SDL_BUTTON_MIDDLE)
    {
        is_panning = false;
    }

    // Until the mouse button is released, update the camera position based on the mouse movement in whole grid units
    if (e.type == SDL_MOUSEMOTION && is_panning)
    {
        r.cam_x += e.motion.xrel;
        r.cam_y += e.motion.yrel;

        // Calculate the camera position on the grid from the screen position
        r.cam_gx = r.cam_x / r.grid_unit_size;
        r.cam_gy = r.cam_y / r.grid_unit_size;
    }

    // Handle zooming in and out with the mousewheel in 2x increments
    if (e.type == SDL_MOUSEWHEEL)
    {
        if (e.wheel.y > 0)
        {
            r.grid_unit_size *= 2;
        }
        else
        {
            r.grid_unit_size /= 2;
        }

        r.cam_gx = r.cam_x / r.grid_unit_size;
        r.cam_gy = r.cam_y / r.grid_unit_size;
    }

}
