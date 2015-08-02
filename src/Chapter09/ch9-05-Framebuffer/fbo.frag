#version 430 core

in VS_OUT {  //Õý³£»æÖÆ
  vec4 color;
  vec2 texCoord;
} fs_in;

out vec4 fcolor;

void main(void) 
{
        fcolor = sin(fs_in.color * vec4(40.0, 20.0, 30.0, 1.0)) * 0.5 + vec4(0.5);
}