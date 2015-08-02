#version 410 core

in VS_OUT {
  vec4 color;
  vec2 texCoord;
} fs_in;

out vec4 fragColor;
uniform sampler2D tex; //将前面一个物体作为纹理数据

void main(void) 
{
        fragColor = mix(fs_in.color, texture(tex, fs_in.texCoord), 0.7); 
}