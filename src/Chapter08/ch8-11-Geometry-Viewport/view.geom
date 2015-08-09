#version 420 core                                                  
                    
//invocations 绘制多少个实例,重复操作绘制命令多次 gl_InvocationID                                          
layout (triangles, invocations = 4) in;                            
layout (triangle_strip, max_vertices = 3) out;                     
                                                                   
layout (std140, binding = 0) uniform transform_block               
{                                                                  
    mat4 mvp_matrix[4];                                            
};                                                                 
                                                                   
in VS_OUT                                                          
{                                                                  
    vec4 color;                                                    
} gs_in[];     //数组形式，因为进入的是图元                                                    
                                                                   
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