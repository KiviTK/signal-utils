#include "build_block.h"
#include <algorithm>
#include <fstream>
#include <cmath>
#include <iostream>

#include "imgui.h"
#include "../sdl_utils/sdl_utils.h"

std::vector<io_input_endpoint *> function_generator::get_inputs()
{
    return {};
}

std::vector<io_output_endpoint *> function_generator::get_outputs()
{
    return {&output};
}

void function_generator::show_ui()
{
    ImGui::Begin("Function Generator");
    ImGui::SliderFloat("Frequency", &frequency, 1, 200);


    if (ImGui::Button("On/Off"))
    {
        is_on = !is_on;
    }


    ImGui::End();
}

function_generator::function_generator()
{

}

std::string function_generator::get_name()
{
    return "Function Generator";
}

void function_generator::read_inputs()
{

}

void function_generator::write_outputs()
{
    // Generate the output sine waveform
    elapsed_time += 1.0f / reference_frequency;

    if (!is_on)
    {
        output.value = 0;
        return;
    }

    output.value = 1.0f * sin(2 * M_PI * frequency * elapsed_time);
}

void device_pool::add_device(generic_device *device)
{
    devices.push_back(device);
    display_data[device] = {static_cast<int>(2 + (devices.size() * 4)), 2, false, nullptr};

    // Generate the name texture with SDL TTF

    if (rc != nullptr)
    {
        SDL_Surface* name_surface = TTF_RenderText_Solid(rc->ui_font, device->get_name().c_str(), {255, 255, 255});
        display_data[device].name_texture = SDL_CreateTextureFromSurface(rc->r, name_surface);
        display_data[device].name_rect = {0, 0, name_surface->w, name_surface->h};
        SDL_FreeSurface(name_surface);
    }

}

void device_pool::remove_device(generic_device *device)
{
    devices.erase(std::remove(devices.begin(), devices.end(), device), devices.end());
}

bool device_pool::update_devices()
{
    bool datasources_available = false;
    for (auto device : devices)
    {
        device->read_inputs();
    }

    for (auto device : devices)
    {
        device->write_outputs();
        if (device->is_datasource() && !device->datasource_ended())
        {
            datasources_available = true;
        }
    }
    return datasources_available;
}

void device_pool::show_ui()
{
    for (auto device : devices)
    {
        // Show the UI for each device if it is visible
        if (display_data[device].is_ui_visible)
        {
            // Pin the next window position to the bottom of the device rectangle using the camera position as offset
            ImGui::SetNextWindowPos(ImVec2((display_data[device].xu + rc->cam_gx) * rc->grid_unit_size, (display_data[device].yu+rc->cam_gy) * rc->grid_unit_size +  + 2 * rc->grid_unit_size));

            device->show_ui();
        }
    }
}

void device_pool::draw_devices( render_config& r )
{
    // If in connecting mode, draw a line from the selected output port to the mouse cursor
    if (connecting_mode && connecting_output != nullptr)
    {
        int x, y;
        SDL_GetMouseState(&x, &y);

        // Convert mouse coordinates to grid units using the camera position
        auto gp = screen_to_grid(r, x, y);

        SDL_SetRenderDrawColor(r.r, 0, 255, 0, 255);


        // Create the connecting_index based on the connecting_output port and the device output list
        int connecting_index = 0;
        for (auto device : devices)
        {
            auto outputs = device->get_outputs();
            for (int i = 0; i < outputs.size(); i++)
            {
                if (outputs[i] == connecting_output)
                {
                    connecting_index = i;
                }
            }
        }

        // Draw the line from the output port to the mouse cursor
        SDL_RenderDrawLine(r.r, (display_data[connecting_device].xu + r.cam_gx + 2) * r.grid_unit_size + 10,
                           (display_data[connecting_device].yu + r.cam_gy) * r.grid_unit_size + 15 + connecting_index * 20,
                           x,
                           y);
    }

    // Draw the devices as rectangles
    for (auto dev_data: display_data)
    {
        // Set up the display rect for the device using the camera position and the device position
        SDL_Rect rect;
        rect.x = (dev_data.second.xu + r.cam_gx) * r.grid_unit_size ;
        rect.y = (dev_data.second.yu + r.cam_gy )* r.grid_unit_size ;
        rect.w = 2 * r.grid_unit_size;
        rect.h = 2 * r.grid_unit_size;

        // If the UI is visible set fill color to light blue
        if (dev_data.second.is_ui_visible)
        {
            SDL_SetRenderDrawColor(r.r, 180, 180, 200, 255);
        }
        else
        {
            SDL_SetRenderDrawColor(r.r, 255, 255, 255, 255);
        }

        SDL_RenderFillRect(r.r, &rect);
        SDL_SetRenderDrawColor(r.r, 0, 0, 0, 255);
        SDL_RenderDrawRect(r.r, &rect);

        SDL_Rect name_rect;
        name_rect.x = (dev_data.second.xu + r.cam_gx)  * r.grid_unit_size + 5;
        name_rect.y = (dev_data.second.yu + r.cam_gy) * r.grid_unit_size - 35 ;
        name_rect.w = dev_data.second.name_rect.w / 2;
        name_rect.h = dev_data.second.name_rect.h / 2;

        SDL_RenderCopy(r.r, dev_data.second.name_texture, nullptr, &name_rect);

        // If the device is a datasource, draw a red arrow pointing to the right
        if (dev_data.first->is_datasource())
        {
            SDL_SetRenderDrawColor(r.r, 255, 0, 0, 255);
            SDL_RenderDrawLine(r.r, (dev_data.second.xu + r.cam_gx) * r.grid_unit_size + 10,
                               (dev_data.second.yu + r.cam_gy) * r.grid_unit_size + 10,
                               (dev_data.second.xu + r.cam_gx) * r.grid_unit_size + 20,
                               (dev_data.second.yu + r.cam_gy) * r.grid_unit_size + 10);

            SDL_RenderDrawLine(r.r, (dev_data.second.xu + r.cam_gx) * r.grid_unit_size + 20,
                               (dev_data.second.yu + r.cam_gy) * r.grid_unit_size + 10,
                               (dev_data.second.xu + r.cam_gx) * r.grid_unit_size + 15,
                               (dev_data.second.yu + r.cam_gy) * r.grid_unit_size + 5);

            SDL_RenderDrawLine(r.r, (dev_data.second.xu + r.cam_gx) * r.grid_unit_size + 20,
                               (dev_data.second.yu + r.cam_gy) * r.grid_unit_size + 10,
                               (dev_data.second.xu + r.cam_gx) * r.grid_unit_size + 15,
                               (dev_data.second.yu + r.cam_gy) * r.grid_unit_size + 15);
        }


        // Draw the input ports equally spaced based on the number of ports of the device
        auto inputs = dev_data.first->get_inputs();
        for (int i = 0; i < inputs.size(); i++)
        {
            SDL_Rect port_rect;
            port_rect.x = (dev_data.second.xu + r.cam_gx) * r.grid_unit_size - 10 ;
            port_rect.y = (dev_data.second.yu + r.cam_gy) * r.grid_unit_size + 10 + i * 20;
            port_rect.w = 10;
            port_rect.h = 10;

            SDL_SetRenderDrawColor(r.r, 255, 0, 0, 255);
            SDL_RenderFillRect(r.r, &port_rect);
            SDL_SetRenderDrawColor(r.r, 0, 0, 0, 255);
            SDL_RenderDrawRect(r.r, &port_rect);
        }

        // Draw the output ports equally spaced based on the number of ports of the device
        auto outputs = dev_data.first->get_outputs();
        for (int i = 0; i < outputs.size(); i++)
        {
            SDL_Rect port_rect;
            port_rect.x = (dev_data.second.xu + r.cam_gx + 2) * r.grid_unit_size;
            port_rect.y = (dev_data.second.yu + r.cam_gy) * r.grid_unit_size + 10 + i * 20;
            port_rect.w = 10;
            port_rect.h = 10;

            SDL_SetRenderDrawColor(r.r, 0, 255, 0, 255);
            SDL_RenderFillRect(r.r, &port_rect);
            SDL_SetRenderDrawColor(r.r, 0, 0, 0, 255);
            SDL_RenderDrawRect(r.r, &port_rect);
        }

        // Draw the connection lines coming into the input ports
        for (auto & input : inputs)
        {
            if (input->connection != nullptr)
            {
                // Find the connected output port
                auto output = input->connection;

                // Find the device that the output port belongs to
                generic_device* output_device = nullptr;
                int port_index = 0;

                for (auto device_ddata : display_data)
                {
                    auto dev_outputs = device_ddata.first->get_outputs();
                    for (int i = 0; i < dev_outputs.size(); i++)
                    {
                        if (dev_outputs[i] == output)
                        {
                            output_device = device_ddata.first;
                            port_index = i;
                        }
                    }
                }

                // Draw the connection line

                SDL_SetRenderDrawColor(r.r, 0, 255, 0, 255);

                int x1 = (display_data[output_device].xu + r.cam_gx + 2) * r.grid_unit_size + 10;
                int y1 = (display_data[output_device].yu + r.cam_gy) * r.grid_unit_size + 15 + port_index * 20;

                int x2 = (dev_data.second.xu + r.cam_gx) * r.grid_unit_size - 10;
                int y2 = (dev_data.second.yu + r.cam_gy) * r.grid_unit_size + 15 + port_index * 20;


                int x1o = x1 + 10;
                int y1o = y1;
                int x2o = x2 - 10;
                int y2o = y2;

                SDL_RenderDrawLine(r.r, x1o, y1o, x2o, y2o);
                SDL_RenderDrawLine(r.r, x2o, y2o, x2, y2);
                SDL_RenderDrawLine(r.r, x1o, y1o, x1, y1);


            }
        }
    }
}

void device_pool::use_render_config(render_config &r)
{
    rc = &r;
}

void device_pool::handle_input(SDL_Event &e)
{
    // Activate the connecting mode on the C key
    if (e.type == SDL_KEYDOWN)
    {
        if (e.key.keysym.sym == SDLK_c)
        {
            connecting_mode = true;
            connecting_output = nullptr;
        }
    }


    // Activate the grabber on the G key
    if (e.type == SDL_KEYDOWN)
    {
        if (e.key.keysym.sym == SDLK_g)
        {
            grabbing_mode = true;
            grabbed_device = nullptr;
        }
    }

    if (e.type == SDL_MOUSEMOTION)
    {
        if (grabbing_mode && grabbed_device != nullptr)
        {
            int x, y;
            SDL_GetMouseState(&x, &y);

            // Convert mouse coordinates to grid units using the camera position
            auto gp = screen_to_grid(*rc, x, y);

            display_data[grabbed_device].xu = gp.x;
            display_data[grabbed_device].yu = gp.y;
        }
    }


    if (e.type == SDL_MOUSEBUTTONDOWN)
    {
        if (e.button.button == SDL_BUTTON_LEFT)
        {
            if (grabbing_mode)
            {
                if (grabbed_device == nullptr)
                {
                    // Grab new device under the cursor
                    int x, y;
                    SDL_GetMouseState(&x, &y);

                    auto gp = screen_to_grid(*rc, x, y);

                    for (auto& dev_data : display_data)
                    {
                        if (dev_data.second.xu == gp.x && dev_data.second.yu == gp.y)
                        {
                            dev_data.second.is_grabbed = true;
                            grabbed_device = dev_data.first;
                        }
                    }
                }

                else
                {
                    // Drop the grabbed device
                    grabbed_device = nullptr;
                    grabbing_mode = false;
                }
            }
            else if (connecting_mode)
            {
                int x, y;
                SDL_GetMouseState(&x, &y);

                auto gp = screen_to_grid(*rc, x, y);

                // Currently no output is selected
                if (connecting_output == nullptr)
                {
                    // Check if we clicked a valid output rectangle


                    for (auto& dev_data : display_data)
                    {
                        SDL_Rect rect;
                        rect.x = (dev_data.second.xu + rc->cam_gx) * rc->grid_unit_size + 2 * rc->grid_unit_size;
                        rect.y = (dev_data.second.yu + rc->cam_gy) * rc->grid_unit_size;
                        rect.w = 10;
                        rect.h = 10;

                        if (gp.x == dev_data.second.xu + 2 && gp.y >= dev_data.second.yu && gp.y <= dev_data.second.yu + 2)
                        {
                            auto a = dev_data.first->get_outputs()[gp.y - dev_data.second.yu];
                            connecting_output = a;
                            connecting_device = dev_data.first;
                            std::cout << "hey" << std::endl;
                        }
                    }
                }
                else
                {
                    // Check if we clicked a valid input rectangle
                    for (auto& dev_data : display_data)
                    {
                        SDL_Rect rect;
                        rect.x = (dev_data.second.xu + rc->cam_gx) * rc->grid_unit_size;
                        rect.y = (dev_data.second.yu + rc->cam_gy) * rc->grid_unit_size;
                        rect.w = 10;
                        rect.h = 10;

                        if (gp.x == dev_data.second.xu && gp.y >= dev_data.second.yu && gp.y <= dev_data.second.yu + 2)
                        {
                            auto a = dev_data.first->get_inputs()[gp.y - dev_data.second.yu];
                            a->connection = connecting_output;
                            connecting_output = nullptr;
                            connecting_device = nullptr;
                            connecting_mode = false;
                        }
                    }
                }
            }
            else
            {
                int x, y;
                SDL_GetMouseState(&x, &y);

                // Convert mouse coordinates to grid units
                auto gp = screen_to_grid(*rc, x, y);

                for (auto& dev_data : display_data)
                {
                    SDL_Rect rect;
                    rect.x = dev_data.second.xu * rc->grid_unit_size;
                    rect.y = dev_data.second.yu * rc->grid_unit_size;
                    rect.w = 2 * rc->grid_unit_size;
                    rect.h = 2 * rc->grid_unit_size;

                    // Check if the mouse is inside the device rectangle in grid units, devices are 2 units wide
                    if (gp.x >= dev_data.second.xu && gp.x <= dev_data.second.xu + 2 && gp.y >= dev_data.second.yu && gp.y <= dev_data.second.yu + 2)
                    {
                        dev_data.second.is_ui_visible = !dev_data.second.is_ui_visible;
                    }
                }
            }

        }
    }


}

file_sink::file_sink(std::string filename) : filename(std::move(filename))
{

}

file_sink::~file_sink()
{
    std::ofstream file(filename);
    for (auto sample : memory)
    {
        file << sample << '\n';
    }
}

void file_sink::show_ui()
{

}

std::string file_sink::get_name()
{
    return "File Sink: " + filename;
}

void file_sink::write_outputs()
{

}

void file_sink::read_inputs()
{
    memory.push_back(input.connection->value);
}

std::vector<io_input_endpoint *> file_sink::get_inputs()
{
    return {&input};
}

std::vector<io_output_endpoint *> file_sink::get_outputs()
{
    return {};
}

fir_filter::fir_filter(std::vector<float> coeffs) : filter(coeffs)
{

}

void fir_filter::show_ui()
{

}

std::string fir_filter::get_name()
{
    return "FIR Filter";
}

void fir_filter::write_outputs()
{
    filter >> output.value;
}

void fir_filter::read_inputs()
{
    filter << input.connection->value;
}

std::vector<io_input_endpoint *> fir_filter::get_inputs()
{
    return {&input};
}

std::vector<io_output_endpoint *> fir_filter::get_outputs()
{
    return {&output};
}

bool generic_device::is_datasource()
{
    return false;
}

bool generic_device::datasource_ended()
{
    return false;
}

file_datasource::file_datasource(const std::string& filename) : filename(filename)
{
    std::ifstream file(filename);
    float sample;
    while (file >> sample)
    {
        data.push_back(sample);
    }
}

std::string file_datasource::get_name()
{
    return "File Datasource [" + filename + "]";
}

void file_datasource::show_ui()
{
    ImGui::Begin("File Datasource");
    ImGui::Text("Filename: %s", filename.c_str());
    ImGui::Text("Data size: %d", data.size());
    ImGui::End();
}

void file_datasource::read_inputs()
{

}

void file_datasource::write_outputs()
{
    if (data_index < data.size())
    {
        output.value = data[data_index];
        data_index++;
    }
    else
    {
        output.value = 0;
    }
}

std::vector<io_input_endpoint *> file_datasource::get_inputs()
{
    return {};
}

std::vector<io_output_endpoint *> file_datasource::get_outputs()
{
    return {&output};
}


bool file_datasource::is_datasource()
{
    return true;
}
bool file_datasource::datasource_ended()
{
    return data_index >= data.size();
}

float io_input_endpoint::read()
{
    if (connection != nullptr)
    {
        return connection->value;
    }


    return 0;
}
