#version 420 core                                              
                                                               
void main(void)                                                
{                                                              
    const vec4 vertices[] = vec4[](vec4(-0.8, -0.8, 0.0, 1.0), 
                                   vec4( 0.8, -0.8, 0.0, 1.0), 
                                   vec4(-0.8,  0.8, 0.0, 1.0), 
                                   vec4( 0.8,  0.8, 0.0, 1.0));
                                                               
    gl_Position = vertices[gl_VertexID];                       
}                                                              