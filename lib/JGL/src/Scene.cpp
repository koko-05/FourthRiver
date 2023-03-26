#include <chrono>
#include "JGL/Scene.h"
#include "imgui/imgui.h"
#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"

/* 
 * Definitions for Scene context
 * */
namespace JGL
{


double SceneContext::millis() const 
{ 
    auto programNow = std::chrono::high_resolution_clock::now();
    return 
        (double)std::chrono::duration_cast<std::chrono::microseconds>
            ( programNow - mStartTime ).count() * .001;
}

double SceneContext::secs() const
{ 
    return millis() * 0.001;
}

      glContext& SceneContext::renderContext()       { return *mRenderContext; }
const glContext& SceneContext::renderContext() const { return *mRenderContext; }

int SceneContext::GetKey( int _keyVal ) const 
{
    return glfwGetKey( renderContext().GetWindow(), _keyVal );
}


}




/* 
 * Class definitions for Scene base class
 * */

namespace JGL
{

/* sets the instance for any glfw callback to use */
Scene* CALLBACK_INSTANCE_PARAMETER = nullptr; 

Scene::Scene( glContext& rContext ) 
    : mRenderContext( &rContext ), 
      mDefaultCamera( JM::ProjectionType::Perspective, 
              mRenderContext->Size ().width, mRenderContext->Size().height)
{

}

Scene::Scene( const Scene& v ) 
    : mRenderContext( v.mRenderContext ),
      mDefaultCamera( JM::ProjectionType::Perspective, 
              mRenderContext->Size ().width, mRenderContext->Size().height)
{

}

Scene& Scene::operator=( const Scene& v )
{ 
    if ( &v == this ) return *this;

    Scene temp( v );

    std::swap( temp.mRenderContext, mRenderContext );
    std::swap( temp.mDefaultCamera, mDefaultCamera );
    std::swap( temp.mCallbackManager, mCallbackManager );
    std::swap( temp.mRunningState, mRunningState );

    return *this;
}

Scene::Scene( Scene&& v )
    : mCallbackManager( std::move( v.mCallbackManager ) ),
      mRenderContext( v.mRenderContext ),
      mDefaultCamera( std::move( v.mDefaultCamera ) ),
      mRunningState( v.mRunningState )
{
}

int64_t Scene::Start( )
{
    mRunningState = true;
    mContext.mRenderContext = mRenderContext;
    Update( );
    return mExitStatus;
}

void Scene::Setup(  )
{
}

void Scene::Exit( int64_t exitCode )
{
    mExitStatus = exitCode;
    OnExit();
    mRunningState = false;
}


void Scene::Render( Object& obj, size_t offset ) const
{
    mDrawCallCounter++;
    obj.shader->Bind();
    obj.mesh->VAO.Bind();

    const GPUBuffer& IBO   = obj.mesh->IBO;
    const size_t     width = IBO.size() / IBO.count(); 
    GLenum indexSize       = 0;

    switch ( width )
    {
        case sizeof( GLuint   ): indexSize = GL_UNSIGNED_INT;   break; 
        case sizeof( GLushort ): indexSize = GL_UNSIGNED_SHORT; break; 
        case sizeof( GLubyte  ): indexSize = GL_UNSIGNED_BYTE;  break;
        default: ASSERT( false, "IBO is corrupt (width is: %ld)", width );  return; 
    }

    glDrawElements( 
        obj.mesh->Primitive, 
        IBO.count(), 
        indexSize, 
        (void*)offset
    );

    obj.mesh->VAO.UnBind();
}


void Scene::Update()
{
    /* waits half a second for input to stop */
    std::this_thread::sleep_for( std::chrono::milliseconds( SCENE_WAIT_TIME ) );

    auto   programStart = std::chrono::high_resolution_clock::now();
    double accumulator  = 0.0;
    double prevTime     = 0.0;
    Scene* prevInstance = this;

    mContext.mStartTime = programStart;

    /* clear input */
    std::swap( prevInstance, CALLBACK_INSTANCE_PARAMETER );
    glfwPollEvents();
    std::swap( prevInstance, CALLBACK_INSTANCE_PARAMETER );

    /* setup callbacks */
    mCallbackManager.SetCallbacks( mRenderContext );

    OnLoad();

    while ( mRunningState )
    {
        auto     programNow   = std::chrono::high_resolution_clock::now();
        double   programTime  = 
            std::chrono::duration_cast<std::chrono::microseconds>
            ( programNow - programStart ).count() * .001;

        if ( glfwWindowShouldClose( mRenderContext->GetWindow() ) )
            Exit();

        /* set info */
        mContext.FrameTime = programTime - prevTime;
        mContext.FPS       = (uint16_t)(1000.0 / mContext.FrameTime);
        mContext.DrawCalls = mDrawCallCounter; mDrawCallCounter = 0;
        mContext.F_FPS     = FIXED_UPDATE_FPS; // TODO: figure this 

        /* Frame setup */
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

        OnUpdate();

        /* cleanup for next frame */
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers( mRenderContext->GetWindow() );
        std::swap( prevInstance, CALLBACK_INSTANCE_PARAMETER );
        glfwPollEvents();
        std::swap( prevInstance, CALLBACK_INSTANCE_PARAMETER );

        /* Fixed Update calls */
        if ( !mRunningState ) return;

        constexpr double interval  = FIXED_UPDATE_INTERVAL;
        accumulator               += programTime - prevTime;
        prevTime                   = programTime;

        for ( ; accumulator >= interval; accumulator -= interval)
        {
            FixedUpdate( interval );
        }
    }
}

void Scene::FixedUpdate( float dt )
{
    OnFixedUpdate( dt );
}

void Scene::DefaultKeyboardCameraMove( float dt, float speed, float camSpeed )
{
    /* Camera Controll */
    auto& cam = GetCamera(); speed *= dt;
    const JM::Vect3 up       = { 0.0f,   speed,  0.0f };
    const JM::Vect3 down     = { 0.0f,   -speed, 0.0f};

    if ( GetContext().GetKey( GLFW_KEY_W ) == GLFW_PRESS )
        cam.Move( GetCamera().Direction * -speed );

    if ( GetContext().GetKey( GLFW_KEY_S ) == GLFW_PRESS )
        cam.Move( GetCamera().Direction *  speed );

    if ( GetContext().GetKey( GLFW_KEY_A ) == GLFW_PRESS )
        cam.Move( GetCamera().RightAxis * -speed );

    if ( GetContext().GetKey( GLFW_KEY_D ) == GLFW_PRESS )
        cam.Move( GetCamera().RightAxis *  speed );

    if ( GetContext().GetKey( GLFW_KEY_SPACE ) == GLFW_PRESS )
        cam.Move( up );

    if ( GetContext().GetKey( GLFW_KEY_LEFT_SHIFT ) == GLFW_PRESS )
        cam.Move( down );

    if ( GetContext().GetKey( GLFW_KEY_LEFT ) == GLFW_PRESS )
        cam.MoveDirection( speed * -camSpeed, 0.0f );
    if ( GetContext().GetKey( GLFW_KEY_RIGHT ) == GLFW_PRESS )
        cam.MoveDirection( speed *  camSpeed, 0.0f );
    if ( GetContext().GetKey( GLFW_KEY_UP ) == GLFW_PRESS )
        cam.MoveDirection( 0.0f, speed * -camSpeed );
    if ( GetContext().GetKey( GLFW_KEY_DOWN ) == GLFW_PRESS )
        cam.MoveDirection( 0.0f, speed * camSpeed );

}

}

