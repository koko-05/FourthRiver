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

Scene::Scene( size_t sx, size_t sy, const char* name )
    : mRenderContext( sx, sy, name ), 
      mDefaultCamera( JM::ProjectionType::Perspective, 
              mRenderContext.Size ().width, mRenderContext.Size().height)
{
    Setup();
}

Scene::Scene(  ) 
    : mRenderContext(), 
      mDefaultCamera( JM::ProjectionType::Perspective, 
              mRenderContext.Size ().width, mRenderContext.Size().height)
{
}

Scene::Scene( glContext& rContext ) 
    : mRenderContext( rContext ), 
      mDefaultCamera( JM::ProjectionType::Perspective, 
              mRenderContext.Size ().width, mRenderContext.Size().height)
{

}

Scene::Scene( const Scene& v ) 
    : mRenderContext( v.mRenderContext ),
      mDefaultCamera( JM::ProjectionType::Perspective, 
              mRenderContext.Size ().width, mRenderContext.Size().height)
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
      mRenderContext( std::move(v.mRenderContext) ),
      mDefaultCamera( std::move( v.mDefaultCamera ) ),
      mRunningState( v.mRunningState )
{
}

void Scene::Start( )
{
    mRunningState = true;
    Update( );
}

void Scene::Setup(  )
{

    /* ImGui setup */
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(mRenderContext.GetWindow(), true);
    ImGui_ImplOpenGL3_Init("#version 330 core");
}

void Scene::Exit()
{
    OnExit();
    mRunningState = false;
}


void Scene::Render( Object& obj, size_t offset ) const
{
    mDrawCallCounter++;
    obj.mesh->IBO.Bind();
    obj.mesh->VAO.Bind();
    obj.shader->Bind();

    const GPUBuffer& IBO   = obj.mesh->IBO;
    const size_t     width = IBO.size() / IBO.count(); 
    GLenum indexSize       = 0;

    switch ( width )
    {
        case sizeof( GLuint   ): indexSize = GL_UNSIGNED_INT;   break; 
        case sizeof( GLushort ): indexSize = GL_UNSIGNED_SHORT; break; 
        case sizeof( GLubyte  ): indexSize = GL_UNSIGNED_BYTE;  break;
        default: ASSERT( false, "IBO seems to be currupted" );  return; 
    }

    /* TODO: optimize MVP creation */
    if ( !obj.glMvpId )
    {
        obj.glMvpId = 
            obj.shader->SetUniformMat4( "uMVP", 
            (obj.transform.GetMatrix() * GetCamera().View() * GetCamera().Projection() ));
    }
    else
    {
        auto mat = 
            GetCamera().Projection() * GetCamera().View()  * obj.transform.GetMatrix();
;
        obj.shader->
            SetUniformMat4( obj.glMvpId, mat.Transpose());
    }

    glDrawElements( obj.mesh->Primitive, IBO.count(), indexSize, (void*)offset);
}


void Scene::Update()
{
    auto   programStart = std::chrono::high_resolution_clock::now();
    double accumulator  = 0.0;
    double prevTime     = 0.0;
    Scene* prevInstance = this;

    mContext.mRenderContext = &mRenderContext;
    mContext.mStartTime     = programStart;

    /* setup callbacks */
    mCallbackManager.SetCallbacks( this );

    OnLoad();

    while ( mRunningState )
    {
        auto     programNow   = std::chrono::high_resolution_clock::now();
        double   programTime  = 
            std::chrono::duration_cast<std::chrono::microseconds>
            ( programNow - programStart ).count() * .001;

        if ( glfwWindowShouldClose( mRenderContext.GetWindow() ) )
            mRunningState = false;

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
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers( mRenderContext.GetWindow() );
        std::swap( prevInstance, CALLBACK_INSTANCE_PARAMETER );
        glfwPollEvents();
        std::swap( prevInstance, CALLBACK_INSTANCE_PARAMETER );

        /* Fixed Update calls */
        constexpr double interval  = FIXED_UPDATE_INTERVAL;
        accumulator               += programTime - prevTime;
        prevTime                   = programTime;

        for ( ; accumulator >= interval; accumulator -= interval)
        {
            FixedUpdate( interval );
        }
    }

    OnExit();

    /* ImGui cleanup */
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void Scene::FixedUpdate( float dt )
{
    OnFixedUpdate( dt );
}


}

