#include "JGL/GPUBuffer.h"
#include "JGL/common.h"
#include <algorithm> // std::swap

namespace JGL
{

/* 
 * Class definitions for GPUBuffer 
 * 
 * */

GPUBuffer::GPUBuffer( )
{
    glGenBuffers( 1, &mId );
}

GPUBuffer::~GPUBuffer()
{
    if ( !mId ) return;
    if ( mMappedMem )
        ReleaseAccess();
    glDeleteBuffers( 1, &mId );
}

GPUBuffer::GPUBuffer( const GPUBuffer& v )
    : mSize( v.mSize ), mTarget( v.mTarget ),
      mType( v.mType )
{
    glGenBuffers(1 , &mId ); Bind( v.mTarget );
    glBufferData( v.mTarget, v.mSize, nullptr, v.mType );

    glBindBuffer( GL_COPY_WRITE_BUFFER, mId );
    glBindBuffer( GL_COPY_READ_BUFFER, v.mId );
    glCopyBufferSubData( 
            GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, 
            0, 0, v.mSize 
        );
    glBindBuffer( GL_COPY_WRITE_BUFFER, 0 );
    glBindBuffer( GL_COPY_READ_BUFFER, 0 );
    Unbind();
}

GPUBuffer& GPUBuffer::operator=(GPUBuffer& v )
{
    if ( &v == this ) return *this;

    GPUBuffer temp( v );

    std::swap( temp.mMappedMem, mMappedMem );
    std::swap( temp.mSize,   mSize );
    std::swap( temp.mTarget, mTarget );
    std::swap( temp.mType,   mType );
    std::swap( temp.mId,     mId );

    return *this;
}

GPUBuffer::GPUBuffer( GPUBuffer&& v )
{
    mSize        = v.mSize;
    mMappedMem   = v.mMappedMem;
    mTarget      = v.mTarget;
    mType        = v.mType;
    mId          = v.mId;

    v.mMappedMem = nullptr;
    v.mSize      = 0;
    v.mTarget    = 0;
    v.mType      = 0;
    v.mId        = 0;
}



void GPUBuffer::Bind() const
{
    glBindBuffer( mTarget, mId );
}

void GPUBuffer::Bind( GLenum target )
{
    glBindBuffer( target, mId );
    mTarget = target;
}

void GPUBuffer::Unbind() const
{
    glBindBuffer( mTarget, 0 );
}

void GPUBuffer::GetAccess( GLenum access )
{
    Bind(); 
    mMappedMem = glMapBuffer( mTarget, access );
    Unbind();
}

void GPUBuffer::ReleaseAccess()
{
    Bind();
    glUnmapBuffer( mTarget );
    mMappedMem = nullptr;
    Unbind();
}

void GPUBuffer::Alloc( size_t size, GLenum type, GLenum accessType )
{
    Bind( type );
    glBufferData( 
        type, size, nullptr, accessType 
    );

    mSize   = size;
    mType   = accessType;
    Unbind();
}

void GPUBuffer::Alloc( size_t size, GLenum type, GLenum accessType, void* data, size_t elemCount )
{
    Bind( type );
    glBufferData( 
        type, size, data, accessType 
    );

    mSize     = size;
    mType     = accessType;
    mElements = elemCount;
    Unbind();
}

void* GPUBuffer::data( ) 
{
    return mMappedMem;
}


}
