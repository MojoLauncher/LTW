cmake_minimum_required(VERSION 3.10.0)
project(LTW VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)

add_library(ltw SHARED basevertex.c blending.c egl.c framebuffer.c env.c glformats.c license_notice.c main.c multidraw.c of_buffer_copier.c proc.c query.c shader_wrapper.c string_utils.c stubs.c swizzle.c vertexattrib.c)
include_directories("${CMAKE_SOURCE_DIR}" "EGL" "GLES3" "GL")
add_subdirectory(unordered_map)
add_subdirectory(vgpu_shaderconv)
add_subdirectory(glsl_optimizer)

if(APPLE)
	target_link_options(ltw PUBLIC -ffunction-sections -fdata-sections -flto -Wl,-exported_symbols_list,${CMAKE_SOURCE_DIR}/exported_symbols.txt)
else()
target_link_options(ltw PUBLIC  -Wl,--version-script=${CMAKE_SOURCE_DIR}/version.script -ffunction-sections -fdata-sections -flto -Wl,--gc-sections)
endif()

target_link_libraries(ltw unordered_map vgpu_shaderconv glsl_optimizer)
if(ANDROID)
	target_link_libraries(ltw log)
endif()

