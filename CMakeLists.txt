# Project 
set( name TigerEngine )
cmake_minimum_required(VERSION 3.12)
project(${name} VERSION 1.0 LANGUAGES CXX C)

# Request C++ 17 standard features
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Set warning level
if(MSVC)
    add_compile_options( /W4 /WX )
else()
    add_compile_options( -Wall -Wextra -Wpedantic ) 
    add_compile_options( -ggdb3 -DDEBUG_BUILD )
endif()

# Compile the passed source files into an executable 
add_executable(${name} 
    "examples/Kitten.cpp" 

    "src/TigerEngine.cpp"
    "src/Component.cpp"
    "src/ComponentManager.cpp"
    "src/Object.cpp"
    "src/Shader.cpp"
    "src/Transform.cpp"
    "src/Mesh.cpp"

    "lib/imgui/imgui.cpp"
    "lib/imgui/imgui_draw.cpp"
    "lib/imgui/imgui_tables.cpp"
    "lib/imgui/imgui_widgets.cpp"
    "lib/imgui/backends/imgui_impl_glfw.cpp"
    "lib/imgui/backends/imgui_impl_opengl3.cpp"

    "lib/JM/src/JMath.cpp"
    "lib/JGL/deps/stb_image.cpp" 
    "lib/JGL/src/common.cpp" "lib/JGL/src/GPUBuffer.cpp" 
    "lib/JGL/src/OpenGL.cpp" "lib/JGL/src/Scene.cpp" 
    "lib/JGL/src/Shader.cpp" "lib/JGL/src/Texture.cpp" 
    "lib/JGL/src/VertexArray.cpp" "lib/JGL/src/Camera.cpp" 
     "lib/JGL/src/Callbacks.cpp"

    "lib/glad/glad.c"
)
#JGL Library ( TODO: create a .lib or .a for this lib (idk how to do it now))

# add include to search path
target_include_directories(${name} PUBLIC
    "include" "lib" "lib/imgui" "." )

#compile glfw3 from src, since we dont know what libs to link against
set( GLFW_BUILD_DOCS     OFF CACHE BOOL "" FORCE )
set( GLFW_BUILD_TESTS    OFF CACHE BOOL "" FORCE )
set( GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE )
add_subdirectory("lib/GLFW")

# Add libraries link
# Windows: "opengl32" "gdi32" "user32" "kernel32" "shell32" "ws2_32" 
target_link_libraries( ${name} PUBLIC glfw )

# Set the output directory for executables
set_target_properties(${name}
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

