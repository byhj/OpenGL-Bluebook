#ifndef Asteroid_H
#define Asteroid_H


#include "ogl/Utility.h"
#include "ogl/Shader.h"
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

		GLuint draw_vbo     = ogl::VALUE;
		GLuint indirect_vbo = ogl::VALUE;
		GLuint view_loc     = ogl::VALUE;
		GLuint proj_loc     = ogl::VALUE;
		GLuint viewproj_loc = ogl::VALUE;
		GLuint time_loc     = ogl::VALUE;
		GLuint program      = ogl::VALUE;

		ogl::Shader AsteroidShader ={ "Asteroid Shader" };
		sb6::Object object;
	};

}
#endif