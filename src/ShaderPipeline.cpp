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

void ShaderPipeline::ModuleStringFirst( std::string& target, const std::string& name, const std::string& source )
{
    target += "\n/* MODULE: " + name + " */\n" + source + "\n";
}

void ShaderPipeline::CreatePipeline()
{
    VertexSource   = "";
    FragmentSource = "";

    for ( const auto& m : modules )
    {
        auto sep = m.Source.find("##~~~##");
        if ( sep == std::string::npos ) 
        {
            if ( m.Target & 0x01 ) ModuleStringFirst( VertexSource, m.Name, m.Source );
            if ( m.Target & 0x02 ) ModuleStringFirst( FragmentSource, m.Name, m.Source );
        }
        else
        {
            sep += 7;
            if ( m.Target & 0x01 ) ModuleStringFirst( VertexSource, m.Name, m.Source.substr( 0, sep - 7 ) );
            if ( m.Target & 0x02 ) ModuleStringFirst( FragmentSource, m.Name, m.Source.substr( sep ) );
        }
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