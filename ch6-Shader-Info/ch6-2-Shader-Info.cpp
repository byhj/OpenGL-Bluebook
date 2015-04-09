#include <GL/glew.h>
#include "shader.h"
#include "sb6.h"

static const struct type_to_name_entry
{
	GLenum type;
	const char * name;
}

type_to_name_table[] =
{
	GL_FLOAT,                       "float",
	GL_FLOAT_VEC2,                  "vec2",
	GL_FLOAT_VEC3,                  "vec3",
	GL_FLOAT_VEC4,                  "vec4",
	GL_DOUBLE,                      "double",
	GL_DOUBLE_VEC2,                 "dvec2",
	GL_DOUBLE_VEC3,                 "dvec3",
	GL_DOUBLE_VEC4,                 "dvec4",
	GL_INT,                         "int",
	GL_INT_VEC2,                    "ivec2",
	GL_INT_VEC3,                    "ivec3",
	GL_INT_VEC4,                    "ivec4",
	GL_UNSIGNED_INT,                "uint",
	GL_UNSIGNED_INT_VEC2,           "uvec2",
	GL_UNSIGNED_INT_VEC3,           "uvec3",
	GL_UNSIGNED_INT_VEC4,           "uvec4",
	GL_BOOL,                        "bool",
	GL_BOOL_VEC2,                   "bvec2",
	GL_BOOL_VEC3,                   "bvec3",
	GL_BOOL_VEC4,                   "bvec4",
	GL_NONE,                        NULL
};

static const char * type_to_name(GLenum type)
{
	for (const type_to_name_entry * ptr = &type_to_name_table[0]; ptr->name != NULL; ptr++)
	{
		if (ptr->type == type)
			return ptr->name;
	}

	return NULL;
}
byhj::Shader shader("test");

const static GLfloat VertexData[] = {
	-0.5f, -0.5f,
	 0.5f, -0.5f,
	 0.0f,  0.5f
};

class test_app : public sb6::Application 
{
public:
	void init_shader()
	{
		shader.attach(GL_VERTEX_SHADER, "triangle.vert");
		shader.attach(GL_FRAGMENT_SHADER, "triangle.frag");
		shader.link();
		shader.use();
		shader.interfaceInfo();
		program = shader.getProgram();
		GLint outputs;
		glGetProgramInterfaceiv(program, GL_PROGRAM_OUTPUT, GL_ACTIVE_RESOURCES, &outputs);
		static const GLenum props[] = { GL_TYPE, GL_LOCATION, GL_ARRAY_SIZE };
		static const char * prop_name[] = { "type", "location", "array size" };
		GLint i;
		GLint params[4];
		GLchar name[64];
		const char * type_name;
		char buffer[1024];
		glGetProgramInfoLog(program, sizeof(buffer), NULL, buffer);
		for (i = 0; i < outputs; i++)
		{
			glGetProgramResourceName(program, GL_PROGRAM_OUTPUT, i, sizeof(name), NULL, name);
			glGetProgramResourceiv(program, GL_PROGRAM_OUTPUT, i, 3, props, 3, NULL, params);
			type_name = type_to_name(params[0]);
			if (params[2] != 0)
			{
				sprintf(buffer, "Index %d: %s %s[%d] @ location %d.", i, type_name, name, params[2], params[1]);
			}
			else
			{
				sprintf(buffer, "Index %d: %s %s @ location %d.", i, type_name, name, params[1]);
			}
		}
	}

	void init_buffer()
	{
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(VertexData), VertexData, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void init_vertexArray()
	{
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);   //bind the vbo to vao, send the data to shader
	    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
	    glEnableVertexAttribArray(0);
	    glBindVertexArray(0);
	}
	virtual void initWindowInfo()
	{
		windowInfo.title = "Test";
		windowInfo.Width = 1200;
		windowInfo.Height = 800;
	}

	virtual void init()
	{
		init_shader();
		init_buffer();
		init_vertexArray();
    }

	virtual void render()
	{
		static const GLfloat red[] = {0.0f, 0.0f, 0.0f, 1.0f };
        glClearBufferfv(GL_COLOR, 0, red);
		glBindVertexArray(vao);
        glUseProgram(program);
        glDrawArrays(GL_TRIANGLES, 0, 3);
		glBindVertexArray(0);
	}

    virtual void shutdown()
    {
		glDeleteBuffers(1, &vbo);
        glDeleteVertexArrays(1, &vao);
        glDeleteProgram(program);
    }

private:
    GLuint  program;
    GLuint  vbo, vao, ibo;;
};

DECLARE_MAIN(test_app);