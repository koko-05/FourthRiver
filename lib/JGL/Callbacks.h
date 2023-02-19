#pragma once

#include "functional"

/*
 * Decleration for Callback Manager class
 * This will manage callbacks in an easy and orderly fashion, so that you can define 
 * 'modules' that will run whenever the glfw callback is called
 *
 * Perhaps this was a bit unnecessary, in hindsight, this was _really_ unnecessary
 *
 * */


namespace JGL
{

class Scene;

/* Mouse Callback ( x, y ) */
using MouseMoveCallback = std::function<void(JGL::Scene&, double, double )>;
/* Key Press Callback ( key, scancode, action, modifiers ) */
using KeyPressCallback  = std::function<void(JGL::Scene&, int, int, int, int)>;
/* Win Resize Callback ( w, h ) */
using WinResizeCallback = std::function<void(JGL::Scene&, int, int )>;
/* Scroll Callback ( xOffset, yOffset ) */
using ScrollCallback    = std::function<void(JGL::Scene&, double, double)>;



class Callbacks
{
public:
    Callbacks( bool useDefaults = true );

public:
    void PerspectiveCameraMouseController();

private:
    void SetCallbacks( Scene* context );

public:
    /* used for Perspecive Camera Controller */
    float  mSensitivity = 0.1f;
    float lx = 0.0f, ly = 0.0f; /* last reported x and y (use only if Perspective Camera Mouse Controller is acrive) */

    std::vector< MouseMoveCallback > mMouseMove;
    std::vector< KeyPressCallback >  mKeyPress;
    std::vector< WinResizeCallback > mWinResize;
    std::vector< ScrollCallback >    mScroll;

private:
    friend class Scene;

    static void DefaultMouseMove(GLFWwindow* win, double x, double y );
    static void DefaultKey(GLFWwindow* win, int key, int scan, int action, int mods);
    static void DefaultWinResize(GLFWwindow* win, int w, int h);
    static void DefaultScroll(GLFWwindow* win, double xOffset, double yOffset);

    static void DefaultWindowResizeCallback( Scene& i, int w, int h );
    static void PersCameraMouse(Scene& i, double x, double y );

};

}
