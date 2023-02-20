#pragma once
#include "JGL/JGL.h"
#include "JM/JMath.h"

/*
 * Defines an initalizer for the engine and a couple of user defined functions that will
 * be called when the engine is created
 *
 * This is engine is thought out to have a Application.cpp file that implements
 * the the methods, and various .cpp .h files that implement Scenes that get objects
 * from other .cpp .h files. 
 *
 * This engine is mainly to manage Scene creation and deletion, while keeping the same
 * rendering context 
 *
 * */

namespace TigerEngine
{
    void Initialize( const char* windowName, int windowWidth, int windowHeight );
    void Initialize( JGL::glContext& context );

    void Terminate();

    /* Scene managing */
    template< typename T, typename... _T >
    int64_t LoadScene( _T&&... params );
    JGL::glContext& GetRenderContext(); /* Scene constructor should be called with this */

    /* User Defined functions */
    void OnLoad();
    void OnExit();
    void Main();

    struct SceneData
    {
        Scene*  scene;
        int64_t exitStatus;
    }

    std::vector< std::unique_ptr<Scene> > Instances;
}

template< typename T, typename... _T >
SceneData& TigerEngine::LoadScene( _T&&... args )
{
    TigerEngine::Instances.emplace_back( 
            std::make_unique<T>( std::forward<T>(args)... ) );
    return scene.Start();
}

