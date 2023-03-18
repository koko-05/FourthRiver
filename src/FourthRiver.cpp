#include "FourthRiver.h"

/*
 * A few definitions for the engine 
 *
 * */


namespace FourthRiver
{

static JGL::glContext RENDER_CONTEXT;
std::vector< std::unique_ptr<JGL::Scene> > Instances;

void Terminate(  )
{
    FourthRiver::Instances.clear();
    RENDER_CONTEXT.Terminate();
    FourthRiver::OnExit();
}

void Initialize( const char* windowName, int windowWidth, int windowHeight )
{
    RENDER_CONTEXT = JGL::glContext( windowWidth, windowHeight, windowName );
    FourthRiver::OnLoad();
    FourthRiver::Main();
}

void Initialize( JGL::glContext& context )
{
    RENDER_CONTEXT = context;
    FourthRiver::OnLoad();
    FourthRiver::Main();
}

JGL::glContext& GetRenderContext()
{
    return RENDER_CONTEXT;
}

}
