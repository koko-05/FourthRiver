#pragma once
#include "JM/JMath.h"
#include "JGL/Scene.h"

#define DONT_COMPONENT_MGR_TEMPLATE_DEFINITIONS
#include "ComponentManager.h"
#undef DONT_COMPONENT_MGR_TEMPLATE_DEFINITIONS

#define OBJECT_MVPID_CACHE_BIT 0x00000001
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
    using Matrix = JM::Mat<4,4>;

public:
    Matrix  mvp_data       = JM::DoNothingMatrix();
    int32_t cached_MVPid   = 0;
    uint8_t flags          = 0;
    char    name[MAX_OSIZE]= "UNNAMED";

public:
    virtual void Render( JGL::Scene* scene );

};

}
