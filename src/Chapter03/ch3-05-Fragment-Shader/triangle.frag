#version 430 core

layout (location = 0) out vec4 fragColor;

void main(void)
{
    //gl_FragCoord.x (0.5, 0.5)left bottom ---> (width-0.5, height - 0.5) right top

   //Run eacg fragment, which is interpolation by Rasterization
	vec4 Color = vec4(sin(gl_FragCoord.x * 0.5f) + 0.5f,
	                  cos(gl_FragCoord.y * 0.5f) + 0.5f,
	                  sin(gl_FragCoord.x * 0.15f) * cos(gl_FragCoord.y * 0.15f),
	                  1.0f);

   fragColor = Color;
}