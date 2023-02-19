#pragma once

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
    void Initialize( std::string windowName, int windowWidth, int windowHeight );
    void Initialize( JGL::glContext& context );

    /* Scene managing */
    template< typename T, typename... _T >
    int64_t LoadScene( _T&& params );
    JGL::glContext& GetGLContext(); /* Scene constructor should be called with this */

    /* User Defined functions */
    void OnLoad();
    void Main();
}

template< typename T, typename... _T >
int64_t TigerEngine::LoadScene<T, _T>( _T...&& params )
{
    T scene( std::forward( params... ) );
    return scene.Start();
}

}
