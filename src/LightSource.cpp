#include "LightSource.h"


namespace FourthRiver
{
namespace Components
{

const char* FR_SHADER_LIGHT_SOURCE_VX = "\
  #version 330 core \
   \
  layout ( location = 0 ) in vec3 pos; \
  layout ( location = 1 ) in vec2 uv; \
  layout ( location = 2 ) in vec3 norm; \
  FR_VAO_ATTRIBS;\
   \
  uniform mat4 uMVP; \
  uniform vec4 color; \
  uniform float brightness; \
   \
  out vec4 oColor \
   \
  void main() \
  { \
    gl_Position = uMVP * vec4( pos, 1.0 ); \
    oColor      = color * brightness; \
  }";

const char* FR_SHADER_LIGHT_SOURRCE_FRAG = "\
  #version 330 core \
  layout ( location = 0 ) out vec4 color; \
  in vec4 oColor; \
   \
  void main() \
  { \
    color = oColor; \
  } ";


LightSource::LightSource( JM::Vect3 color )
    : Color( color ),
      Shader( LIGHT_SHADER_SOURCE )
{  }

}
}
