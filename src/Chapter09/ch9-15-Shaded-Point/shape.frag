#version 430 core

layout (location = 0) out vec4 fragColor;

flat in int shape;

void main(void) 
{
      fragColor = vec4(1.0);   //��ȡ����ͼԪ��������ͼ��
	  	  vec2 p = gl_PointCoord * 2.0 - vec2(1.0);
	  if ( shape == 0) 
	  {
	     if (dot(p, p) > 1.0)
		     discard;
	  }
	  else if ( shape == 1) 
	  { 
		 if (dot(p, p) > sin(atan(p.y, p.x) * 5.0)) 
			 discard; 
	  }
	 else if ( shape == 2) 
	 {
		if (abs(0.8 - dot(p, p)) > 0.2)  
			discard; 
	 }
	 else if (shape == 3) 
	 {
		 if (abs(p.x) > abs(p.y))  
		   discard;
	}
}