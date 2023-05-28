#pragma once
#include "JGL/UniformBufferObject.h"


/*
 * Light context can be modefied if inhereted
 * from this class 
 *
 * */

#define MAX_LIGHTS 256

namespace FourthRiver
{

struct LightInformation
{
    JM::Vect3 color;
};

class LightContext
    : public JGL::UBO
{
public:
    virtual LightContext();
    virtual void CreateLightContext();
    virtual void AddLight();
    virtual void CreateLightMacro();

public:
    size_t size = 0;
    std::string MacroDef;
    std::string Macro = "FR_LIGHT_ATTRIBS";

};

}
