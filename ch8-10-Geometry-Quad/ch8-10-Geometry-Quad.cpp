#include <gl/glew.h>
#include <gl/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <sb6.h>
#include <shader.h>

#include <string>


class gsquads_app : public sb6::Application
{
public:
    gsquads_app()
        : program_linesadjacency(0),
          program_fans(0),
          mode(0),
          paused(0),
          vid_offset(0)
    {

    }

    void init()
    {
		  init_shader();
    }


    void shutdown(void)
    {
        glDeleteProgram(program_linesadjacency);
        glDeleteProgram(program_fans);
        glDeleteVertexArrays(1, &vao);
    }

    void render()
    {
        static const GLfloat black[] = { 0.0f, 0.25f, 0.0f, 1.0f };
        glViewport(0, 0, info.windowWidth, info.windowHeight);

        static double last_time = 0.0;
        static double total_time = 0.0;

        if (!paused)
            total_time += (currentTime - last_time);
        last_time = currentTime;

        float t = (float)total_time;

        glClearBufferfv(GL_COLOR, 0, black);

        vmath::mat4 mv_matrix = vmath::translate(0.0f, 0.0f, -2.0f) *
                                vmath::rotate((float)t * 5.0f, 0.0f, 0.0f, 1.0f) *
                                vmath::rotate((float)t * 30.0f, 1.0f, 0.0f, 0.0f);
        vmath::mat4 proj_matrix = vmath::perspective(50.0f, (float)info.windowWidth / (float)info.windowHeight, 0.1f, 1000.0f);
        vmath::mat4 mvp = proj_matrix * mv_matrix;

        switch (mode)
        {
            case 0:
                glUseProgram(program_fans);
                glUniformMatrix4fv(mvp_loc_fans, 1, GL_FALSE, mvp);
                glUniform1i(vid_offset_loc_fans, vid_offset);
                glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
                break;
            case 1:
                glUseProgram(program_linesadjacency);
                glUniformMatrix4fv(mvp_loc_linesadj, 1, GL_FALSE, mvp);
                glUniform1i(vid_offset_loc_linesadj, vid_offset);
                glDrawArrays(GL_LINES_ADJACENCY, 0, 4);
                break;
        }
    }


    void init_shader()
    {
        GLuint vs;
        GLuint gs;
        GLuint fs;

        if (program_fans)
            glDeleteProgram(program_fans);

        program_fans = glCreateProgram();

        vs = sb6::shader::load("media/shaders/gsquads/quadsasfans.vs.glsl", GL_VERTEX_SHADER);
        fs = sb6::shader::load("media/shaders/gsquads/quadsasfans.fs.glsl", GL_FRAGMENT_SHADER);

        glAttachShader(program_fans, vs);
        glAttachShader(program_fans, fs);

        glLinkProgram(program_fans);

        mvp_loc_fans = glGetUniformLocation(program_fans, "mvp");
        vid_offset_loc_fans = glGetUniformLocation(program_fans, "vid_offset");

        glDeleteShader(vs);
        glDeleteShader(fs);

        vs = sb6::shader::load("media/shaders/gsquads/quadsaslinesadj.vs.glsl", GL_VERTEX_SHADER);
        gs = sb6::shader::load("media/shaders/gsquads/quadsaslinesadj.gs.glsl", GL_GEOMETRY_SHADER);
        fs = sb6::shader::load("media/shaders/gsquads/quadsaslinesadj.fs.glsl", GL_FRAGMENT_SHADER);

        if (program_linesadjacency)
            glDeleteProgram(program_linesadjacency);

        program_linesadjacency = glCreateProgram();

        glAttachShader(program_linesadjacency, vs);
        glAttachShader(program_linesadjacency, gs);
        glAttachShader(program_linesadjacency, fs);

        glLinkProgram(program_linesadjacency);

        mvp_loc_linesadj = glGetUniformLocation(program_linesadjacency, "mvp");
        vid_offset_loc_linesadj = glGetUniformLocation(program_fans, "vid_offset");

        glDeleteShader(vs);
        glDeleteShader(gs);
        glDeleteShader(fs);
    }

private:
    GLuint      program_fans;
    GLuint      program_linesadjacency;
    GLuint      vao;
    int         mode;
    int         mvp_loc_fans;
    int         mvp_loc_linesadj;
    int         vid_offset_loc_fans;
    int         vid_offset_loc_linesadj;
    int         vid_offset;
    bool        paused;
};

DECLARE_MAIN(gsquads_app);
