#version 430 core

layout (location = 0) in vec4 position;
out vec4 color;
uniform mat4 mvp_matrix;

layout (binding = 0) uniform sampler1D grasspalette_texture;                                              
layout (binding = 1) uniform sampler2D length_texture;                                                      
layout (binding = 2) uniform sampler2D orientation_texture;                                               
layout (binding = 3) uniform sampler2D grasscolor_texture;                                                 
layout (binding = 4) uniform sampler2D bend_texture;   

int random(int seed, int iterations)    //随机数生产器                 
{                                                               
    int value = seed;                                                                                                                                                  
    for (int n = 0; n < iterations; n++)                       
        value = ((value >> 7) ^ (value << 9)) * 15485863;                                                                     
    return value;                                               
}                                                               
                                                                
vec4 random_vector(int seed)       //随机向量器                             
{                                                               
    int r = random(gl_InstanceID, 4);                           
    int g = random(r, 2);                                       
    int b = random(g, 2);                                       
    int a = random(b, 2);                                       
                                                                
    return vec4(float(r & 0x3FF) / 1024.0,                      
                float(g & 0x3FF) / 1024.0,                      
                float(b & 0x3FF) / 1024.0,                      
                float(a & 0x3FF) / 1024.0);                     
}                                                               
                                                                
mat4 construct_rotation_matrix(float angle)   //旋转矩阵生产器              
{                                                               
    float st = sin(angle);                                      
    float ct = cos(angle);                                      
                                                                
    return mat4(vec4(ct, 0.0, st, 0.0),                         
                vec4(0.0, 1.0, 0.0, 0.0),                       
                vec4(-st, 0.0, ct, 0.0),                        
                vec4(0.0, 0.0, 0.0, 1.0));                      
}  

void main()
{
        //每根小草的位置
      vec4 offset = vec4(float(gl_InstanceID >> 10) - 512.0,      
                       0.0f,                                    
                       float(gl_InstanceID & 0x3FF) - 512.0,    
                       0.0f); 

    int number1 = random(gl_InstanceID, 3);                     
    int number2 = random(number1, 2);  

	 //添加混乱效果
	 offset += vec4(float(number1 & 0xFF) / 256.0,               
                   0.0f,                                        
                   float(number2 & 0xFF) / 256.0,               
                   0.0f);     

	 // float angle = float(random(number2, 2) & 0x3FF) / 1024.0;
                                                                
    vec2 texcoord = offset.xz / 1024.0 + vec2(0.5);       //纹理坐标      
                                                                
    // float bend_factor = float(random(number2, 7) & 0x3FF) / 1024.0;                                  
    float bend_factor = texture(bend_texture, texcoord).r * 2.0;
    float bend_amount = cos(position.y);                         
                                                                
    float angle = texture(orientation_texture, texcoord).r * 2.0 * 3.141592;                            
    mat4 rot = construct_rotation_matrix(angle);                
    vec4 position = (rot * (position + vec4(0.0, 0.0, bend_amount * bend_factor, 0.0))) + offset;        
                                                                
    position *= vec4(1.0, texture(length_texture, texcoord).r * 0.9 + 0.3, 1.0, 1.0);                   
                                                                
    gl_Position = mvp_matrix * position; // (rot * position);    
    // color = vec4(random_vector(gl_InstanceID).xyz * vec3(0.1, 0.5, 0.1) + vec3(0.1, 0.4, 0.1), 1.0); 
    // color = texture(orientation_texture, texcoord);    
	      
    color = texture(grasspalette_texture, texture(grasscolor_texture, texcoord).r) +                    
            vec4(random_vector(gl_InstanceID).xyz * vec3(0.1, 0.5, 0.1), 1.0);      
}


