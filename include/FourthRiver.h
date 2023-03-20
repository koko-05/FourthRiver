#pragma once
#include <memory>
#include "JGL/JGL.h"
#include "JM/JMath.h"

/*
 * Defines an initalizer for the engine and a couple of user defined functions that will
 * be called when the engine is created
 *
 * This is engine is thought out to have a Application.cpp file that implements
 * the scene, and various .cpp .h files that implement Scenes that get objects
 * from other .cpp .h files. 
 *
 * This engine is mainly to manage Scene creation and deletion, while keeping the same
 * rendering context 
 *
 * */

namespace FourthRiver
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
FourthRiver::SceneData FourthRiver::LoadScene( _T&&... args )
{
    /* c++ syntax for you, friend */
    FourthRiver::Instances.emplace_back( 
            new T( std::forward<_T>(args)... ) );
    auto status = FourthRiver::Instances.back().get()->Start();
    if ( status != SCENE_PAUSE ) {
        FourthRiver::Instances.pop_back();
        return { nullptr, status };
    }
    return { FourthRiver::Instances.back().get(), status };
}

