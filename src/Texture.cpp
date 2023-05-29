#include "Texture.h"
#include "src/ComponentManagerTemplates.cpp"


namespace FourthRiver
{
namespace Components
{

void Texture::Apply( JGL::Scene* sc )
{
    UNUSED( sc );
    ASSERT( texture, "texture is nullptr! (%s)", name );
    texture->Bind();
}

void Texture::Merge( Object* dest, JGL::Scene* scene )
{
    UNUSED( scene );
    auto& c = dest->GetComponent<Texture>();

    ASSERT( texture, "texture is nullptr at merge! (%s)", name );
    ASSERT( c.texture, "object texture is nullptr at merge! (%s)", dest->name );

    texture->Bind();
    c.texture->Bind();
}

void Texture::Unmerge( Object* dest, JGL::Scene* scene )
{
    UNUSED( scene );
    UNUSED( dest );
}

}
}
