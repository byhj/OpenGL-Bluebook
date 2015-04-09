#version 430 core

layout (location = 0) out  vec4 color;

in vec2 tc;
uniform sampler2D tex_star; //Œ∆¿Ì
flat in vec4 starColor;

void main(void)
{
   color = starColor * texture(tex_star, tc);
}
