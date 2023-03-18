#pragma once
#include <cstddef>
#include <array>

/* 
 * First Component Manager
 * */

namespace FourthRiver
{
namespace Components
{

class Component;
constexpr size_t MAX_COMPONENTS = 16;

class cList : public std::array<Component*, MAX_COMPONENTS>
{
public:
    size_t size = 0;

public:
    void push_back( Components::Component* );
    void pop();

};

class ComponentManager
{
public:
    cList componentList;

public:
    template<typename C> C& GetComponent();
    template<typename C> C* FindComponent();
    void ForEach_Component( void(*func)(Components::Component* c) );
};

}
}

/*
 * Template definitions
 * */

#ifndef DONT_COMPONENT_MGR_TEMPLATE_DEFINITIONS
#include "Component.h"

namespace FourthRiver
{
namespace Components
{


template<typename C>
C* ComponentManager::FindComponent()
{
    for ( auto& it = componentList.begin();
          it != componentList.end(); it++ )
    {
        if ( (*it)->GetID() == C::mId  )
            return *it;
    }

    return nullptr;
}

template<typename C>
C& ComponentManager::GetComponent()
{ 
    return *(this); // implicit cast should take care of errors
}


}
}
#endif
