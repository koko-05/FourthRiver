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

void UBO::Associate( JGL::Shader& sh, const char* name)
{
    ASSERT( index != -1, "Tried to associete \"%s\" with empty UniformBuffer!", name);

    auto i = glGetUniformBlockIndex( sh.gl_Program, name );
    if ( i == GL_INVALID_INDEX )
    {
        DEBUG_PRINT( "Could not find \"%s\" in shader!", "[WARN]", stdout, name );
        return;
    }

    glUniformBlockBinding( sh.gl_Program, i, index );
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
