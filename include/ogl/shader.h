#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <iostream>
#include <vector>
#include <string>
#include <windows.h>
#include <memory>

#ifdef _WIN32
#define WINDOW_PLATFORM
#endif


namespace byhj
{

namespace ogl
{

class Shader
{

public:
	Shader(): m_Program(0), m_Name("Shader") {}
	Shader(std::string shaderName):m_Program(0), m_Name(shaderName) {}

	~Shader(){}

public:
	void init();
	void attach(int type, char *filename);
	void link();
	void info();
	void use() const; 
	void end() const;

	GLuint GetProgram() const;


private:
	char *textFileRead( char *fn) ;

	GLuint m_Program;   //Shader program
	std::string m_Name;   //Shader class name
};

}
}
#endif

