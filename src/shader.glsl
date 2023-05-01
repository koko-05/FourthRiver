// VERTEX SHADER
#version 330 core

FR_VAO_ATTRIBS

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
