#include "Shader.h"
#include "JGL/Scene.h"
#include "ComponentManagerTemplates.cpp"

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


char* Shader::CreateMacroDef( JGL::VertexArray& va )
{
    std::string d;
    auto& v = va.getAttribs()[0].attrib.GetArr();

    for ( size_t i = 0; i < v.size(); i++ )
        d += CreateStringMacro( v[i], i);

    auto dh = new char[ d.size() + 1 ];
    lastGenMacroDef = dh;
    memcpy( dh, d.c_str(), d.size() + 1);
    return dh;
}

void Shader::SetMacroOnShader( JGL::Shader* shader )
{
    ASSERT( mesh, "Make sure you have a mesh before creating a shader!" );
    
    if ( !shader || !shader->gl_Program )
    {
        SetCurrentShader( static_cast<JGL::Shader*>(this) );
        Shader::SetMacroDef( CreateMacroDef( mesh->VAO ) );
        Shader::CreateShaderF( "src/shader.glsl", '~' );
        return;
    }

    if ( lastGenMacroDef ) delete[] lastGenMacroDef;
    shader->SetMacroDef( CreateMacroDef( mesh->VAO ) );
    shader->CreateShaderS( shader->VertexSource.c_str(), shader->FragmentSource.c_str() );
}

void Shader::SetCurrentShader( JGL::Shader* shader )
{
    mCurrentShader = shader;
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
