#pragma once
#include "Component.h"
#include "JGL/Shader.h"

/*
 * Shader component allows you to use deafult shader
 * or define one for your own, if you want a material system
 * inheret from MaterialShader instead of this
 *
 * Fair warning, Shader should be declared in inheretence BEFORE anything that may use shader
 * */

namespace JGL { class Scene; }

namespace TigerEngine { namespace Components {

    class Shader : public Component, public JGL::Shader
{
public:
    Shader();

    static constexpr uint16_t mId = 2;
    
public:
    void Apply( JGL::Scene* scene ) override;
    uint16_t GetID() const override;

};

}
}
