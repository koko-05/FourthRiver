#pragma once
#include "JM/JMath.h"
#include "JGL/Scene.h"
#include "Component.h"
#include "Shader.h"

/*
 * Defines a Light Source Component for any
 * object 
 *
 * */

#ifndef LIGHT_SHADER_SRC_PREFIX
#define LIGHT_SHADER_SRC_PREFIX 
#endif // !LIGHT_SHADER_SRC


#ifndef LIGHT_SHADER_SRC_FRAG
  #define LIGHT_SHADER_SRC_FRAG FR_SHADER_LIGHT_SOURCE_FRAG
#endif
#ifndef LIGHT_SHADER_SRC_VERT
  #define LIGHT_SHADER_SRC_VERT FR_SHADER_LIGHT_SOURCE_VERT
#endif

extern const char* LIGHT_SHADER_SRC_FRAG;
extern const char* LIGHT_SHADER_SRC_VERT;

namespace FourthRiver
{
namespace Components
{

class LightSource
    : public Shader
{
public:
    LightSource( JM::Vect3 color );

    static constexpr uint16_t mId = 2;

public:
    void UpdateInformation( JGL::Scene* scene );

public:
    void Apply( JGL::Scene* sc ) override;
    void Merge( Object* d, JGL::Scene* sc ) override;
    void Unmerge( Object* d, JGL::Scene* sc ) override;
    uint16_t GetID() const override { return mId; }

public:
    JM::Vect3 Color;
    int mIndex = -1;

};



}
}
