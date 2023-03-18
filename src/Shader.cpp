#include "Shader.h"
#include "JGL/Scene.h"

namespace TigerEngine
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

    JGL::Shader* sh = static_cast<JGL::Shader*>(this);
    sh->Bind();

    if ( Component::Object::flags & OBJECT_MVPID_CACHE_BIT )
    {
        sh->SetUniformMat4( 
            Component::Object::cached_MVPid, 
            Component::Object::mvp_data.Transpose()
        );
    }
    else
    {
        cached_MVPid = sh->GetUniformLocation( "uMVP" );
        Component::Object::flags |= OBJECT_MVPID_CACHE_BIT;
    }

    Component::Object::shader = sh;
}

uint16_t Shader::GetID() const 
{
    return mId;
}


}

}
