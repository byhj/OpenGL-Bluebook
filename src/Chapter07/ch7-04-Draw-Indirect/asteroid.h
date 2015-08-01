#ifndef Asteroid_H
#define Asteroid_H


#include "ogl/oglUtility.h"
#include "ogl/oglShader.h"
#include "ogl/object.h"

enum {
	NUM_DRAWS = 50000
};

//The draw command struct for indirect draw
struct DrawArraysIndirectCommand
{
	GLuint count;
	GLuint primCount;
	GLuint first;
	GLuint baseInstance;
};


namespace byhj
{


	class Asteroid
	{
	public:
		Asteroid() = default;
		~Asteroid() = default;

		void Init();
		void Render(GLfloat aspect);
		void Shutdown();

	private:
		void init_buffer();
		void init_vertexArray();
		void init_shader();

		enum MODE
		{
			MODE_FIRST,
			MODE_MULTIDRAW = 0,
			MODE_SEPARATE_DRAWS,
			MODE_MAX = MODE_SEPARATE_DRAWS
		};

		MODE  mode = MODE_MULTIDRAW;

		GLuint draw_vbo     = byhj::OGL_VALUE;
		GLuint indirect_vbo = byhj::OGL_VALUE;
		GLuint view_loc     = byhj::OGL_VALUE;
		GLuint proj_loc     = byhj::OGL_VALUE;
		GLuint viewproj_loc = byhj::OGL_VALUE;
		GLuint time_loc     = byhj::OGL_VALUE;
		GLuint program      = byhj::OGL_VALUE;

		byhj::Shader AsteroidShader ={ "Asteroid Shader" };
		sb6::Object object;
	};

}
#endif