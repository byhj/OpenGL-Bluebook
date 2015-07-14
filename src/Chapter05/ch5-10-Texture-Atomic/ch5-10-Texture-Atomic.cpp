#include <gl/glew.h>

#include "ogl/oglApp.h"
#include <ogl/vmath.h>
#include <ogl/object.cpp>
#include <ogl/ktx.cpp>
#include "ogl/shader.h"

class fragmentlist_app : public byhj::Application
{
public:
    fragmentlist_app()
        : clear_program(0),
          append_program(0),
          resolve_program(0)
    {
    }

	void v_Init();
	void v_Render();
	void init_shader();
	void init_buffer();
protected:

    GLuint          clear_program;
    GLuint          append_program;
    GLuint          resolve_program;

	Shader  ClearShader;
	Shader  AppendShader;
	Shader  ResolveShader;

    struct
    {
        GLuint      color;
        GLuint      normals;
    } textures;

    struct uniforms_block
    {
        vmath::mat4     mv_matrix;
        vmath::mat4     view_matrix;
        vmath::mat4     proj_matrix;
    };

    GLuint          uniforms_buffer;

    struct
    {
        GLint           mvp;
    } uniforms;

    sb6::Object     object;

    GLuint          fragment_buffer;
    GLuint          head_pointer_image;
    GLuint          atomic_counter_buffer;
    GLuint          dummy_vao;
};


void fragmentlist_app::v_Init()
{
   init_shader();
   init_buffer();


}

void fragmentlist_app::init_buffer()
{
	int w = GetScreenWidth();
	int h = GetScreenHeight();
	glGenBuffers(1, &uniforms_buffer);
	glBindBuffer(GL_UNIFORM_BUFFER, uniforms_buffer);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(uniforms_block), NULL, GL_DYNAMIC_DRAW);

	object.load("../../../media/objects/dragon.sbm");

	glGenBuffers(1, &fragment_buffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, fragment_buffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, w * h* 16, NULL, GL_DYNAMIC_COPY);

	glGenBuffers(1, &atomic_counter_buffer);
	glBindBuffer(GL_ATOMIC_COUNTER_BUFFER, atomic_counter_buffer);
	glBufferData(GL_ATOMIC_COUNTER_BUFFER, 4, NULL, GL_DYNAMIC_COPY);

	glGenTextures(1, &head_pointer_image);
	glBindTexture(GL_TEXTURE_2D, head_pointer_image);
	glTexStorage2D(GL_TEXTURE_2D, 1, GL_R32UI, w, h);

	glGenVertexArrays(1, &dummy_vao);
	glBindVertexArray(dummy_vao);
}

void fragmentlist_app::v_Render()
{
    static const GLfloat zeros[] = { 0.0f, 0.0f, 0.0f, 0.0f };
    static const GLfloat gray[] = { 0.1f, 0.1f, 0.1f, 0.0f };
    static const GLfloat ones[] = { 1.0f };
    const float f = (float)glfwGetTime();

    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT | GL_ATOMIC_COUNTER_BARRIER_BIT | GL_SHADER_STORAGE_BARRIER_BIT);

    glUseProgram(clear_program);
    glBindVertexArray(dummy_vao);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    glUseProgram(append_program);

    vmath::mat4 model_matrix = vmath::scale(7.0f);
    vmath::vec3 view_position = vmath::vec3(cosf(f * 0.35f) * 120.0f, cosf(f * 0.4f) * 30.0f, sinf(f * 0.35f) * 120.0f);
    vmath::mat4 view_matrix = vmath::lookat(view_position,
                                            vmath::vec3(0.0f, 30.0f, 0.0f),
                                            vmath::vec3(0.0f, 1.0f, 0.0f));

    vmath::mat4 mv_matrix = view_matrix * model_matrix;
    vmath::mat4 proj_matrix = vmath::perspective(50.0f,
                                                 GetAspect(),
                                                 0.1f,
                                                 1000.0f);

    glUniformMatrix4fv(uniforms.mvp, 1, GL_FALSE, proj_matrix * mv_matrix);

    static const unsigned int zero = 0;
    glBindBufferBase(GL_ATOMIC_COUNTER_BUFFER, 0, atomic_counter_buffer);
    glBufferSubData(GL_ATOMIC_COUNTER_BUFFER, 0, sizeof(zero), &zero);

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, fragment_buffer);

    glBindImageTexture(0, head_pointer_image, 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32UI);

    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT | GL_ATOMIC_COUNTER_BARRIER_BIT | GL_SHADER_STORAGE_BARRIER_BIT);

    object.render();

    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT | GL_ATOMIC_COUNTER_BARRIER_BIT | GL_SHADER_STORAGE_BARRIER_BIT);

    glUseProgram(resolve_program);

    glBindVertexArray(dummy_vao);

    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT | GL_ATOMIC_COUNTER_BARRIER_BIT | GL_SHADER_STORAGE_BARRIER_BIT);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}



void fragmentlist_app::init_shader()
{
	ClearShader.init();
    ClearShader.attach( GL_VERTEX_SHADER, "clear.vert");
    ClearShader.attach( GL_FRAGMENT_SHADER, "clear.frag");
	ClearShader.link();
    clear_program = ClearShader.GetProgram();

    AppendShader.attach( GL_VERTEX_SHADER, "append.vert");
    AppendShader.attach( GL_FRAGMENT_SHADER, "append.frag");
	AppendShader.link();
    append_program = AppendShader.GetProgram();

    uniforms.mvp = glGetUniformLocation(append_program, "mvp");

    ResolveShader.attach( GL_VERTEX_SHADER, "resolve.vert");
    ResolveShader.attach( GL_FRAGMENT_SHADER, "resolve.frag");
	ResolveShader.link();
    resolve_program = ResolveShader.GetProgram();
}

CALL_MAIN(fragmentlist_app)
