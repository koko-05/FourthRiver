#pragma once
#include "JM/JMath.h"
#include "JGL/Scene.h"
#include "Component.h"
#include "Lighting.h"
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

class LightEmitter
    : public Lighting
      public Shader
{
public:
    LightEmitter( const JGL::UBO& context );

    static constexpr uint16_t mId = 2;

public:
    virtual void UpdateInformation( );

public:
    void Apply( JGL::Scene* sc ) override;
    void Merge( Object* d, JGL::Scene* sc ) override;
    void Unmerge( Object* d, JGL::Scene* sc ) override;
    uint16_t GetID() const override { return mId; }

public:
    int index = -1;

};



}
}
