cmake_minimum_required(VERSION 3.26)
project(signaling_utilities)

set(CMAKE_CXX_STANDARD 17)

set(IMGUI_SRC
        external/imgui/imgui.cpp external/imgui/imgui_demo.cpp external/imgui/imgui_draw.cpp external/imgui/imgui_tables.cpp external/imgui/imgui_tables.cpp
        external/imgui/backends/imgui_impl_sdlrenderer.cpp
)
include_directories(external/imgui external/imgui/backends)
set (SDL_LIB SDL2 SDL2_ttf)

include_directories(external/sdl/include)
include_directories(external/sdl_ttf/include)
include_directories(external/fftw3_win)
include_directories(external/rtd_dm35418/include)

set (SDL_LIB ${CMAKE_SOURCE_DIR}/external/sdl/lib/x64/SDL2.lib ${CMAKE_SOURCE_DIR}/external/sdl_ttf/lib/x64/SDL2_ttf.lib)
set (LIBS ${CMAKE_SOURCE_DIR}/external/fftw3_win/libfftw3-3.lib)

add_executable(signaling_utilities main.cpp sigutil/envelope_detector.cpp sigutil/envelope_detector.h
        sigutil/datapoint.cpp
        sigutil/datapoint.h
        sigutil/finite_impuse_response_filter.cpp
        sigutil/finite_impuse_response_filter.h
        sigutil/generator.cpp
        sigutil/generator.h
        filters/low_pass_test_filter_5param.h
        sigutil/build_block.cpp
        sigutil/build_block.h

        external/imgui/imgui.cpp external/imgui/imgui_widgets.cpp external/imgui/imgui_draw.cpp external/imgui/imgui_tables.cpp
        external/imgui/backends/imgui_impl_sdlrenderer.cpp external/imgui/imgui_demo.cpp external/imgui/backends/imgui_impl_sdl.cpp
        sdl_utils/sdl_utils.cpp
        sdl_utils/sdl_utils.h
        sdl_utils/render_configuration.h
        main_input_handler.cpp
        main_input_handler.h
)

target_link_libraries(signaling_utilities ${SDL_LIB} ${LIBS})