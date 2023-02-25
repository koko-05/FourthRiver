#include "Component.h"
#include "Object.h"
#include "JGL/Scene.h"

/*
 * Defines what is written in Object header
 * */

namespace TigerEngine
{

void Object::Render( JGL::Scene* scene )
{
    // TODO: make a macro for this, the function doesnt work for capturing in lambda

    for ( size_t i = 0; i < ComponentManager::componentList.size; i++ )
        ComponentManager::componentList[i]->Apply( scene );


    scene->Render( *this );
}


}
