#version 430 core

layout (binding = 0) uniform sampler2D hdr_image;

uniform float exposure = 1.0;

out vec4 color;

void main(void)
{
    vec4 c = texture(hdr_image, 2.0 * gl_FragCoord.xy / textureSize(hdr_image, 0));
    c.rgb = vec3(1.0) - exp(-c.rgb * exposure);
    color = c;
}