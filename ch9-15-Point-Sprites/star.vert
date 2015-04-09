#version 430 core                                           

layout (location = 0) in vec4 position; 
layout (location = 1) in vec4 color; 

out vec2 tc;

uniform float time;    
uniform mat4 proj_matrix;

flat out vec4 starColor; 
                                               
void main(void)                                               
{               

vec2 tco[4] =  vec2[4](
	vec2(0.0f, 0.0f),
	vec2(1.0f, 0.0f),
	vec2(1.0f, 1.0f),
	vec2(0.0f, 1.0f)
);                                                
    vec4 newVertex = position;  
    newVertex.z += time;                                      
    newVertex.z = fract(newVertex.z);                         
                                                              
    float size = (20.0 * newVertex.z * newVertex.z);          
                                                              
    starColor = smoothstep(1.0, 7.0, size) * color;           
                                                              
    newVertex.z = (999.9 * newVertex.z) - 1000.0;             
    gl_Position = proj_matrix * newVertex;                    
    gl_PointSize = size;   
	tc =  tco[gl_VertexID].xy;                                  
}                                                             
                                                              