#include "ShaderPipeline.h"

namespace FourthRiver
{
namespace Components
{

void ShaderPipeline::AddModuleM( uint8_t target, const std::string& name, InitFunc init, const std::string& src )
{
    modules.emplace_back( name, src, target, init );
}

void ShaderPipeline::InitializeModules()
{
    for ( const auto& m : modules )
        m.Initialize( static_cast<FourthRiver::Object*>(this) );
}

void ShaderPipeline::CreatePipeline()
{
    VertexSource   = "";
    FragmentSource = "";

    for ( const auto& m : modules )
    {
        std::string& target = m.Target == Shaders::VS ? VertexSource : FragmentSource;
        target += "\n/* MODULE: " + m.Name + " */\n" + m.Source + "\n";
    }

    FragmentSource += "/* MAIN */\nvoid main() {\n";
    VertexSource   += "/* MAIN */\nvoid main() {\n";

    for ( const auto& m : modules )
    {
        std::string& target = m.Target == Shaders::VS ? VertexSource : FragmentSource;
        target += "\t" + m.Name + "();\n";
    }

    FragmentSource += "}";
    VertexSource   += "}";

}

void ShaderPipeline::UpdatePipeline()
{
    CreatePipeline();
}

}
}
