#version 430 core

layout (location = 0) out vec4 fragColor;

in VS_OUT {
  vec2 tc;
}fs_in;

layout (binding = 0) uniform sampler2D tex;

void main(void)
{
  fragColor = texture2D(tex, fs_in.tc);
}