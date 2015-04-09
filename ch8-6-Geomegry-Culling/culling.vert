#version 430 core

in vec4 position;
in vec3 normal;

out VS_OUT {
   vec3 normal;
   vec4 color;
}vs_out;

uniform vec3 LightPos= vec3(-10.0, 40.0, 200.0);
uniform mat4 mv_matrix;

void main()
{
   //normal, positoin in view coord
   vec3 EyeNormal = mat3(mv_matrix) * normal;
   vec4 EyePosition = mv_matrix * position;
   vec3 vPosition = position.xyz / position.w;
   vec3 lightDir = normalize(LightPos - vPosition);

   //add the diffuse intensity
   vs_out.color = vec4(0.7, 0.6, 1.0, 1.0) * abs(dot(EyeNormal, lightDir));
   vs_out.normal = normal;

   gl_Position = position;
}