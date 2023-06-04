#include "JGL/Framebuffer.h"


namespace JGL
{

Framebuffer::Framebuffer()
{
    glGenFramebuffers( 1, &glId );
}

Framebuffer::~Framebuffer()
{
    glDeleteFramebuffers( 1, &glId );
}

void Framebuffer::Bind( GLenum type )
{
    glBindFramebuffer( type, glId );
    glTypeBind = type;
}

void Framebuffer::Unbind()
{
    glBindFramebuffer( glTypeBind, 0 );
}

bool Framebuffer::CheckBuffer()
{
    return glCheckFramebufferStatus( glTypeBind ) == GL_FRAMEBUFFER_COMPLETE;
}

void Framebuffer::SetSize( size_t sx, size_t sy )
{
    Size.x = sx;
    Size.y = sy;
}

void Framebuffer::ColorAttachment()
{
    texture_color.mTarget = GL_TEXTURE_2D;
    texture_color.Bind(0);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Size.x, Size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFramebufferTexture2D( glTypeBind, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture_color.mId, 0 );

}

void Framebuffer::DepthStencilAttachment()
{
    texture_depth_stencil.mTarget = GL_TEXTURE_2D;
    texture_depth_stencil.Bind(1);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, Size.x, Size.y, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFramebufferTexture2D( glTypeBind, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, texture_depth_stencil.mId, 0 );


}

uint32_t Framebuffer::BindColor()
{
    texture_color.Bind( texture_color.GetSlot() );
    return texture_color.GetSlot();
}

uint32_t Framebuffer::BindDepthStencil()
{
    texture_depth_stencil.Bind( texture_depth_stencil.GetSlot() );
    return texture_depth_stencil.GetSlot();

}

}
