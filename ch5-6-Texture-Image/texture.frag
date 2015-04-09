#version 430 core

layout (location = 0) out vec4 fcolor;
uniform sampler2D tex;

in VS_OUT {
  vec2 tc;
}fs_in;

void main(void)
{
   fcolor = texture(tex, fs_in.tc); //获取纹理数据
}
