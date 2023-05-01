#pragma once
#include "JM/JMath.h"
#include "Component.h"

/*
 * Defines a Light Source Component for any
 * object 
 * */

#ifndef LIGHT_SHADER_SRC_PREFIX
#define LIGHT_SHADER_SRC_PREFIX FR_SHADER_LIGHT_SOURCE;
#endif // !LIGHT_SHADER_SRC

#define LIGHT_SHADER_SRC_FRAG LIGHT_SHADER_SRC_PREFIX##_FRAG
#define LIGHT_SHADER_SRC_VERT LIGHT_SHADER_SRC_PREFIX##_VERT

namespace FourthRiver
{
namespace Components
{

extern const char* LIGHT_SHADER_SRC_FRAG;
extern const char* LIGHT_SHADER_SRC_VERT;

class LightSource
    : public virtual Shader
{
public:
    LightSource( JM::Vect3 color );
    LightSource();

    static constexpr uint16_t mId = 4;

public:
    void Apply( JGL::Scene* sc ) override;
    void Merge( Object* d, JGL::Scene* sc ) override;
    void Unmerge( Object* d, JGL::Scene* sc ) override;
    uint16_t GetID() const override { return mId; }

public:
    JM::Vect3 Color;

};



}
}
