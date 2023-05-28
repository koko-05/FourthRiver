#include "JGL/UniformBufferObject.h"


namespace JGL
{

void UBO::Create( size_t _index )
{
    Alloc( size, GL_UNIFORM_BUFFER, GL_STATIC_DRAW );
    Bind();
    glBindBufferBase( GL_UNIFORM_BUFFER, _index, id() );
    index = _index;
}

void UBO::UpdateBuffer()
{
    Bind();
    glBufferSubData( GL_UNIFORM_BUFFER, 0, size, udata() );
}

void* UBO::udata() { return Memory(); }

void* UBO::GetLocation( size_t index )
{ 
    return elements[index].offset + (char*)udata();
}

void UBO::AddElement( size_t count, size_t align, const char* glslCode )
{
    size_t offset = size + size % align;

    elements.push_back( { offset, align, glslCode } );
    size += count * align;
}

}
