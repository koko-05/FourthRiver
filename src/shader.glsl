// VERTEX SHADER
#version 330 core

FR_VAO_ATTRIBS

/* TODO: Turn this into a conditional to object flags  macro like FR_VAO_ATTRIBS */
layout (std140) uniform FR_LightingBlock
{
                            /* Base Align | Aligned Offset */
    vec3 LightsColors[256]; /*    16      |       0        */
    uint LightCount;        /*    4       |     4096       */
}

uniform mat4 uMVP;
out vec4 oColor;

void main()
{
    gl_Position = uMVP * vec4( pos, 1.0 );
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
