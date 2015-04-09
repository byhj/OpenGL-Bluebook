#include <iostream>
#include <vector>
#include <string>

char *textFileRead(const char *fn);  


class Shader {
public:
	Shader(){}
	~Shader(){}
	Shader(std::string shaderName):name(shaderName) {}
	void init();
	void attach(int type, char *filename);
	void link();
	void use(void) {
		glUseProgram(program);
		
	}
	void end(void) {
		glUseProgram(0);
		
	}
	std::vector<GLuint> handles; //shader handle
	GLuint program;   //shader program
	std::string name;   //shader class name
};

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


void Shader::init()
{

}

void Shader::attach(int type, char* filename) //���Ӳ�ͬ����shader
{
		char* mem = textFileRead(filename);
		GLuint handle = glCreateShader(type);
		glShaderSource(handle, 1, (const GLchar**)(&mem), 0);
		

		glCompileShader(handle);
		
		GLint compileSuccess=0;
		GLchar compilerSpew[256];

		glGetShaderiv(handle, GL_COMPILE_STATUS, &compileSuccess);
		
		if (!compileSuccess) {
			glGetShaderInfoLog(handle, sizeof(compilerSpew), 0, compilerSpew);
			printf("Shader %s\n%s\ncompileSuccess=%d\n",filename, compilerSpew, compileSuccess);
			
			while(1);;
		}
		handles.push_back(handle); //�洢������shader handle�Թ���������
}

void Shader::link()
{
    program = glCreateProgram();
	for (int i=0; i!=handles.size(); i++) {
		glAttachShader(program, handles[i]); //��ǰ�洴����shader��ӵ�program
		
	}
	glLinkProgram(program);  //����shader program
	

	GLint linkSuccess;
	GLchar compilerSpew[256];
	glGetProgramiv(program, GL_LINK_STATUS, &linkSuccess); //���������Ϣ
	if(!linkSuccess) {
			glGetProgramInfoLog(program, sizeof(compilerSpew), 0, compilerSpew);
			printf("Shader Linker:\n%s\nlinkSuccess=%d\n",compilerSpew,linkSuccess);
			
			while(1);;
	}
		printf("%s linked successful\n",name.c_str());
	
}

