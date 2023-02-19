#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <initializer_list>

namespace JGL
{

/* 
 * OpenGL GPU Buffer  abstraction layer
 *
 * */
class GPUBuffer
{
public: /* Initialization, and destruction */
     GPUBuffer();
     GPUBuffer( const GPUBuffer& v );
     GPUBuffer( GPUBuffer&& v );
    ~GPUBuffer();
     GPUBuffer& operator=(GPUBuffer& v );

public: /* OpenGL interface */
    void Bind()   const;
    void Bind( GLenum target );
    void Unbind() const;

    void GetAccess( GLenum access ); /* buffer cant be used untill it's released */
    void ReleaseAccess();

    void Alloc( size_t size, GLenum type, GLenum accessType );
    void Alloc( size_t size, GLenum type, GLenum accessType, void* data, size_t elemCount);

public: /* Data interface */
    void* data( ); /* gets data, OpenAccess must be called beforehand, if modified, please set count*/
    template<class T>
    void* dataSet( std::initializer_list<T> src );

public: /* getters and setters */
    size_t   size()          const { return mSize; } /* byte size */
    size_t   count()         const { return mElements; } /* elements size */
    int32_t  gpuAccessType() const { return mType; }
    int32_t  Type()          const { return mTarget; }
    uint32_t id()            const { return mId; }

    /* should be used if element count has been modified using data() */
    void setCount( size_t v ) { mElements = v; } 


private:
    void*    mMappedMem = nullptr;
    size_t   mSize      = 0; // byte size
    size_t   mElements  = 0; // element size
    int32_t  mTarget    = 0; // tType 
    int32_t  mType      = 0; // accessType
    uint32_t mId        = 0; // openGL id

public: /* template definition */
    template<class T>
    void* dataSet( std::initializer_list<T>& src ) 
    {   
        const void* arr = (void*)&(*src.begin()); mElements = src.size();
        return memcpy( mMappedMem, arr, src.size() * sizeof( T ) );
    }

};

}
