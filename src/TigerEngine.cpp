#include "TigerEngine.h"

/*
 * A few definitions for the engine 
 *
 * */


namespace TigerEngine
{

static JGL::glContext RENDER_CONTEXT;

void Terminate(  )
{
    RENDER_CONTEXT.Terminate();
    TigerEngine::OnExit();
}

void Initialize( const char* windowName, int windowWidth, int windowHeight )
{
    RENDER_CONTEXT = JGL::glContext( windowWidth, windowHeight, windowName );
    TigerEngine::OnLoad();
    TigerEngine::Main();
}

void Initialize( JGL::glContext& context )
{
    RENDER_CONTEXT = context;
    TigerEngine::OnLoad();
    TigerEngine::Main();
}

JGL::glContext& GetRenderContext()
{
    return RENDER_CONTEXT;
}

}
