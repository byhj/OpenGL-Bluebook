#ifndef OGLSHADER_H
#define OGLSHADER_H

#include <GL/glew.h>
#include <iostream>
#include <vector>
#include <string>
#include <windows.h>
#include <memory>

#ifdef _WIN32
#define WINDOW_PLATFORM
#endif

class OGLShader 
{

public:
	OGLShader(): m_Program(0), m_Name("Shader") {}
	OGLShader(std::string shaderName):m_Program(0), m_Name(shaderName) {}

	~OGLShader(){}

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

	GLuint m_Program;   //OGLShader program
	std::string m_Name;   //OGLShader class name
};

#endif

