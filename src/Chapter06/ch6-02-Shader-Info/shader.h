#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <string>
#include <vector>
#include <gl/glew.h>

char *textFileRead(const char *fn);

namespace byhj
{
   class Shader 
   {
    public:
	  Shader() {};
	  Shader(std::string ShaderName): name(ShaderName), program(0){}
	  ~Shader(){}
	  void attach(int type, char* filename);
	  void link();
	  void use(void) const { glUseProgram(program); }
	  void interfaceInfo();
	  GLuint getProgram() const { return program; }
   private:
	   GLuint program;
	   std::string name;
	   std::vector<GLuint> handles;  //shader handles
   };
}  //namespace byhj
char *textFileRead( char *fn) {  //read the shader code
	FILE *fp;  
	char *content = NULL;  
	int count=0;  

	if (fn != NULL) {  
		fopen_s(&fp , fn, "rt");

		if (fp != NULL) {  
			fseek(fp, 0, SEEK_END);  
			count = ftell(fp);  
			rewind(fp);  

			if (count > 0) {  
				content = (char *)malloc(sizeof(char) * (count+1));  
				count = fread(content,sizeof(char),count,fp);  
				content[count] = '\0';  
			}  
			fclose(fp);  
		} 
		else 
			std::cout << "Fail to open the shader file" << std::endl;
	}  
	return content;  
}  

void byhj::Shader::attach(int type, char* filename) //连接不同种类shader
{
	//获取shader内容
	char* mem = textFileRead(filename);            
	GLuint handle = glCreateShader(type);
	glShaderSource(handle, 1, (const GLchar**)(&mem), 0);
	glCompileShader(handle);

	//获取编译信息
	GLint compileSuccess=0;
	GLchar compilerSpew[256];                      
	glGetShaderiv(handle, GL_COMPILE_STATUS, &compileSuccess);

	if (!compileSuccess) 
	{
		glGetShaderInfoLog(handle, sizeof(compilerSpew), 0, compilerSpew);
		std::cout << "Shader " << filename << std::endl;
		std::cout << compilerSpew << std::endl;
		std::cout <<  "Compile" << ( compileSuccess? " Success!": " Fail!") << std::endl;
		while(1);;
	}
	//存储创建的shader handle以供连接所用
	handles.push_back(handle); 
}

void byhj::Shader::link()
{
	program = glCreateProgram();
	for (int i=0; i!=handles.size(); i++) 
		glAttachShader(program, handles[i]); //将前面创建的shader添加到program

	glLinkProgram(program);  //连接shader program

	GLint linkSuccess;
	GLchar compilerSpew[256];
	glGetProgramiv(program, GL_LINK_STATUS, &linkSuccess); //输出连接信息
	if(!linkSuccess) 
	{
		glGetProgramInfoLog(program, sizeof(compilerSpew), 0, compilerSpew);
		printf("Shader Linker:\n%s\nlinkSuccess=%d\n",compilerSpew,linkSuccess);
		while(1);;
	}
	printf("%s Linked successful\n",name.c_str());

}

void byhj::Shader::interfaceInfo()
{
	GLint outputs;
	glGetProgramInterfaceiv(program, GL_PROGRAM_INPUT,  GL_ACTIVE_RESOURCES, &outputs);
	static const GLenum props[] = {GL_TYPE, GL_LOCATION};
	GLint params[2];
	GLchar name[64];
	const char *type_name;

	for (int i = 0; i != outputs; ++i)
	{
		glGetProgramResourceName(program, GL_PROGRAM_INPUT, i, sizeof(name), NULL, name);
		glGetProgramResourceiv(program, GL_PROGRAM_INPUT, i, 2, props, 2, NULL, params);

		std::cout << "------------Input-----------" << std::endl;
		type_name = name;
		std::cout << "Index " << i << std::endl;
		std::cout <<  "(" <<  type_name  << ")" << " locatoin:" << params[1] << std::endl;
	}

	glGetProgramInterfaceiv(program, GL_PROGRAM_OUTPUT,  GL_ACTIVE_RESOURCES, &outputs);
	for (int i = 0; i != outputs; ++i)
	{
		glGetProgramResourceName(program, GL_PROGRAM_OUTPUT, i, sizeof(name), NULL, name);
		glGetProgramResourceiv(program, GL_PROGRAM_OUTPUT, i, 2, props, 2, NULL, params);

		type_name = name;
		std::cout << "------------Onput-----------" << std::endl;
		std::cout << "Index " << i << std::endl;
		std::cout  <<  "(" <<  type_name  << ")" << " locatoin:" << params[1] << std::endl;
	}
}
#endif