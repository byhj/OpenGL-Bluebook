#version 430 core

layout(location = 0) out vec4 fragColor;

uniform sampler2D tex;

void main(void)
{
   fragColor = texture(tex, gl_FragCoord.xy / textureSize(tex, 0));
}