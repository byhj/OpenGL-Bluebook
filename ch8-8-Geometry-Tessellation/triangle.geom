#version 430 core                                                           
                                                                                                                                                    
layout (triangles) in;                                                      
layout (triangle_strip, max_vertices = 12) out; //输出三角形带                              
                                                                                                                                                                             
flat out vec4 color;                                                                                                                                
uniform mat4 mvp_matrix;                                                     
uniform mat4 mv_matrix;                                                      
uniform float stretch = 0.7;  
                                                                          
void make_face(vec3 a, vec3 b, vec3 c)       //生成新的面                               
{                                                                           
    vec3 face_normal = normalize(cross(c - a, c - b));                      
    vec4 face_color = vec4(1.0, 0.2, 0.4, 1.0) * (mat3(mv_matrix) * face_normal).z;
    gl_Position = mvp_matrix * vec4(a, 1.0);                                 
    color = face_color;                                                     
    EmitVertex();                                                           
                                                                            
    gl_Position = mvp_matrix * vec4(b, 1.0);                                 
    color = face_color;                                                     
    EmitVertex();                                                           
                                                                            
    gl_Position = mvp_matrix * vec4(c, 1.0);                                 
    color = face_color;                                                     
    EmitVertex();                                                           
                                                                            
    EndPrimitive();          //结束图元                                               
}                                                                           
                                                                            
void main(void)                                                             
{                                                                           
    int n;                                                                  
    vec3 a = gl_in[0].gl_Position.xyz;       //当前图元的三个点                               
    vec3 b = gl_in[1].gl_Position.xyz;                                      
    vec3 c = gl_in[2].gl_Position.xyz;                                      
                                                                            
    vec3 d = (a + b) * stretch;                                             
    vec3 e = (b + c) * stretch;                                             
    vec3 f = (c + a) * stretch;                                             
                                                                            
    a *= (2.0 - stretch);                                                   
    b *= (2.0 - stretch);                                                   
    c *= (2.0 - stretch);                                                   

	//生成新的图元
    make_face(a, d, f);                                   
    make_face(d, b, e);                                                     
    make_face(e, c, f);                                                     
    make_face(d, e, f);                                                     

    EndPrimitive();                                                         
}  