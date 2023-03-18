#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

#include "GPUBuffer.h"


/* 
 * Abstraction layer for VAOs in OpenGL
 * */

namespace JGL
{

/* Vertex Element used internally to keep track of what a buffer stores */
struct VertexElement
{
    GLenum type;
    GLuint count;
    bool   normalized;
};


/* Vertex Attribute abstracted class */
class VertexAttribute
{
public: /* Initialization and destruction, and copying */
     VertexAttribute() : mStride(0), mElements() {  }
    ~VertexAttribute()                                     = default;
    VertexAttribute(const VertexAttribute& v)              = default;
    VertexAttribute(VertexAttribute&& v)                   = default;
    VertexAttribute& operator=( const VertexAttribute& v ) = default;

public: /* add and remove interface TODO: add remove */
    void Add( GLenum _type, unsigned int _count, bool _normalized = false );


public: /* Getters */
    const std::vector< VertexElement >& GetArr() const;
    GLuint GetStride() const;

private:
    GLuint                       mStride;
    std::vector< VertexElement > mElements;
};


/* OpenGL manager for VertexAttibute */
class VertexArray
{
public: /* Initialization and destruction */
    VertexArray();
    VertexArray( VertexArray&& v );
    ~VertexArray();

    /* VAO shouldnt be copied, if you want the same attributes, just create a new
     * one and call VAO_dest.AddAttrib( VBO, VAO_src.getAttribs().at(index) )
     * */
    VertexArray( const VertexArray&  )           = delete;
    VertexArray& operator=( const VertexArray& ) = delete;

public:
    void AddAttrib( const GPUBuffer& vbo, const GPUBuffer& ibo, const VertexAttribute& _attrib );

private:
    struct Attribute;

public:
    void Bind() const;
    void UnBind() const;

    const std::vector<Attribute>& getAttribs() const;

private:
    GLuint mId = 0;

private:
    struct Attribute
    {
        Attribute( const VertexAttribute& va, const GPUBuffer* _vbo, const GPUBuffer* _ibo )
            : attrib( std::move( va ) ),
              vbo( _vbo ), ibo( _ibo )
        {  }

        VertexAttribute  attrib;

        const GPUBuffer* vbo;
        const GPUBuffer* ibo;
    };

    std::vector<Attribute> mAttributes;
};

}
