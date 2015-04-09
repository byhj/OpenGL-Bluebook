#version 430 core

out vec4 fcolor;

in VS_OUT {
  vec2 tc;
}fs_in;

void main(void) 
{
     float val = abs(fs_in.tc.x + fs_in.tc.y) * 20.0f;
	 fcolor = vec4(fract(val) >= 0.5 ? 1.0 : 0.25); 
}