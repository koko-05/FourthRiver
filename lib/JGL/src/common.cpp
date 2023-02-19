
#include "JGL/common.h"

GLuint GetGLSize( GLenum _type )
{
    switch ( _type )
    {
        case GL_FLOAT:         return 4;
        case GL_INT:           return 4;
        case GL_UNSIGNED_INT:  return 4;
        case GL_UNSIGNED_BYTE: return 1;
        case GL_BYTE:          return 1;
        
        default: UNIMPLEMENTED;
    }
    return 0;
}
