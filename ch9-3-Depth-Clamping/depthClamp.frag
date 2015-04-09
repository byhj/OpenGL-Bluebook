#version 410 core

// Output
layout (location = 0) out vec4 fragColor;

// Input from vertex shader
in VS_OUT
{
    vec3 normal;
	vec4 color;
} fs_in;

void main(void)
{
   fragColor = vec4(1.0) * abs(normalize(fs_in.normal).z); //深度值确定颜色深浅
}
