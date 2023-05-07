#include "Shader.h"
#include "JGL/Scene.h"
#include "algorithm"
#include "ComponentManagerTemplates.cpp"


/* TODO: Turn this into a conditional to object flags  macro like FR_VAO_ATTRIBS */
const char* DEFAULT_SHADER_SRC_VERT = 
    "\n\
    #version 330 core \n\
    \n\
    \n\
    FR_VAO_ATTRIBS\n\
    \n\
    layout (std140) uniform FR_LightingBlock \n\
    {\n\
        vec3 LightsColors[256]; //    16      |       0 \n\
        uint LightCount;        //    4       |     4096\n\
    };\n\
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

void Shader::SetLightingUniforms()
{
    if ( flags & OBJECT_LIGHTING_EFFECT_BIT )
    {
        ASSERT( mCurrentShader->gl_Program, "Attempted to set Lighting Uniforms on empty shader! (%s)", name );
        mCurrentShader->Bind();
        auto index = glGetUniformBlockIndex( mCurrentShader->gl_Program, "FR_LightingBlock" );
        glUniformBlockBinding( mCurrentShader->gl_Program, index, 0 );
    }
}

void Shader::SetMacroOnShader( JGL::Shader* sh )
{
    ASSERT( mesh, "Make sure you have a mesh before creating a shader!" );
    
    if ( !sh || !sh->gl_Program )
    {
        SetShader( static_cast<JGL::Shader*>(this) );
        Shader::SetMacroDef( CreateMacroDef( mesh->VAO ) );
        Shader::CreateShaderS( BaseVertexSource.c_str(), BaseFragmentSource.c_str() );
        SetLightingUniforms();
        return;
    }

    if ( lastGenMacroDef ) delete[] lastGenMacroDef;
    sh->SetMacroDef( CreateMacroDef( mesh->VAO ) );

    auto ssc = BaseVertexSource + BaseFragmentSource + std::string(sh->MacroDef);
    auto altShader = CheckSource( ssc );

    if ( !altShader ) 
    {
        sh->CreateShaderS( 
            sh->VertexSource.c_str(), 
            sh->FragmentSource.c_str() );

        ShaderCache[ssc] = static_cast<JGL::Shader*>( this );
    }
    else
        mCurrentShader = altShader;
}

JGL::Shader* Shader::CheckSource( const std::string& sh )
{
    auto it = ShaderCache.find( sh );
    if ( it == ShaderCache.end() )
        return nullptr;
    else
        return (*it).second;
}

void Shader::SetShader( JGL::Shader* sh )
{
    mCurrentShader = sh;
}

void Shader::SetShader( std::string vs, std::string fs )
{
    BaseVertexSource = vs;
    BaseFragmentSource = fs;
}

void Shader::CreateShader()
{  UpdateShader(); }

void Shader::UpdateShader()
{
    SetMacroOnShader( static_cast<JGL::Shader*>( this ) );
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
