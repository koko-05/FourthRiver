#include "LightEmitter.h"
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


LightEmitter::LightEmitter( const JGL::UBO& context )
{
  SetShader( LIGHT_SHADER_SRC_VERT, LIGHT_SHADER_SRC_FRAG );
}


void LightEmitter::UpdateInformation(  )
{
  if ( mIndex == -1 )
  {
    index = (int)context.size;
    context.AddLight( &LightAttributes );
    return;
  }

  context.Get<UBO::Array<JM::Vect3>>()[index] = LightAttributes.Color;
}

void LightEmitter::Apply( JGL::Scene* sc )
{
    UNUSED( sc );
    Component::Object::shader = mCurrentShader;

    if ( mIndex != -1 && shader->gl_Program )
      shader->SetUniform1i( "LightIndex", mIndex );
}

void LightEmitter::Merge( Object* d, JGL::Scene* sc )
{
  /* Be Carefull: this should be exactly the same as shader Merge, same goes to apply and unmerge */
    auto c = d->FindComponent<LightEmitter>();
    if ( c ) c->Apply( sc );
    else d->shader = mCurrentShader;
}

void LightEmitter::Unmerge( Object* d, JGL::Scene* sc )
{
    UNUSED( sc );
    d->shader = nullptr;
}

}
}
