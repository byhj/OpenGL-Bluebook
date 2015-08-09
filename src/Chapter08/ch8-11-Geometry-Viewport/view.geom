#version 420 core                                                  
                    
//invocations ���ƶ��ٸ�ʵ��,�ظ��������������� gl_InvocationID                                          
layout (triangles, invocations = 4) in;                            
layout (triangle_strip, max_vertices = 3) out;                     
                                                                   
layout (std140, binding = 0) uniform transform_block               
{                                                                  
    mat4 mvp_matrix[4];                                            
};                                                                 
                                                                   
in VS_OUT                                                          
{                                                                  
    vec4 color;                                                    
} gs_in[];     //������ʽ����Ϊ�������ͼԪ                                                    
                                                                   
out GS_OUT                                                         
{                                                                  
    vec4 color;                                                    
} gs_out;                                                          
                                                                   
void main(void)                                                    
{                                                                  
    for (int i = 0; i < gl_in.length(); i++)                       
    {                                                              
        gs_out.color = gs_in[i].color;                             
        gl_Position = mvp_matrix[gl_InvocationID] * gl_in[i].gl_Position; 

		//mutil viewport                       
        gl_ViewportIndex = gl_InvocationID;                        
        EmitVertex();                                              
    }                                                              
    EndPrimitive();     
}                                           