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

void ModuleStringFirst( std::string& target, const std::string& name, const std::string& source )
{
    target += "\n/* MODULE: " + m.Name + " */\n" + m.Source + "\n";
}

void ShaderPipeline::CreatePipeline()
{
    VertexSource   = "";
    FragmentSource = "";

    for ( const auto& m : modules )
    {
        auto sep = m.Source.find("##~~~##");
        if ( sep = std::npos ) sep = m.Source.size() - 1;

        if ( m.Target & 0x01 ) ModuleStringFirst( VertexSource, m.Name, m.Source.substr( 0, sep ) );
        if ( m.Target & 0x02 ) ModuleStringFirst( FragmentSource, m.Name, m.Source.substr( sep ) );
    }

    FragmentSource += "/* MAIN */\nvoid main() {\n";
    VertexSource   += "/* MAIN */\nvoid main() {\n";

    for ( const auto& m : modules )
    {
        if ( m.Target & 0x01 ) VertexSource   += "\t" + m.Name + "();\n";
        if ( m.Target & 0x02 ) FragmentSource += "\t" + m.Name + "();\n";
        
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
