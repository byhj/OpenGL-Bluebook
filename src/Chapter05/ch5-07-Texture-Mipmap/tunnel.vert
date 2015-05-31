#version 430 core

out VS_OUT 
{
  vec2 tc;

}vs_out;

uniform mat4 mvp;
uniform float offset;

void main(void)
{
   const vec2[4]  Position = vec2[4](vec2(-0.5, -0.5), 
                                     vec2( 0.5, -0.5),
                                     vec2(-0.5,  0.5), 
									 vec2( 0.5,  0.5)
									 ); 

   vs_out.tc = (Position[gl_VertexID].xy + vec2(offset, 0.5) ) * vec2(30.0, 1.0);;

   gl_Position = mvp * vec4(Position[gl_VertexID], 0.0, 1.0);
}