#include <gl/glew.h>
#include <GL/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <sb6.h>
#include <shader.h>

static const GLfloat tetrahedron_verts[] = {
             0.000f,  0.000f,  1.000f,
             0.943f,  0.000f, -0.333f,
            -0.471f,  0.816f, -0.333f,
            -0.471f, -0.816f, -0.333f
};

static const GLushort tetrahedron_indices[] = {
            0, 1, 2,
            0, 2, 3,
            0, 3, 1,
            3, 2, 1
};

class Geometry : public sb6::Application
{
public:
	Geometry(){};
	~Geometry(){};
	virtual void init();
	virtual void render();
	void init_shader();
private:
	Shader cubeShader;
	GLuint program, mv_matrix_loc, mvp_matrix_loc;
	GLuint stretch_loc, vao, vbo;
};

DECLARE_MAIN(Geometry);

void Geometry::init_shader(void)
{
	cubeShader.attach(GL_VERTEX_SHADER, "triangle.vert");
	cubeShader.attach(GL_GEOMETRY_SHADER, "triangle.geom");
	cubeShader.attach(GL_FRAGMENT_SHADER, "triangle.frag");
	cubeShader.link();
	cubeShader.use();
    program =  cubeShader.program;
    mv_matrix_loc = glGetUniformLocation(program, "mv_matrix");
    mvp_matrix_loc = glGetUniformLocation(program, "mvp_matrix");
    stretch_loc = glGetUniformLocation(program, "stretch");
}

void Geometry::init() 
{
	 init_shader();
     glGenVertexArrays(1, &vao);
     glBindVertexArray(vao);

     glGenBuffers(1, &vbo);
     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo);
     glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(tetrahedron_verts) + sizeof(tetrahedron_indices), NULL, GL_STATIC_DRAW);
     glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(tetrahedron_indices), tetrahedron_indices);
     glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, sizeof(tetrahedron_indices), sizeof(tetrahedron_verts), tetrahedron_verts);

     glBindBuffer(GL_ARRAY_BUFFER, vbo);
     glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (GLvoid*)sizeof(tetrahedron_indices));
     glEnableVertexAttribArray(0);

     glEnable(GL_CULL_FACE);
	 glEnable(GL_DEPTH_TEST);
     glDepthFunc(GL_LEQUAL);
}

void Geometry::render(void)
{
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    static const GLfloat black[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	static const GLfloat one = 1.0f;
    glClearBufferfv(GL_COLOR, 0, black);
	glClearBufferfv(GL_DEPTH, 0, &one);
	glUseProgram(program);
    static float currentTime ;
	currentTime = glfwGetTime() / 100.0;
	float f = (float)currentTime;
	glm::mat4 proj_matrix = glm::perspective(50.0f, 1300.0f / 900.0f, 0.1f, 1000.0f);

 	    glm::mat4 mv_matrix = glm::translate(glm::mat4(1.0), glm::vec3(0.0f, 0.0f, -10.5f)) ;
		mv_matrix *= glm::rotate(glm::mat4(1.0), (float)currentTime * 71.0f, glm::vec3(0.0f, 1.0f, 0.0f)) ;
		mv_matrix *= glm::rotate(glm::mat4(1.0), (float)currentTime * 10.0f, glm::vec3(1.0f, 0.0f, 0.0f));
		glm::mat4 mvp_matrix = proj_matrix * mv_matrix;
		glUniformMatrix4fv(mvp_matrix_loc, 1, GL_FALSE, &mvp_matrix[0][0]);
		glUniformMatrix4fv(mv_matrix_loc, 1, GL_FALSE, &mv_matrix[0][0]);
        glUniform1f(stretch_loc, sinf(f * 4.0f) * 0.75f + 1.0f);

        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_SHORT, NULL);

}

