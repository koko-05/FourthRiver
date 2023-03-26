#pragma once
#include <stdint.h>
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

template<typename T, size_t c>
class c_arr
{
public:
    T values[c];

    T& operator[]( size_t index )
    {
        return values[index];
    }
};

class cList : public c_arr<Component*, MAX_COMPONENTS>
{
public:
    size_t size = 0;

public:
    void push_back( Components::Component* );
    void pop();

    Component** end();
    Component** begin();
};

class ComponentManager
{
public:
    cList componentList;

public:
    /* you need to include ComponentManagerTemplates.cpp in order to use these */
    template<typename C> C& GetComponent();
    template<typename C> C* FindComponent();
    Components::Component* FindComponentByID( uint32_t id );


    void ForEach_Component( void(*func)(Components::Component* c) );
};

}
}


