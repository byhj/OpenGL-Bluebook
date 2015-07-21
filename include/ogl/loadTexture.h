#ifndef LOADTEXTURE_H
#define LOADTEXTURE_H

#include <iostream>
#include <GL/glew.h>
#include <SOIL.h>
#include <vector>
#include <string>

GLuint loadTexture(const char * textureFile);

// This function loads a texture from file. Note: texture loading functions like these are usually 
// managed by a 'Resource Manager' that manages all resources (like textures, models, audio). 
// For learning purposes we'll just define it as a utility function.
GLuint loadTextureGamma(GLchar* path, bool gammaCorrection);

GLuint loadTexture(GLchar* path, GLboolean alpha);

GLuint loadCubeMap(const std::vector<std::string> &faces);

#endif