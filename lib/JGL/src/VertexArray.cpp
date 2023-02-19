#include "JGL/VertexArray.h"
#include "JGL/common.h"
#include <vector>

namespace JGL
{

const std::vector< VertexElement >& VertexAttribute::GetArr() const 
{ 
    return mElements;
}

GLuint VertexAttribute::GetStride() const 
{
     return mStride;
}

void VertexAttribute::Add( GLenum _type, unsigned int _count, bool _normalized )
{  
    mElements.push_back( { _type, _count, _normalized } );
    mStride += GetGLSize( _type ) * _count;
}
}



namespace JGL
{

VertexArray::VertexArray()
    : mAttributes()
{ 
    glGenVertexArrays(1, &mId); 
    glBindVertexArray(mId);
}

VertexArray::~VertexArray()
{
    if ( mId ) 
        glDeleteVertexArrays( 1, &mId );
}

VertexArray::VertexArray( VertexArray&& _other )
{
    mId        = _other.mId;
    _other.mId = 0;
}

void VertexArray::AddAttrib( const GPUBuffer& vbo, const VertexAttribute& _attrib )
{
    ASSERT( vbo.Type() == GL_ARRAY_BUFFER, "cannot bind vao with non-vbo" );
    vbo.Bind(); Bind();
    unsigned int offset = 0;
    const auto& elems = _attrib.GetArr();

    for ( unsigned int i = 0; i < elems.size(); i++ )
    {
        const auto& elem = elems[i];
        glEnableVertexAttribArray( i );
        glVertexAttribPointer( i, elem.count, elem.type, elem.normalized, _attrib.GetStride(), (void*)size_t(offset) );
        offset += elem.count * GetGLSize( elem.type );
    }

    mAttributes.emplace_back( _attrib, &vbo );
}

void VertexArray::Bind() const
{
    glBindVertexArray( mId );
}

void VertexArray::UnBind() const
{
    glBindVertexArray( 0 );
}

const std::vector<VertexArray::Attribute>& VertexArray::getAttribs() const
{
    return mAttributes;
}


}
