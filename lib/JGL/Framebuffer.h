#pragma once
#include "JGL/OpenGL.h"
#include "JGL/Texture.h"


namespace JGL
{

/*
 * Abstracts the framebuffer into a class, 
 * that is stored with the context
 *
 * */

class Framebuffer
{
public:
     Framebuffer();
    ~Framebuffer();

//    _noCopy( Framebuffer ); // TODO

public:
    void Bind( GLenum type = GL_FRAMEBUFFER );
    void Unbind();

    bool CheckBuffer();

public:
    void SetSize( size_t sx, size_t sy );

    /* TODO: add more of these */
    void ColorAttachment();
    void DepthStencilAttachment();

    uint32_t BindColor();
    uint32_t BindDepthStencil();

public:
    unsigned int glId = 0;
    GLenum glTypeBind = 0;
    struct { size_t x = 0, y = 0; } Size;

public:
    /* TODO: make RenderBuffers possible */
    Texture texture_color;
    Texture texture_depth_stencil;
};

}
