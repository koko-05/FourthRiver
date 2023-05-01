#pragma once
#include "JM/JMath.h"
#include "GPUBuffer.h"

#ifndef MAX_LIGHTS
#define MAX_LIGHTS 256
#endif

/*
 * Declares a manager for managing light sources within a scene,
 * globally through Uniform Buffer Objects 
 *
 * */

namespace JGL
{

struct LightInfo
{
    JM::Vect3 color;
};

class LightManager
{
public:
    void AddLight( const LightInfo& l );
    void UpdateBuffer();


public:
    JM::Vect3* GetColorLoc();
    unsigned int* GetSizeLoc();

public:
    static constexpr size_t colorOffset = 0;
    static constexpr size_t sizeOffset  = MAX_LIGHTS * 16;

    static constexpr size_t BlockSize = sizeOffset + 4;

public:
    uint8_t lightData[BlockSize] = { 0 };
    size_t Count = 0;

    GPUBuffer UniformBuffer;
};
    
}
