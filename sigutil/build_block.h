#pragma once

#include <string>
#include <vector>
#include "datapoint.h"
#include "finite_impuse_response_filter.h"
#include <unordered_map>

#include <SDL.h>
#include "../sdl_utils/render_configuration.h"


struct io_output_endpoint;
class generic_device;
struct io_input_endpoint
{
    std::string name;
    std::string description;
    io_output_endpoint* connection = nullptr;

    float read();
};

struct io_output_endpoint
{
    std::string name;
    std::string description;
    float value = 0;
};

class generic_device
{
public:
    virtual void show_ui() = 0;
    virtual std::string get_name() = 0;
    virtual std::vector<io_input_endpoint*> get_inputs() = 0;
    virtual std::vector<io_output_endpoint*> get_outputs() = 0;

    virtual void write_outputs() = 0;
    virtual void read_inputs() = 0;
    virtual ~generic_device() = default;
    virtual bool is_datasource();
    virtual bool datasource_ended();
    virtual void reset();
    void raise_graphics_reset();
    bool get_graphics_reset_flag();
    void unset_graphics_reset_flag();
private:
    bool graphics_reset = false;
};

class device_pool
{
public:
    void add_device(generic_device* device);
    void remove_device(generic_device* device);
    void use_render_config(render_config& r);
    bool update_devices();
    void show_ui();
    void draw_devices(render_config& r);
    void handle_input(SDL_Event& e);
    void reset();
private:

    struct dev_display_data
    {
        int xu{};
        int yu{};

        bool is_ui_visible = false;

        SDL_Texture* name_texture{};
        SDL_Rect name_rect{};
        bool is_grabbed = false;
    };

    bool connecting_mode = false;
    io_output_endpoint* connecting_output = nullptr;
    generic_device* connecting_device = nullptr;


    bool grabbing_mode = false;
    generic_device* grabbed_device = nullptr;

    render_config* rc = nullptr;

    std::vector<generic_device*> devices;
    std::unordered_map<generic_device*, dev_display_data> display_data;
};

class function_generator : public generic_device
{
public:
    function_generator();
    void show_ui() override;

    std::string get_name() override;

    void write_outputs() override;
    void read_inputs() override;

    std::vector<io_input_endpoint*> get_inputs() override;
    std::vector<io_output_endpoint*> get_outputs() override;

    float frequency = 2.0f;
    float reference_frequency = 1000.0f;
    float elapsed_time = 0.0f;
    io_output_endpoint output;
private:
    bool is_on = true;
};

// Simple device to write the received data to its memory and then to a file in the destructor
class file_sink : public generic_device
{
public:
    file_sink(std::string filename);
    ~file_sink() override;

    void show_ui() override;

    void reset() override;

    std::string get_name() override;

    void write_outputs() override;
    void read_inputs() override;

    std::vector<io_input_endpoint*> get_inputs() override;
    std::vector<io_output_endpoint*> get_outputs() override;

    io_input_endpoint input;
    std::string filename;
    std::vector<float> memory;
};

class fir_filter : public generic_device
{
public:
    fir_filter(std::vector<float> coeffs);

    void show_ui() override;

    std::string get_name() override;

    void write_outputs() override;
    void read_inputs() override;

    std::vector<io_input_endpoint*> get_inputs() override;
    std::vector<io_output_endpoint*> get_outputs() override;

    io_input_endpoint input;
    io_output_endpoint output;

    continuous_fir_filter filter;
};

class file_datasource : public generic_device
{
public:
    explicit file_datasource(const std::string& filename);
    void show_ui() override;

    std::string get_name() override;

    void write_outputs() override;
    void read_inputs() override;

    std::vector<io_input_endpoint*> get_inputs() override;
    std::vector<io_output_endpoint*> get_outputs() override;
    io_output_endpoint output;

    bool is_datasource() override;
    bool datasource_ended() override;

    void reset() override;

    int data_index = 0;
    std::string filename;
    std::vector<float> data;
};

class multiplier : public generic_device
{
public:
    void show_ui() override;

    std::string get_name() override;

    void write_outputs() override;
    void read_inputs() override;

    std::vector<io_input_endpoint*> get_inputs() override;
    std::vector<io_output_endpoint*> get_outputs() override;

    float gain = 1.0f;
    float weight1 = 1.0f;
    float weight2 = 1.0f;

    float value = 0.0f;

    io_input_endpoint input1;
    io_input_endpoint input2;
    io_output_endpoint output;
};