#include "JGL/LightManager.h"
#include "JGL/common.h"

namespace JGL
{

void LightManager::AddLight( const LightInfo& i )
{
    ASSERT( Count < MAX_LIGHTS, "Can only add %i lights, you can change this by defining MAX_LIGHTS macro before including JGL", MAX_LIGHTS );

    GetColorLoc()[Count] = i.color;
    *GetSizeLoc() = (unsigned int)(++Count);

    UpdateBuffer();
}
JM::Vect3* LightManager::GetColorLoc() 
{ 
    return (JM::Vect3*)((size_t)&(lightData[0])+ colorOffset);
}

unsigned int* LightManager::GetSizeLoc() 
{ 
    return (uint32_t*)((size_t)&(lightData[0]) + sizeOffset );
}

void LightManager::UpdateBuffer()
{
    if ( !UniformBuffer.size() )
    {
        UniformBuffer.Alloc( 
            BlockSize,
            GL_UNIFORM_BUFFER,
            GL_STATIC_DRAW
        );

        UniformBuffer.Bind();
        glBindBufferBase( GL_UNIFORM_BUFFER, 0, UniformBuffer.id() );
    }

    UniformBuffer.Bind();
    glBufferSubData( GL_UNIFORM_BUFFER, 0, BlockSize, lightData );
}

}
