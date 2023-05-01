#include "LightSource.h"
#include "src/ComponentManagerTemplates.cpp"


namespace FourthRiver
{
namespace Components
{

const char* FR_SHADER_LIGHT_SOURCE_VERT= "\
  #version 330 core \n\
   \n\
    \n\
  FR_VAO_ATTRIBS\n\
   \n\
  uniform mat4 uMVP; \n\
  uniform int LightIndex; \n\
   \n\
  layout (std140) uniform FR_LightingBlock \n\
  { \n\
      vec3 LightsColors[256]; \n\
      uint LightCount;\n\
  };\n\
   \n\
  out vec3 oColor; \n\
   \n\
  void main() \n\
  { \n\
    gl_Position = uMVP * vec4( pos, 1.0 ); \n\
    oColor      = LightsColors[LightIndex]; \n\
  }";

const char* FR_SHADER_LIGHT_SOURCE_FRAG = "\
  #version 330 core \n\
    \n\
  layout ( location = 0 ) out vec4 color; \n\
  in vec3 oColor; \n\
   \n\
  void main() \n\
  { \n\
    color = vec4(oColor, 1.0); \n\
  } ";


LightSource::LightSource( JM::Vect3 color )
    : Color( color )
{
  Shader::DefaultSourceVertex   = LIGHT_SHADER_SRC_VERT;
  Shader::DefaultSourceFragment = LIGHT_SHADER_SRC_FRAG;
}


void LightSource::UpdateInformation( JGL::Scene* scene )
{
  if ( mIndex == -1 )
  {
    mIndex = (int)scene->mLightManager.Count;
    scene->mLightManager.AddLight( { Color } );
    return;
  }

  scene->mLightManager.GetColorLoc()[mIndex] = Color;
}

void LightSource::Apply( JGL::Scene* sc )
{
    UNUSED( sc );
    Component::Object::shader = mCurrentShader;

    if ( mIndex != -1 && shader->gl_Program )
      shader->SetUniform1i( "LightIndex", mIndex );
}

void LightSource::Merge( Object* d, JGL::Scene* sc )
{
  /* Be Carefull: this should be exactly the same as shader Merge, same goes to apply and unmerge */
    auto c = d->FindComponent<LightSource>();
    if ( c ) c->Apply( sc );
    else d->shader = mCurrentShader;
}

void LightSource::Unmerge( Object* d, JGL::Scene* sc )
{
    UNUSED( sc );
    d->shader = nullptr;
}

}
}
