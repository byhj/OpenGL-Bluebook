#version 430 core

layout (location = 0) in int alien_index;

out VS_OUT
{
  flat int alien;
  vec2 tc; 
}vs_out;

//Drop dataStuct
struct droplet_t
{
  float x_offset;
  float y_offset;
  float orientation;
  float unused;
};

//UBO data
layout (std140, binding = 0) uniform droplets
{
   droplet_t droplet[256];
};

void main(void)
{
      const vec2[4] Position = vec2[4](vec2(-0.5, -0.5),    
                                       vec2( 0.5, -0.5),                 
                                       vec2(-0.5,  0.5),                 
                                       vec2( 0.5,  0.5));  

    vs_out.tc = Position[gl_VertexID].xy + vec2(0.5f);
	vs_out.alien = alien_index % 64;

	//Create a rotate Z axis matrix
    float co = cos(droplet[alien_index].orientation);                  
    float so = sin(droplet[alien_index].orientation);                  
    mat2 rot = mat2(vec2(co, so), vec2(-so, co)); 
	
	vec2 Pos = 0.25f * rot * Position[gl_VertexID];
	gl_Position = vec4(Pos.x + droplet[alien_index].x_offset,          
                       Pos.y + droplet[alien_index].y_offset,          
                       0.5, 1.0);  
}
