#pragma once
#include <memory>
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
    struct SceneData;

    void Initialize( const char* windowName, int windowWidth, int windowHeight );
    void Initialize( JGL::glContext& context );

    void Terminate();

    /* Scene managing */
    template< typename T, typename... _T >
    SceneData LoadScene( _T&&... params );
    JGL::glContext& GetRenderContext(); /* Scene constructor should be called with this */

    /* User Defined functions */
    void OnLoad();
    void OnExit();
    void Main();

    struct SceneData
    {
        JGL::Scene*  scene;
        int64_t exitStatus;
    };

    extern std::vector< std::unique_ptr<JGL::Scene> > Instances;
}

template< typename T, typename... _T >
TigerEngine::SceneData TigerEngine::LoadScene( _T&&... args )
{
    /* c++ syntax for you, friend */
    TigerEngine::Instances.emplace_back( 
            new T( std::forward<_T>(args)... ) );
    auto status = TigerEngine::Instances.back().get()->Start();
    if ( status != SCENE_PAUSE ) {
        TigerEngine::Instances.pop_back();
        return { nullptr, status };
    }
    return { TigerEngine::Instances.back().get(), status };
}

