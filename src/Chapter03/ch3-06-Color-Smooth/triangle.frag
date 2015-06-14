#version 430 core

layout (location = 0) out vec4 fragColor;

in VS_OUT
{
  flat vec4 flatColor;
  smooth vec4 smoothColor;
}fs_in;

uniform bool isFlat;

void main(void)
{
   if (isFlat)
      fragColor = fs_in.flatColor;
   else
      fragColor = fs_in.smoothColor;
}