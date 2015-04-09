#version 430 core

layout (location = 0) in vec4 position; //顶点和纹理坐标数据，由模型给出
layout (location = 4) in vec2 texCoord;

uniform mat4 mvp_matrix;
 
 out VS_OUT {
   vec2 tc;
}vs_out;

void main(void)
{
    gl_Position = mvp_matrix * position;
    vs_out.tc = texCoord; //将纹理坐标传入到下一个shader
}
   
