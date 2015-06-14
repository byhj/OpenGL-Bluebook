#version 410 core

in GS_OUT {
  vec4 color;
} fs_in;

out vec4 fragColor;

void main(void) 
{
        fragColor = fs_in.color;  
}