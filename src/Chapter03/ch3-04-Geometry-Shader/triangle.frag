#version 430 core

layout (location = 0) out vec4 fragColor;

in GEOM_OUT
{
  vec4 color;
}fs_in;

void main(void)
{
   fragColor = fs_in.color;
}