#include "Texture.h"


namespace FourthRiver
{
namespace Components
{

void Texture::Apply( JGL::Scene* sc )
{
    ASSERT( texture, "texture is nullptr! (%s)", name );
    texture->Bind();
}

void Texture::Merge( Object* dest, JGL::Scene* scene )
{
    auto& c = dest->GetComponent<Texture>();

    ASSERT( texture, "texture is nullptr at merge! (%s)", name );
    ASSERT( c.texture, "object texture is nullptr at merge! (%s)", dest->name );

    texture->Bind();
    c.texture->Bind();
}

void Texture::Unmerge( Object* dest, JGL::Scene* scene )
{

}

}
}
