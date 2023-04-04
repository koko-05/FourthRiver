#include "Shader.h"
#include "JGL/Scene.h"
#include "ComponentManagerTemplates.cpp"

namespace FourthRiver
{

namespace Components
{

JGL::Shader* GLOBAL_SHADER = nullptr;

Shader::Shader()
{
    if ( !GLOBAL_SHADER )
    {
        GLOBAL_SHADER = new JGL::Shader;
        GLOBAL_SHADER->CreateShaderF( "src/shader.glsl", '~' );
    }

    mCurrentShader = GLOBAL_SHADER;
}

void Shader::Apply( JGL::Scene* scene )
{
    UNUSED( scene );
    Component::Object::shader = mCurrentShader;
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
