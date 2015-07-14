#version 430 core

layout (location = 0) out vec4 fragColor;

in VS_OUT
{
  flat int alien;
  vec2 tc;
}fs_in;

uniform sampler2DArray tex_array;

void main(void)
{
   //The fs_in.alien is the texture index in texture array 
   fragColor = texture(tex_array, vec3(fs_in.tc, float(fs_in.alien) ) );
}