#version 410 core                                            
         
in vec4 position;

uniform mat4 mvp_matrix;
    
out VS_OUT {
   vec2 tc;
}vs_out;
                                                        
void main(void)                                               
{                                                               
                                                                   
     gl_Position = mvp_matrix * position;  
	 vs_out.tc = position.xy;                        
}                                                                 