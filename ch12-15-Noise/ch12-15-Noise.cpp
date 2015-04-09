#include <gl/glew.h>
#include <gl/glfw3.h>
#include <sb6.h>
#include <ktx.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <shader.h>
#include <string>


class noise_app : public sb6::Application
{
public:
    noise_app()
        : prog_noise(0){ }

    void init()
    {
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);
        init_shader();
    }

    void shutdown(void)
    {
        glDeleteProgram(prog_noise);
        glDeleteVertexArrays(1, &vao);
    }

    void render()
    {
        static const GLfloat black[] = { 0.0f, 0.25f, 0.0f, 1.0f };
		glClearBufferfv(GL_COLOR, 0, black);
        float t = (float)glfwGetTime() / 100.0f;

        glm::mat4 mv_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -2.0f)) *
                                glm::rotate(glm::mat4(1.0f), (float)t * 5.0f, glm::vec3(0.0f, 0.0f, 1.0f)) *
                                glm::rotate(glm::mat4(1.0f), (float)t * 30.0f, glm::vec3(1.0f, 0.0f, 0.0f));
       
		glm::mat4 proj_matrix = glm::perspective(50.0f, 1300.0f / 900.0f, 0.1f, 1000.0f);
        glm::mat4 mvp = proj_matrix * mv_matrix;

        glUseProgram(prog_noise);
        glUniform1f(loc_time, t * 0.0002);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }

    void init_shader()
    {
		NoiseShader.attach(GL_VERTEX_SHADER, "noise.vert");
		NoiseShader.attach(GL_FRAGMENT_SHADER, "noise.frag");
        NoiseShader.link();
	    prog_noise = NoiseShader.program;
        loc_time = glGetUniformLocation(prog_noise, "time");
    }

private:
    GLuint      prog_noise;
    GLuint      vao;
    int         loc_time;
	Shader NoiseShader;
};

DECLARE_MAIN(noise_app);
