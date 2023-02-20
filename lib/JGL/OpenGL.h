#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"
#include "common.h"
#include <utility>

namespace JGL
{

/*
 * OpenGL context creation abstraction using GLFW
 *
 * */

class glContext
{
public: /* Initialization and destruction */
     glContext();
     glContext( size_t sx, size_t sy, const char* title );
     glContext( glContext&& _other );
    ~glContext();

    glContext( const glContext& c );
    glContext& operator=( const glContext& _o );
    glContext& operator=( glContext&& _o );

    void Setup();
    void Terminate();

public: /* Window creation */
    bool CreateWindow( size_t sx, size_t sy, const char* title, bool useDefaults = true );

    void SetWindowHint( int _hint, int _value);
    void SetErrorCallback( GLFWerrorfun _callBack );

public: /* GL configs */
    void SetBlendingFunction(GLenum _src, GLenum _dst, GLenum _mode);

public: /* getters and setters */
    GLFWwindow* GetWindow() const { return mWindow; }
    struct SizeS { int width, height; };
    SizeS Size();


public: /* these will only be updated if Size() is called */
    int mSx = 0, mSy = 0;

private:
    GLFWwindow* mWindow = nullptr;
    bool   mDestroyWindow = false;

    static void ErrorCallback( int _ErrorCode, const char* _Desc );
    static void ResizeCallback( GLFWwindow* _window, int _w, int _h );
};


}
