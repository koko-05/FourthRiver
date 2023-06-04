#pragma once
#include "ShaderPipeline.h"

namespace FourthRiver
{
namespace Components
{
namespace PipelineModules
{

#define _const static constexpr
#define _noInit static void Initialize( Object* obj ) { UNUSED( obj ); }

struct Core
{
    _noInit

    _const uint8_t Target       = ShaderPipeline::FS | ShaderPipeline::VS;
    _const const char* Name     = "Core";
    _const const char* Source   = 
        "#version 330 core\n"
        "FR_VAO_ATTRIBS\n"
        "void Core() {}"
        "##~~~##" /* Vertex to fragment Transition */
        "#version 330 core\n"
        "void Core() {}";
};

struct MVP
{
    _noInit

    _const uint8_t Target    = ShaderPipeline::VS;
    _const const char* Name  = "MVP";
    _const const char* Source= 
        "uniform mat4 uMVP;\n"
        "void MVP() { gl_Position = uMVP * vec4( pos, 1.0 );}";
};

struct Color
{
    _noInit

    _const uint8_t Target    = ShaderPipeline::FS;
    _const const char* Name  = "Color";
    _const const char* Source= 
        "uniform vec4 uColor = vec4( 1.0, 1.0, 1.0, 1.0 );\n"
        "layout (location = 0) out vec4 color;\n"
        "void Color() { color = uColor; }";
};

struct Texturer
{
    ShaderPipeline::InitFunc Initialize;
    _const uint8_t Target = ShaderPipeline::FS | ShaderPipeline::VS;

    Texturer( const char* samplerName,  ShaderPipeline::InitFunc in, const char* code, int dim = 2, bool isFirst = true )
        : Initialize( in ), Source( isFirst ? "out vec2 UV;\n" : "\n")
    {
        Name += samplerName;

        Source += "void " + Name + "() { UV = uv; }\n"
                + "##~~~##" /* Vertex to fragment Transition */
                + "in vec2 UV;\n"
                + "uniform sampler" + std::to_string( dim ) + "D " + samplerName
                + ";\nvoid " + Name + "() { \n" + code + "\n}\n";
    }

    std::string Name     = "Texturer_";
    std::string Source;
};


}
}

#undef _const
#undef _noInit

}
