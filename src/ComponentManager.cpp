#include "ComponentManager.h"
#include "JGL/Scene.h"

/*
 * Defines what is written in Object header
 * */

namespace TigerEngine
{
namespace Components
{

void cList::push_back( Components::Component* c )
{
    (*this)[size++] = c;
}

void cList::pop()
{
    (*this)[size--] = nullptr;
}

void ComponentManager::ForEach_Component( void(*func)(Components::Component* c) )
{
    for ( auto& o : componentList )
    {
        func( o );
    }
}
    


}

}
