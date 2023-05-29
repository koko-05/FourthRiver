#pragma once
#include "Object.h"
#include "JGL/Texture.h"
#include "Component.h"


/*
 * Provides the code for a texture to work in an object
 * but does not provide the shader code, nor Texture creation
 *
 * */


namespace FourthRiver
{

namespace Components
{

class Texture : 
    public Component
{
public:
    void Apply( JGL::Scene* sc ) override;
    void Merge( Object* dest, JGL::Scene* scene ) override;
    void Unmerge( Object* dest, JGL::Scene* scene ) override;

public:
    void SetTexture( JGL::Texture& t ) { texture = &t; }
    JGL::Texture* texture;

public:
    uint16_t GetID() const override { return mId; }
    static constexpr uint16_t mId = 5;

};

}

}

