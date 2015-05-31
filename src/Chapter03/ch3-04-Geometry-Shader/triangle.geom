#version 430 core

layout (triangles) in ;
layout (triangle_strip, max_vertices = 6) out;

in TES_OUT
{
  vec4 color;
}gm_in[];

out GEOM_OUT
{
   vec4 color;
}gm_out;

void main(void)
{
   for (int i = 0; i != gl_in.length(); ++i)
   {
      gm_out.color = gm_in[i].color;
      gl_Position = gl_in[i].gl_Position;
	  EmitVertex();		
   }
   EndPrimitive();

   for (int i = 0; i != gl_in.length(); ++i)
   {
      gm_out.color = gm_in[i].color;
      gl_Position =  gl_in[i].gl_Position * vec4(-1.0f, -1.0f, 1.0f, 1.0f);
	  EmitVertex();		
   }
   EndPrimitive();
}