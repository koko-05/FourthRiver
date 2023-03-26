#include "JGL/OpenGL.h"
#include "JGL/common.h"
#include <iostream>

/* 
 * Definitions for glContext
 *
 * */

namespace JGL
{


glContext::glContext()
{
    ASSERT(glfwInit(), "glfwInit() has failed");
    SetErrorCallback( ErrorCallback );
}

glContext::glContext( const glContext& c )
    : mSx( c.mSx ), mSy( c.mSy ), mWindow( c.mWindow )
{  }

glContext& glContext::operator=( glContext&& _o )
{
    if ( this == &_o ) return *this;

    glContext temp( *this );

    mWindow    = _o.mWindow;
    mSx        = _o.mSx;
    mSy        = _o.mSy;
    _o.mWindow = nullptr;
    _o.mSy     = 0;
    _o.mSx     = 0;

    return *this;
}

glContext& glContext::operator=( const glContext& _o )
{
    if ( this == &_o ) return *this;

    glContext temp( _o );

    std::swap( mWindow, temp.mWindow );
    std::swap( mSy, temp.mSy );
    std::swap( mSx, temp.mSx );

    return *this;
}

glContext::~glContext()
{
}

glContext::glContext( size_t sx, size_t sy, const char* title )
{
    ASSERT(glfwInit(), "glfwInit() has failed");
    SetErrorCallback( ErrorCallback );

    CreateWindow( sx, sy, title );
}

void glContext::Setup() 
{
    /* ImGui setup */
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(GetWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 330 core");

}

void glContext::Terminate()
{
    if ( mWindow ) 
    {
        /* ImGui cleanup */
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        /* glfw cleanup */
        glfwSetWindowShouldClose( mWindow, GLFW_TRUE );
        glfwTerminate();

        mWindow = nullptr;
    }
}


bool glContext::CreateWindow( size_t sx, size_t sy, const char* title, bool useDefaults )
{
    /* set default Hints */
    if ( useDefaults ) 
    {
        SetWindowHint( GLFW_DOUBLEBUFFER,          GLFW_TRUE ); 
        SetWindowHint( GLFW_CLIENT_API,            GLFW_OPENGL_API ); 
        SetWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 ); 
        SetWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
        SetWindowHint( GLFW_OPENGL_PROFILE,        GLFW_OPENGL_CORE_PROFILE);
    }

    mWindow = glfwCreateWindow(sx, sy,title, NULL, NULL );
    ASSERT( mWindow, "Could not create window!" );
    mSx = sx; mSy = sy;

    glfwMakeContextCurrent( mWindow );
    ASSERT( gladLoadGLLoader( (GLADloadproc) glfwGetProcAddress ), 
            "Cannot load OpenGL function pointers!");

    glfwSetWindowSizeCallback( mWindow, ResizeCallback );

    DEBUG_PRINT( "Initialized OpenGL!\n", "[INFO]", stdout );

    SetBlendingFunction( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_FUNC_ADD );
    glEnable( GL_DEPTH_TEST );
    glfwSwapInterval( 1 );


    Setup();

    return true;
}

void glContext::SetWindowHint( int _hint, int _value)
{
    glfwWindowHint( _hint, _value );
}

void glContext::SetErrorCallback( GLFWerrorfun _callBack )
{
    glfwSetErrorCallback( _callBack );
}

void glContext::SetBlendingFunction(GLenum _src, GLenum _dst, GLenum _mode)
{
    glEnable( GL_BLEND );
    glBlendFunc( _src, _dst );
    glBlendEquation( _mode );
}

glContext::SizeS glContext::Size()
{
    glfwGetWindowSize( mWindow, &mSx, &mSy );
    return { mSx, mSy };
}


void glContext::ErrorCallback( int _ErrorCode, const char* _Desc )
{
    std::cout << "[GLFW][ERROR] " << _Desc << " ( Code: " << _ErrorCode << ')' << std::endl;
}

void glContext::ResizeCallback( GLFWwindow* _window, int _w, int _h )
{
    UNUSED( _window);
    glViewport(0, 0, _w, _h);
} 
}
