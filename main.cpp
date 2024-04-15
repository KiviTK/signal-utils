#include <iostream>
#include <cmath>
#include <fstream>
#include <chrono>

#include "sigutil/envelope_detector.h"
#include "sigutil/finite_impuse_response_filter.h"
#include "sigutil/generator.h"

#include "sigutil/build_block.h"

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_sdlrenderer.h"

#include <thread>
#include <atomic>
#include "sdl_utils/sdl_utils.h"

#include <SDL.h>
#include <SDL_ttf.h>
#include "sdl_utils/render_configuration.h"

#include "main_input_handler.h"
#undef main
void SetupImGuiStyle()
{
    // Fork of Microsoft style from ImThemes
    ImGuiStyle& style = ImGui::GetStyle();

    style.Alpha = 1.0f;
    style.DisabledAlpha = 0.6000000238418579f;
    style.WindowPadding = ImVec2(4.0f, 6.0f);
    style.WindowRounding = 0.0f;
    style.WindowBorderSize = 0.0f;
    style.WindowMinSize = ImVec2(32.0f, 32.0f);
    style.WindowTitleAlign = ImVec2(0.0f, 0.5f);
    style.WindowMenuButtonPosition = ImGuiDir_Left;
    style.ChildRounding = 0.0f;
    style.ChildBorderSize = 1.0f;
    style.PopupRounding = 0.0f;
    style.PopupBorderSize = 1.0f;
    style.FramePadding = ImVec2(8.0f, 6.0f);
    style.FrameRounding = 0.0f;
    style.FrameBorderSize = 1.0f;
    style.ItemSpacing = ImVec2(8.0f, 6.0f);
    style.ItemInnerSpacing = ImVec2(8.0f, 6.0f);
    style.CellPadding = ImVec2(4.0f, 2.0f);
    style.IndentSpacing = 20.0f;
    style.ColumnsMinSpacing = 6.0f;
    style.ScrollbarSize = 20.0f;
    style.ScrollbarRounding = 0.0f;
    style.GrabMinSize = 5.0f;
    style.GrabRounding = 0.0f;
    style.TabRounding = 4.0f;
    style.TabBorderSize = 0.0f;
    style.TabMinWidthForCloseButton = 0.0f;
    style.ColorButtonPosition = ImGuiDir_Right;
    style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
    style.SelectableTextAlign = ImVec2(0.0f, 0.0f);

    style.Colors[ImGuiCol_Text] = ImVec4(0.09803921729326248f, 0.09803921729326248f, 0.09803921729326248f, 1.0f);
    style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.4980392158031464f, 0.4980392158031464f, 0.4980392158031464f, 1.0f);
    style.Colors[ImGuiCol_WindowBg] = ImVec4(0.9490196108818054f, 0.9490196108818054f, 0.9490196108818054f, 1.0f);
    style.Colors[ImGuiCol_ChildBg] = ImVec4(0.9490196108818054f, 0.9490196108818054f, 0.9490196108818054f, 1.0f);
    style.Colors[ImGuiCol_PopupBg] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    style.Colors[ImGuiCol_Border] = ImVec4(0.6000000238418579f, 0.6000000238418579f, 0.6000000238418579f, 1.0f);
    style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
    style.Colors[ImGuiCol_FrameBg] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.0f, 0.4666666686534882f, 0.8392156958580017f, 0.2000000029802322f);
    style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.0f, 0.4666666686534882f, 0.8392156958580017f, 1.0f);
    style.Colors[ImGuiCol_TitleBg] = ImVec4(0.1321630477905273f, 0.4403105974197388f, 0.7510729432106018f, 1.0f);
    style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.1490196138620377f, 0.5176470875740051f, 0.8901960849761963f, 1.0f);
    style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.0f, 0.0f, 0.0f, 0.5099999904632568f);
    style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.8588235378265381f, 0.8588235378265381f, 0.8588235378265381f, 1.0f);
    style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.8588235378265381f, 0.8588235378265381f, 0.8588235378265381f, 1.0f);
    style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.686274528503418f, 0.686274528503418f, 0.686274528503418f, 1.0f);
    style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.0f, 0.0f, 0.0f, 0.2000000029802322f);
    style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.0f, 0.0f, 0.0f, 0.5f);
    style.Colors[ImGuiCol_CheckMark] = ImVec4(0.09803921729326248f, 0.09803921729326248f, 0.09803921729326248f, 1.0f);
    style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.686274528503418f, 0.686274528503418f, 0.686274528503418f, 1.0f);
    style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.0f, 0.0f, 0.0f, 0.5f);
    style.Colors[ImGuiCol_Button] = ImVec4(0.8588235378265381f, 0.8588235378265381f, 0.8588235378265381f, 1.0f);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.0f, 0.4666666686534882f, 0.8392156958580017f, 0.2000000029802322f);
    style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.0f, 0.4666666686534882f, 0.8392156958580017f, 1.0f);
    style.Colors[ImGuiCol_Header] = ImVec4(0.8588235378265381f, 0.8588235378265381f, 0.8588235378265381f, 1.0f);
    style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.0f, 0.4666666686534882f, 0.8392156958580017f, 0.2000000029802322f);
    style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.0f, 0.4666666686534882f, 0.8392156958580017f, 1.0f);
    style.Colors[ImGuiCol_Separator] = ImVec4(0.4274509847164154f, 0.4274509847164154f, 0.4980392158031464f, 0.5f);
    style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.09803921729326248f, 0.4000000059604645f, 0.7490196228027344f, 0.7799999713897705f);
    style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.09803921729326248f, 0.4000000059604645f, 0.7490196228027344f, 1.0f);
    style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 0.2000000029802322f);
    style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 0.6700000166893005f);
    style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 0.949999988079071f);
    style.Colors[ImGuiCol_Tab] = ImVec4(0.1764705926179886f, 0.3490196168422699f, 0.5764706134796143f, 0.8619999885559082f);
    style.Colors[ImGuiCol_TabHovered] = ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 0.800000011920929f);
    style.Colors[ImGuiCol_TabActive] = ImVec4(0.196078434586525f, 0.407843142747879f, 0.6784313917160034f, 1.0f);
    style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.06666667014360428f, 0.1019607856869698f, 0.1450980454683304f, 0.9724000096321106f);
    style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.1333333402872086f, 0.2588235437870026f, 0.4235294163227081f, 1.0f);
    style.Colors[ImGuiCol_PlotLines] = ImVec4(0.6078431606292725f, 0.6078431606292725f, 0.6078431606292725f, 1.0f);
    style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.0f, 0.4274509847164154f, 0.3490196168422699f, 1.0f);
    style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.8980392217636108f, 0.6980392336845398f, 0.0f, 1.0f);
    style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(1.0f, 0.6000000238418579f, 0.0f, 1.0f);
    style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.1882352977991104f, 0.1882352977991104f, 0.2000000029802322f, 1.0f);
    style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.3098039329051971f, 0.3098039329051971f, 0.3490196168422699f, 1.0f);
    style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.2274509817361832f, 0.2274509817361832f, 0.2470588237047195f, 1.0f);
    style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.0f, 0.0f, 0.0f, 0.0f);
    style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.0f, 1.0f, 1.0f, 0.05999999865889549f);
    style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 0.3499999940395355f);
    style.Colors[ImGuiCol_DragDropTarget] = ImVec4(1.0f, 1.0f, 0.0f, 0.8999999761581421f);
    style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.2588235437870026f, 0.5882353186607361f, 0.9764705896377563f, 1.0f);
    style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.0f, 1.0f, 1.0f, 0.699999988079071f);
    style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.800000011920929f, 0.800000011920929f, 0.800000011920929f, 0.2000000029802322f);
    style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.800000011920929f, 0.800000011920929f, 0.800000011920929f, 0.3499999940395355f);
}
void am_test()
{
    auto start = std::chrono::high_resolution_clock::now();

    printf("Running envelope detector test\n");


    float fc = 5000;
    float fs = 100;
    std::ofstream signal_out("amsignal.txt");
    std::ofstream demod_out("demod.txt");


    std::vector<datapoint<int, float>> am_signal;
    for (int i = 0; i < 20000; i++)
    {
        float t = ((float)i)*.00001;
        am_signal.push_back(
                {i,
                 static_cast<float>(sin(t * fc) * (0.5 + (0.15*((sin(t * fs))))))});

        //am_signal.push_back(sin(t * fc + 0.3) * sin(t * fs));
    }

    for (auto modulated_signal : am_signal)
    {
        signal_out << modulated_signal.y << '\n';
    }

    signal_out.close();

    auto demod = envelope_detection(am_signal);

    for (auto demod_sample : demod)
    {
        demod_out << demod_sample.y << ' ' << demod_sample.x << '\n';
    }

    demod_out.close();

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    printf("Envelope detector test done! Execution time: %lld ms\n\n", duration);
}



void fir_test()
{
    auto start = std::chrono::high_resolution_clock::now();

    printf("Running FIR test\n");

    // Create a simple low pass filter


    std::vector<float> coeffs =
        #include "filters/low_pass_test_filter.h" // This file is generated by octave_scripts/generate_fir.m
    ;

    continuous_fir_filter filter(coeffs);

    std::ofstream signal_out("signal.txt");
    std::ofstream filtered_out("filtered.txt");

    std::vector<datapoint<int, float>> signal = generate_chirp_signal(.02, 12, 1, 5000, 5);


    for (auto sample : signal)
    {
        signal_out << sample.y << '\n';
    }

    signal_out.close();

    for (auto sample : signal)
    {
        filter << sample.y;
        float filtered_sample;
        filter >> filtered_sample;
        filtered_out << filtered_sample << '\n';
    }

    filtered_out.close();

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    printf("FIR test done! Execution time: %lld ms\n\n", duration);
}

void fir_impulse_response_test()
{
    auto start = std::chrono::high_resolution_clock::now();

    printf("Running FIR impulse response test\n");

    // Create a simple low pass filter
    std::vector<float> coeffs =
        #include "filters/low_pass_test_filter_5param.h" // This file is generated by octave_scripts/generate_fir.m
    ;

    continuous_fir_filter filter(coeffs);

    std::ofstream impulse_out("impulse.txt");
    std::ofstream filtered_out("filtered_impulse.txt");

    std::vector<datapoint<int, float>> impulse = generate_pulse_signal(1, 500, 0.5);


    for (auto sample : impulse)
    {
        impulse_out << sample.y << '\n';
    }

    impulse_out.close();

    for (auto sample : impulse)
    {
        filter << sample.y;
        float filtered_sample;
        filter >> filtered_sample;
        filtered_out << filtered_sample << '\n';
    }

    filtered_out.close();

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    printf("FIR impulse response test done! Execution time: %lld ms\n\n", duration);
}

int main()
{
    // Run the tests
/*    fir_test();
    fir_impulse_response_test();
    am_test();*/

    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();



    auto w = SDL_CreateWindow("Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_SHOWN);
    auto r = SDL_CreateRenderer(w, -1, SDL_RENDERER_ACCELERATED);

    render_config rc;
    rc.r = r;
    rc.grid_unit_size = 50;
    rc.screen_w = 1280;
    rc.screen_h = 720;

    rc.ui_font = TTF_OpenFont("OCRAEXT.TTF", 40);

    device_pool dp;
    dp.use_render_config(rc);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NoMouseCursorChange;

    ImGui_ImplSDL2_InitForSDLRenderer(w, r);
    ImGui_ImplSDLRenderer_Init(r);

    SetupImGuiStyle();


    // Create a function generator
    int external_timer = 0;
    function_generator f;
    file_sink fs("output.txt");

    std::vector<float> filter_cfs =
#include "filters/low_pass_test_filter.h" // This file is generated by octave_scripts/generate_fir.m
    ;

    fir_filter filter(filter_cfs);

    // Create a file data source
    file_datasource fds("data.txt");


    multiplier m;

    // Create a device pool


    dp.add_device(&fds);
    dp.add_device(&f);
    dp.add_device(&fs);
    dp.add_device(&filter);
    dp.add_device(&m);

    // Connect the devices

    std::atomic<bool> should_stop = false;

    std::atomic<bool> simulation_status = false;

    // Create the data processing thread
    std::thread data_proc_thread([&dp, &should_stop, &simulation_status](){
        while (!should_stop)
        {
            if (simulation_status)
            {
                bool datasource_status = dp.update_devices();

                if (!datasource_status)
                {
                    simulation_status = false;
                }
            }
        }
    });

    for (;;)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL2_ProcessEvent(&event);
            dp.handle_input(event);
            main_input_handler(rc, event);
        }

        ImGui_ImplSDLRenderer_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        SDL_SetRenderDrawColor(r, 80, 80, 80, 255);
        SDL_RenderClear(r);

        sdl_draw_grid(rc, 50);

        ImGui::NewFrame();

        ImGui::Begin("Main");
        if (ImGui::Button("Stop"))
        {
            should_stop = true;
        }

        // Show mouse grid position
        int mx, my;
        SDL_GetMouseState(&mx, &my);
        auto grid_pos = screen_to_grid(rc, mx, my);
        ImGui::Text("Mouse grid position: %d, %d", grid_pos.x, grid_pos.y);

        // Show the camera position
        ImGui::Text("Camera position: %d, %d", rc.cam_gx, rc.cam_gy);

        if (simulation_status)
        {
            ImGui::Text("Simulation running");
        }
        else
        {
            if (ImGui::Button("Start Simulation"))
            {
                simulation_status = true;
                // Reset all devices
                dp.reset();
            }
        }

        ImGui::End();

        dp.draw_devices(rc);
        dp.show_ui();

        //std::cout << "External timer: " << external_timer << "\t" << "Function generator output: " << f.get_outputs()[0]->value << std::endl;

        ImGui::Render();
        ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
        SDL_RenderPresent(r);

        if (should_stop)
        {
            break;
        }
    }
    should_stop = true;

    data_proc_thread.join();
}


