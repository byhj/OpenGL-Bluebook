#version 410 core

in VS_OUT {
  vec4 color;
  vec2 texCoord;
} fs_in;

out vec4 fragColor;
uniform sampler2D tex; //��ǰ��һ��������Ϊ��������

void main(void) 
{
        fragColor = mix(fs_in.color, texture(tex, fs_in.texCoord), 0.7); 
}