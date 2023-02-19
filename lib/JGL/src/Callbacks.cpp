#include "JGL/Scene.h"
#include "JGL/Callbacks.h"

/*
 * Definitions for Callback class
 * */

namespace JGL
{

extern Scene* CALLBACK_INSTANCE_PARAMETER;

Callbacks::Callbacks( bool useDefaults )
{
    if ( useDefaults )
        mWinResize.push_back( &DefaultWindowResizeCallback );
}


void Callbacks::PerspectiveCameraMouseController()
{
    mMouseMove.push_back( &PersCameraMouse );
}

void Callbacks::PersCameraMouse(Scene& i, double x, double y )
{
    float& lx = i.mCallbackManager.lx;
    float& ly = i.mCallbackManager.ly;

    float dx = x - lx, dy = y - ly;
    dx *= i.mCallbackManager.mSensitivity;
    dy *= i.mCallbackManager.mSensitivity;

    lx = x; ly = y;

    i.GetCamera().MoveDirection( dx, dy );
}

void Callbacks::SetCallbacks( Scene* context )
{
    auto win = context->GetContext().renderContext().GetWindow();

    glfwSetKeyCallback( win, &DefaultKey );
    glfwSetCursorPosCallback( win, &DefaultMouseMove );
    glfwSetScrollCallback( win, &DefaultScroll );
    glfwSetWindowSizeCallback( win, &DefaultWinResize );
}

void Callbacks::DefaultWindowResizeCallback( Scene& i, int w, int h )
{
    glViewport(0, 0, w, h);

    /* modify the projection matrix if on Perspective */
    if ( i.GetCamera().mProjType != JM::ProjectionType::Perspective )
        return;

    auto& proj    = i.GetCamera().mProjection;
    auto& context = i.GetContext().renderContext();

    /* Gets the (1.0f / tan(fov * 0.5)) from the proj matrix */
    float frustumScale = proj.mVals[0][0] * ( (float)context.mSx / (float)context.mSy );
    proj.mVals[0][0]   = frustumScale / ( (float)w / (float)h);

    context.Size();
}

void Callbacks::DefaultMouseMove(GLFWwindow* win, double x, double y )
{
    UNUSED( win );
    auto& vect = JGL::CALLBACK_INSTANCE_PARAMETER->mCallbackManager.mMouseMove;

    for ( const auto& fun : vect )
        fun( *JGL::CALLBACK_INSTANCE_PARAMETER, x, y );

}

void Callbacks::DefaultKey(GLFWwindow* win, int key, int scan, int action, int mods)
{
    UNUSED( win );
    auto& vect = JGL::CALLBACK_INSTANCE_PARAMETER->mCallbackManager.mKeyPress;

    for ( auto fun : vect )
        fun( *JGL::CALLBACK_INSTANCE_PARAMETER, key, scan, action, mods );

}

void Callbacks::DefaultWinResize(GLFWwindow* win, int w, int h)
{
    UNUSED( win );
    auto& vect = JGL::CALLBACK_INSTANCE_PARAMETER->mCallbackManager.mWinResize;

    for ( auto fun : vect )
        fun( *JGL::CALLBACK_INSTANCE_PARAMETER, w, h );

}

void Callbacks::DefaultScroll(GLFWwindow* win, double xOffset, double yOffset)
{
    UNUSED( win );
    auto& vect = JGL::CALLBACK_INSTANCE_PARAMETER->mCallbackManager.mScroll;

    for ( auto fun : vect )
        fun( *JGL::CALLBACK_INSTANCE_PARAMETER, xOffset, yOffset );

}


}


