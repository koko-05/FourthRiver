#include "JGL/Renderer.h"
#include "JGL/common.h"

namespace JGL
{

Renderer::Renderer()
    : mGlContext()
{ }

Renderer::~Renderer()
{ }

Renderer::Renderer( Renderer&& _other)
    : mGlContext( std::move(_other.mGlContext) )
{ }

GL_Context& Renderer::GetContext() {
    return mGlContext;
}

bool Renderer::CreateWindow( const std::pair< int, int >& _dimentions, const char* _title) {
    return mGlContext.CreateWindow( _dimentions, _title );
} 


void Renderer::DrawCall( const BasicObject& _obj, const GLenum _GlMode, size_t _Indexoffset  ) const
{
    // binds everything
    _obj.ibo.Bind();
    _obj.vao.Bind();
    _obj.shader.Bind();

    // calculate the size of element in ibo
    // and from taht calculate if you are using uint ushort or uchar
    size_t indexSize = _obj.ibo.mCpuBufferSize / _obj.ibo.mElementCount; 
    GLenum type      = 0;

    switch ( indexSize )
    {
    case sizeof( GLuint ):   type = GL_UNSIGNED_INT;   break; 
    case sizeof( GLushort ): type = GL_UNSIGNED_SHORT; break; 
    case sizeof( GLubyte ):  type = GL_UNSIGNED_BYTE;  break;
    default: ASSERT( false, "Encountered a index type for ibo that is invalid (size is not 4, 2 or 1)!" ); return; 
    }

    glDrawElements( _GlMode, _obj.vertexCount, type, (const void*)_Indexoffset );
}

void Renderer::EndFrame()
{
    glfwSwapBuffers( mGlContext.mWindow );
    glfwPollEvents();
}


void Renderer::ClearScreen() const
{
    glClear( GL_COLOR_BUFFER_BIT );
}
    
}
