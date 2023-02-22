#pragma once
#include <thread>
#include <chrono>
#include "common.h"
#include "OpenGL.h"
#include "GPUBuffer.h"
#include "Shader.h"
#include "Texture.h"
#include "VertexArray.h"
#include "JM/JMath.h"
#include "Transform.h"
#include "Camera.h"
#include "Callbacks.h"

/* 
 * Scene abstraction for OpenGL
 * rendering applications
 *
 * */

#define FIXED_UPDATE_FPS 30
#define FIXED_UPDATE_INTERVAL 1000 / FIXED_UPDATE_FPS;

#define SCENE_LOAD_FAILURE -1
#define SCENE_PAUSE -2

namespace JGL
{

struct Mesh;
struct Object;
class  Scene;

// TODO: make this into its won file, get it from disk, etc 
// add dynamic geometry, add a combineMesh() that will allocate everything on 
// the CPU, and upload it onto the GPU once it has combined everyhting  ( for batch rendering  )etc 
struct Mesh
{
    GPUBuffer   VBO;
    GPUBuffer   IBO;
    VertexArray VAO;

    GLenum      Primitive;
};

struct Object
{
    Mesh*   mesh;
    Shader* shader; // TODO: make a material 

    Transform transform;

private:
    uint32_t glMvpId = 0;
    friend class Scene;
};

struct SceneContext
{
    uint16_t FPS;
    uint16_t F_FPS;
    uint16_t DrawCalls;

    /* Time */
    double FrameTime;
    double millis() const;
    double secs() const;

    /* Render */
          glContext& renderContext();
    const glContext& renderContext() const;

    /* GLFW Input */
    int GetKey( int _keyVal ) const;

public:
    SceneContext() {}

private:
    std::chrono::high_resolution_clock::time_point mStartTime;
    glContext* mRenderContext;
    friend class Scene;
};

/* 
 * A child class should be created and initialization of said class is
 * up to the user 
 * */
class Scene
{
public: /* Initialization and destruction */
    Scene( glContext& rContext );
    Scene( Scene&& v );
    Scene( );
    virtual ~Scene() {}

    /* Copy */
    Scene( const Scene& v );
    Scene& operator=( const Scene& v ); 

protected: /* logic */
    virtual void OnLoad()                  {}
    virtual void OnUpdate()                {}
    virtual void OnFixedUpdate( float dt ) { (void)dt; }

    virtual void OnExit() {};

public: /* Execution */
    int64_t Start(); 
    void    Exit( int64_t exitStatus = 0 );
    void    Pause()     { Exit( SCENE_PAUSE ); }
    void    Unpause()   { Start(); }


public: /* Rendering */
    void Render( Object& obj, size_t offset = 0) const;
    void SetCamera( Camera& cam ) { mCamera = &cam; }
          Camera& GetCamera()       { return *mCamera; }
    const Camera& GetCamera() const { return *mCamera; }
    const Camera& GetDefaultCamera() const { return mDefaultCamera; }



public: /* Information */
    const SceneContext& GetContext() const { return mContext; }
    SceneContext& GetContext() { return mContext; }


private: /* private loops */
    void Update( );
    void FixedUpdate( float dt );
    void Setup( );

public: /* Callback manager */
    Callbacks     mCallbackManager;
    size_t        mExitStatus = 0; /* -1 reserverd for scene instatiotion error */

public:
    glContext*    mRenderContext;

private:
    SceneContext  mContext;
    Camera        mDefaultCamera;

    Camera*          mCamera          = &mDefaultCamera;
    bool             mRunningState    = 0;
    mutable uint16_t mDrawCallCounter = 0;

};

}

