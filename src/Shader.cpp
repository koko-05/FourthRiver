#include "Shader.h"
#include "JGL/Scene.h"

namespace TigerEngine
{

namespace Components
{

Shader::Shader()
{
    JGL::Shader::CreateShaderF( "src/shader.glsl", '~' );
}

void Shader::Apply( JGL::Scene* scene )
{
    UNUSED( scene );

    JGL::Shader* sh = static_cast<JGL::Shader*>(this);
    sh->Bind();

    if ( Component::Object::cached_MVPid )
        sh->SetUniformMat4( Component::Object::cached_MVPid, Component::Object::mvp_data.Transpose());

    Component::Object::shader = sh;
}

uint16_t Shader::GetID() const 
{
    return mId;
}


}

}
