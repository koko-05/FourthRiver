#pragma once
#include "Object.h"
#include "JGL/Scene.h"


/*
 * Provides an interface to add components 
 * to an object through inheretence
 *
 *
 * */


namespace TigerEngine
{

namespace Components
{

class Component : 
    public virtual TigerEngine::Object
{
public:
    Component();
    virtual ~Component();

public:
    virtual void Apply( JGL::Scene* sc ) = 0; /* should be called on each render */
    virtual uint16_t GetID() const;

public:
    /* Every component must define the following with a unique id: */
    // constexpr uint16_t mId = 0;
    /* They should also define GetID() to return mId */
};

}

}

