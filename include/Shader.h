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

namespace FourthRiver { 

namespace Components {

class Shader : public Component, protected JGL::Shader
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
    void CreateShader(); // alias for UpdateShader
    void UpdateShader();

public:
    void SetShader( JGL::Shader* shader );
    void SetShader( const std::string& vs, const std::string& fs );
    void SetMacro();

public:
    const std::string& V_Source() { return VertexSource; }
    const std::string& F_Source() { return FragmentSource; }

    const std::string& V_SourceComplete() { return mVertexSource; }
    const std::string& F_SourceComplete() { return mFragmentSource; }

public:
    JGL::Shader* mCurrentShader = nullptr;

private:
    std::string VertexSource   = DEF_SH_SRC_VERT;
    std::string FragmentSource = DEF_SH_SRC_FRAG;

private:
    std::string CreateStringMacro( const JGL::VertexElement& ve, size_t i );
    void CreateMacroDef( JGL::VertexArray& v );

    std::string GeneratedMacroDef = " ";

};

}
}
