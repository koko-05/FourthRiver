#include "ComponentManager.h"
#include "JGL/Scene.h"
#include "Component.h"

/*
 * Defines what is written in Object header
 * */

namespace FourthRiver
{
namespace Components
{

Component** cList::end()
{
    return &((*this)[size]);
}

Component** cList::begin()
{
    return &((*this)[0]);
}

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
    
Component* ComponentManager::FindComponentByID( uint32_t id )
{
    for ( auto& o : componentList )
    {
        if ( o->GetID() == id )
            return o;
    }

    return nullptr;
}


}

}
