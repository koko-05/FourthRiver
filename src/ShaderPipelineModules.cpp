#pragma once
#include "ShaderPipeline.h"

namespace FourthRiver
{
namespace Components
{
namespace PipelineModules
{

#define _const static constexpr

struct Core_Vertex
{
    static void Initialize( Object* obj ) { UNUSED( obj ); }

    _const uint8_t Target     = ShaderPipeline::VS;
    _const const char* Name   = "Core";
    _const const char* Source = 
        "#version 330 core\n"
        "FR_VAO_ATTRIBS\n"
        "void Core() {}"; 
};

struct Core_Fragment
{
    static void Initialize( Object* obj ) { UNUSED( obj ); }

    _const uint8_t Target     = ShaderPipeline::FS;
    _const const char* Name   = "Core";
    _const const char* Source = 
        "#version 330 core\n"
        "void Core() {}";
};

struct MVP
{
    static void Initialize( Object* obj ) { UNUSED( obj ); }

    _const uint8_t Target     = ShaderPipeline::VS;
    _const const char* Name   = "MVP";
    _const const char* Source = 
        "uniform mat4 uMVP;\n"
        "void MVP() { gl_Position = uMVP * vec4( pos, 1.0 );}";
};

struct Color
{
    static void Initialize( Object* obj ) { UNUSED( obj ); }

    _const uint8_t Target     = ShaderPipeline::FS;
    _const const char* Name   = "Color";
    _const const char* Source = 
        "uniform vec4 uColor = vec4( 1.0, 1.0, 1.0, 1.0 );\n"
        "layout (location = 0) out vec4 color;\n"
        "void Color() { color = uColor; }";
};

}
}

#undef _const

}
