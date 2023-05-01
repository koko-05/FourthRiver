#pragma once
#include "JM/JMath.h"
#include "JGL/Scene.h"
#include "ComponentManager.h"

// TODO: allow adding more components through dyneamic mem alloc

#define OBJECT_MVPID_CACHE_BIT 0x00000001
#define OBJECT_LIGHTING_EFFECT_BIT 0x00000002
#define OBJECT_DEFAULT_FLAGS OBJECT_LIGHTING_EFFECT_BIT | 0x0
#define MAX_OSIZE 16

namespace JGL { class Scene; }

namespace FourthRiver
{

namespace Components { class Component; }

class Object : 
    public virtual JGL::Object, 
    public virtual Components::ComponentManager
{
public:
    using Matrix = JM::Matrix<4,4>;

public:
    virtual ~Object() {}

public:
    Matrix  mvp_data       = JM::DoNothingMatrix();
    int32_t cached_MVPid   = 0;
    uint8_t flags          = OBJECT_DEFAULT_FLAGS;
    char    name[MAX_OSIZE]= "UNNAMED";

public:
    virtual void Render( JGL::Scene* scene );

};

}
