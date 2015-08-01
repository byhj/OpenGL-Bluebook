#ifndef Asteroid_H
#define Asteroid_H


#include "ogl/oglUtility.h"
#include "ogl/oglShader.h"

namespace byhj
{


	class Asteroid
	{
	public:
		Asteroid() = default;
		~Asteroid() = default;

		void Init();
		void Render();
		void Shutdown();

	private:
		void init_buffer();
		void init_vertexArray();
		void init_shader();

		GLuint vao = byhj::OGL_VALUE;
		GLuint vbo = byhj::OGL_VALUE;
		GLuint program = byhj::OGL_VALUE;

		byhj::Shader AsteroidShader ={ "Asteroid Shader" };

	};

}
#endif