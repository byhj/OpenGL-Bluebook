#version 430 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_OUT {
  vec3 normal;
  vec4 color;
}gs_in[];  //array

out vec4 color;
uniform mat4 mvp_matrix;
uniform mat4 mv_matrix;
uniform vec3 viewpoint;

void main()
{
   int n;
   //calc the normal vector
   vec3 ab = gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz;
   vec3 ac = gl_in[2].gl_Position.xyz - gl_in[1].gl_Position.xyz;
   vec3 normal  = normalize(cross(ab, ac));

   vec3 vNormal = (mat3(mv_matrix) * normal);
   vec4 worldSpace = gl_in[0].gl_Position;
   vec3 vt = normalize(viewpoint - worldSpace.xyz);

   if (dot(normal, vt) > 0.0)
   {
      for (int n = 0; n != 3; ++n)
	  {
	     gl_Position = mvp_matrix * gl_in[n].gl_Position;
		 color = gs_in[n].color;
		 EmitVertex();
	  }
	  EndPrimitive();
   }
}



