#pragma once
#include "Component.h"
#include "JGL/Shader.h"
#include "JGL/Scene.h"

/*
 * Shader component allows you to use deafult shader
 * or define one for your own, if you want a material system
 * inheret from MaterialShader instead of this
 *
 * Fair warning, Shader should be declared in inheretence BEFORE anything that may use shader
 * */



#ifndef DEF_SH_SRC_VERT
  #define DEF_SH_SRC_VERT DEFAULT_SHADER_SRC_VERT
#endif
#ifndef DEF_SH_SRC_FRAG
  #define DEF_SH_SRC_FRAG DEFAULT_SHADER_SRC_FRAG
#endif

extern const char* DEF_SH_SRC_VERT;
extern const char* DEF_SH_SRC_FRAG;

namespace FourthRiver { namespace Components {

    class Shader : public Component, public JGL::Shader
{
public:
    Shader();

    static constexpr uint16_t mId = 2;
    
public:
    void Apply( JGL::Scene* scene ) override;
    void Merge( Object* dest, JGL::Scene* scene ) override;
    void Unmerge( Object* dest, JGL::Scene* scene ) override;
    uint16_t GetID() const override;

public:
    void SetCurrentShader( JGL::Shader* shader );
    void SetMacroOnShader( JGL::Shader* shader );
    void SetLightingUniforms();
    JGL::Shader* mCurrentShader = nullptr;

    const char* DefaultSourceVertex   = DEF_SH_SRC_VERT;
    const char* DefaultSourceFragment = DEF_SH_SRC_FRAG;

private:
    std::string CreateStringMacro( const JGL::VertexElement& ve, size_t i );
    char* CreateMacroDef( JGL::VertexArray& v );
    char* lastGenMacroDef = nullptr;

};

}
}
