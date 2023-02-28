#include "Mesh.h"

namespace TigerEngine
{
namespace Components
{

void Mesh::Apply( JGL::Scene* sc )
{
    UNUSED( sc );
    JGL::Mesh* m = static_cast<JGL::Mesh*>(this);
    Object::mesh = m;
}

uint16_t Mesh::GetID() const
{
    return mId;
}

}
}
