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
    /* you need to include ComponentManagerTemplates.cpp in order to use these */
    template<typename C> C& GetComponent();
    template<typename C> C* FindComponent();

    void ForEach_Component( void(*func)(Components::Component* c) );
};

}
}


