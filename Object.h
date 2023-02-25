#pragma once
#include "JM/JMath.h"
#include "JGL/Scene.h"

#define DONT_COMPONENT_MGR_TEMPLATE_DEFINITIONS
#include "ComponentManager.h"
#undef DONT_COMPONENT_MGR_TEMPLATE_DEFINITIONS

namespace JGL { class Scene; }

namespace TigerEngine
{

namespace Components { class Component; }

class Object : 
    public virtual JGL::Object, 
    public virtual Components::ComponentManager
{
public:
    using Matrix = JM::Mat<4,4>;

public:
    Matrix mvp_data = JM::DoNothingMatrix();

public:
    virtual void Render( JGL::Scene* scene );

};

}
