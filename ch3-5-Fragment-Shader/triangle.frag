#version 430 core

layout (location = 0) out vec4 fragColor;  //Æ¬ÔªÑÕÉ«

void main()
{ 
   fragColor = vec4(sin(gl_FragCoord.x * 0.25) * 0.5 + 0.5,
                    cos(gl_FragCoord.y * 0.25) * 0.5 + 0.5,
					sin(gl_FragCoord.x * 0.15) * cos(gl_FragCoord.y) * 0.15,
					1.0);
}