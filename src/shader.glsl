
// VERTEX SHADER
// : look up sytax and semantics of GLS

#version 330 core

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inVertColor;
layout (location = 2) in vec2 inTexPos;

uniform vec4 uColor;
uniform mat4 uMVP;

out vec4 oColor;
out vec2 TexturePos;

void main()
{
    gl_Position = uMVP * vec4( inPosition, 1.0 );

    TexturePos  = inTexPos;
    oColor      = vec4( inVertColor, 1.0 );
    oColor.w =  uColor.w;
}


~
// FRAGMENT SHADER

#version 330 core

layout(location = 0) out vec4 color;

uniform sampler2D textureId;

in vec2 TexturePos;
in vec4 oColor;


void main()
{
    color = oColor * texture( textureId, TexturePos );
}
