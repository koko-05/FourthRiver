#include "Shader.h"
#include "JGL/Scene.h"
#include "algorithm"
#include "ComponentManagerTemplates.cpp"


const char* DEFAULT_SHADER_SRC_VERT = 
    "\n\
    #version 330 core \n\
    \n\
    FR_VAO_ATTRIBS\n\
    \n\
    uniform mat4 uMVP;\n\
    out vec4 oColor;\n\
    \n\
    void main()\n\
    {\n\
        gl_Position = uMVP * vec4( pos, 1.0 );\n\
        oColor      = vec4( 1.0 );\n\
    } ";

const char* DEFAULT_SHADER_SRC_FRAG = 
    "\n\
    #version 330 core \n\
    \n\
    layout(location = 0) out vec4 color;\n\
    in vec4 oColor; \n\
    \n\
    void main()\n\
    {\n\
        color = oColor; \n\
    }";

namespace FourthRiver
{

namespace Components
{

Shader::Shader()
{
}

void Shader::Apply( JGL::Scene* scene )
{
    UNUSED( scene );
    Component::Object::shader = mCurrentShader;
}

std::string Shader::CreateStringMacro( const JGL::VertexElement& ve, size_t i )
{
    // TODO: Add others to the macro
    std::string ret = "layout (location = " + std::to_string(i) + ") in ";
    if ( ve.type == GL_FLOAT )
    {
        if ( ve.count == 1 ) 
        {
            ret += "float " + std::string(ve.name) + ";";
            return ret;
        }
        
        ret += "vec" + std::to_string(ve.count) + " " + std::string( ve.name ) + ";\n";
        return ret;
    }
    else 
        ASSERT(0, "Macro def with non-float type has not been implemented yet");

    return ret;
}


void Shader::CreateMacroDef( JGL::VertexArray& va )
{
    std::string d;
    auto& v = va.getAttribs()[0].attrib.GetArr();

    for ( size_t i = 0; i < v.size(); i++ )
        d += CreateStringMacro( v[i], i);

    std::swap( GeneratedMacroDef, d );
}

void Shader::SetMacro( )
{
    ASSERT( mesh, "Tried creating a shader macro with no mesh!" );
    CreateMacroDef( mesh->VAO );
}

void Shader::SetShader( JGL::Shader* sh )
{
    mCurrentShader = sh;
}

void Shader::SetShader( const std::string& vs, const std::string& fs )
{
    VertexSource   = vs;
    FragmentSource = fs;
}

void Shader::CreateShader()
{  UpdateShader(); }

void Shader::UpdateShader()
{
    if ( !mCurrentShader )
        SetShader( static_cast<JGL::Shader*>(this) );

    if ( !mCurrentShader->gl_Program )
    {
        auto vs = JGL::Shader::replaceMacros( 
            VertexSource.c_str(), 
            "FR_VAO_ATTRIBS", 
            GeneratedMacroDef.c_str() 
        );

        auto fs = JGL::Shader::replaceMacros( 
            FragmentSource.c_str(), 
            "FR_VAO_ATTRIBS", 
            GeneratedMacroDef.c_str() 
        );

        Shader::CreateShaderS( 
            vs ? vs : VertexSource.c_str(),
            fs ? fs : FragmentSource.c_str()
        );

        if (  vs ) delete[] vs;
        if (  fs ) delete[] fs;
        return;
    }
}

void Shader::Merge( Object* dest, JGL::Scene* scene )
{
    UNUSED( scene );
    auto c = dest->FindComponent<Shader>();

    if ( c )
        c->Apply( scene );
    else
        dest->shader = mCurrentShader;
}

void Shader::Unmerge( Object* dest, JGL::Scene* scene )
{
    UNUSED( scene );
    dest->shader = nullptr;
}

uint16_t Shader::GetID() const 
{
    return mId;
}


}

}
