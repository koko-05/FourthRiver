#include "Shader.h"
#include "JGL/Scene.h"
#include "ComponentManagerTemplates.cpp"

namespace FourthRiver
{

namespace Components
{

Shader::Shader()
{
    // TODO: if you already created one, why create another? Use a global, friend
    JGL::Shader::CreateShaderF( "src/shader.glsl", '~' );
}

void Shader::Apply( JGL::Scene* scene )
{
    UNUSED( scene );
    Component::Object::shader = static_cast<JGL::Shader*>(this);
}

void Shader::Merge( Object* dest, JGL::Scene* scene )
{
    UNUSED( scene );
    if ( !dest->FindComponent<Shader>() )
        dest->shader = static_cast<JGL::Shader*>(this);
}

uint16_t Shader::GetID() const 
{
    return mId;
}


}

}
