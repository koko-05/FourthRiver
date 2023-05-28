#pragma once 
#include "JGL/UniformBufferObject"
#include "LightContext.h"


/*
 * Provides context for lighting operations
 *
 * */

namespace FourthRiver
{

class Lighting 
{
public:
    static constexpr uint16_t mId = 4;

public:
    JGL::LightContext& Context;

    LightInformation LightAttributes;

public:
    void Apply( JGL::Scene* sc ) override {}
    void Merge( Object* dest, JGL::Scene* scene ) override {}
    void Unmerge( Object* dest, JGL::Scene* scene ) override {}
    uint16_t GetID() const override { return mId; }

};

}
