
// VERTEX SHADER
// : look up sytax and semantics of GLS

#version 330 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inTexPos;

uniform mat4 uMVP;
out vec4 oColor;

void main()
{
    gl_Position = uMVP * vec4( inPosition, 1.0 );
    oColor      = vec4( 1.0 );
}


~
// FRAGMENT SHADER

#version 330 core

layout(location = 0) out vec4 color;
in vec4 oColor;


void main()
{
    color = oColor;
}
