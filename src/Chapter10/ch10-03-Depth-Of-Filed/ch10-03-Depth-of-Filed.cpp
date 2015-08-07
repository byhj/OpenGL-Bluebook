#include <GL/glew.h>
#include <memory>
#include "ogl/oglApp.h"
#include "ogl/vmath.h"
#include "ogl/object.h"
#include "ogl/ktx.h"
#include "ogl/shader.h"

#define FBO_SIZE                2048
#define FRUSTUM_DEPTH           1000

class shadowmapping_app : public byhj::Application
{
public:
    shadowmapping_app()
        : display_program(0),
          filter_program(0),
          view_program(0),
          paused(false),
          focal_distance(40.0f),
          focal_depth(50.0f)
    {
    }

protected:
	void v_InitInfo() {}
    void v_Init();
    void v_Render();
	void v_Shutdown() {}
    void render_scene(double currentTime);
	void init_shader();
	void v_Keyboard(GLFWwindow * window, int key, int scancode, int action, int mode);

    GLuint          view_program;
    GLuint          filter_program;
    GLuint          display_program;
	Shader ViewShader;
	Shader FilterShader;
	Shader DisplayShader;

    struct
    {
        struct
        {
            GLint   focal_distance;
            GLint   focal_depth;
        } dof;
        struct
        {
            GLint   mv_matrix;
            GLint   proj_matrix;
            GLint   full_shading;
            GLint   diffuse_albedo;
        } view;
    } uniforms;

    GLuint          depth_fbo;
    GLuint          depth_tex;
    GLuint          color_tex;
    GLuint          temp_tex;

    enum { OBJECT_COUNT = 5 };
    struct
    {
        sb6::Object     obj;
        vmath::mat4     model_matrix;
        vmath::vec4     diffuse_albedo;
    } objects[OBJECT_COUNT];

    vmath::mat4     camera_view_matrix;
    vmath::mat4     camera_proj_matrix;

    GLuint          quad_vao;

    bool paused;

    float          focal_distance;
    float          focal_depth;
};

void shadowmapping_app::v_Init()
{
    init_shader();

    int i;

    static const char * const object_names[] =
    {
        "../../../media/objects/dragon.sbm",
        "../../../media/objects/sphere.sbm",
        "../../../media/objects/cube.sbm",
        "../../../media/objects/cube.sbm",
        "../../../media/objects/cube.sbm",
    };

    static const vmath::vec4 object_colors[] =
    {
        vmath::vec4(1.0f, 0.7f, 0.8f, 1.0f),
        vmath::vec4(0.7f, 0.8f, 1.0f, 1.0f),
        vmath::vec4(0.3f, 0.9f, 0.4f, 1.0f),
        vmath::vec4(0.6f, 0.4f, 0.9f, 1.0f),
        vmath::vec4(0.8f, 0.2f, 0.1f, 1.0f),
    };

    for (i = 0; i < OBJECT_COUNT; i++)
    {
        objects[i].obj.load(object_names[i]);
        objects[i].diffuse_albedo = object_colors[i];
    }

    glGenFramebuffers(1, &depth_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, depth_fbo);

    glGenTextures(1, &depth_tex);
    glBindTexture(GL_TEXTURE_2D, depth_tex);
    glTexStorage2D(GL_TEXTURE_2D, 11, GL_DEPTH_COMPONENT32F, FBO_SIZE, FBO_SIZE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glGenTextures(1, &color_tex);
    glBindTexture(GL_TEXTURE_2D, color_tex);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32F, FBO_SIZE, FBO_SIZE);

    glGenTextures(1, &temp_tex);
    glBindTexture(GL_TEXTURE_2D, temp_tex);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA32F, FBO_SIZE, FBO_SIZE);

    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depth_tex, 0);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, color_tex, 0);

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glEnable(GL_DEPTH_TEST);

    glGenVertexArrays(1, &quad_vao);
    glBindVertexArray(quad_vao);
}

void shadowmapping_app::v_Render()
{
	double currentTime = glfwGetTime();
    static const GLfloat zeros[] = { 0.0f, 0.0f, 0.0f, 0.0f };
    
    static double last_time = 0.0;
    static double total_time = 0.0;

    if (!paused)
        total_time += (currentTime - last_time);
    last_time = currentTime;

    const float f = (float)total_time + 30.0f;

    vmath::vec3 view_position = vmath::vec3(0.0f, 0.0f, 40.0f);

    camera_proj_matrix = vmath::perspective(50.0f,
                                            GetAspect(),
                                            2.0f,
                                            300.0f);

    camera_view_matrix = vmath::lookat(view_position,
                                       vmath::vec3(0.0f),
                                       vmath::vec3(0.0f, 1.0f, 0.0f));

    objects[0].model_matrix = vmath::translate(5.0f, 0.0f, 20.0f) *
                              vmath::rotate(f * 14.5f, 0.0f, 1.0f, 0.0f) *
                              vmath::rotate(20.0f, 1.0f, 0.0f, 0.0f) *
                              vmath::translate(0.0f, -4.0f, 0.0f);

    objects[1].model_matrix = vmath::translate(-5.0f, 0.0f, 0.0f) *
                              vmath::rotate(f * 14.5f, 0.0f, 1.0f, 0.0f) *
                              vmath::rotate(20.0f, 1.0f, 0.0f, 0.0f) *
                              vmath::translate(0.0f, -4.0f, 0.0f);

    objects[2].model_matrix = vmath::translate(-15.0f, 0.0f, -20.0f) *
                              vmath::rotate(f * 14.5f, 0.0f, 1.0f, 0.0f) *
                              vmath::rotate(20.0f, 1.0f, 0.0f, 0.0f) *
                              vmath::translate(0.0f, -4.0f, 0.0f);

    objects[3].model_matrix = vmath::translate(-25.0f, 0.0f, -40.0f) *
                              vmath::rotate(f * 14.5f, 0.0f, 1.0f, 0.0f) *
                              vmath::rotate(20.0f, 1.0f, 0.0f, 0.0f) *
                              vmath::translate(0.0f, -4.0f, 0.0f);

    objects[4].model_matrix = vmath::translate(-35.0f, 0.0f, -60.0f) *
                              vmath::rotate(f * 14.5f, 0.0f, 1.0f, 0.0f) *
                              vmath::rotate(20.0f, 1.0f, 0.0f, 0.0f) *
                              vmath::translate(0.0f, -4.0f, 0.0f);

    glEnable(GL_DEPTH_TEST);
    render_scene(total_time);

    glUseProgram(filter_program);

    glBindImageTexture(0, color_tex, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
    glBindImageTexture(1, temp_tex, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);

    glDispatchCompute(GetScreenHeight(), 1, 1);

    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

    glBindImageTexture(0, temp_tex, 0, GL_FALSE, 0, GL_READ_ONLY, GL_RGBA32F);
    glBindImageTexture(1, color_tex, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);

    glDispatchCompute(GetScreenWidth(), 1, 1);

    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

    glBindTexture(GL_TEXTURE_2D, color_tex);
    glDisable(GL_DEPTH_TEST);
    glUseProgram(display_program);
    glUniform1f(uniforms.dof.focal_distance, focal_distance);
    glUniform1f(uniforms.dof.focal_depth, focal_depth);
    glBindVertexArray(quad_vao);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void shadowmapping_app::render_scene(double currentTime)
{
    static const GLfloat ones[] = { 1.0f };
    static const GLfloat zero[] = { 0.0f };
    static const GLfloat gray[] = { 0.1f, 0.1f, 0.1f, 0.0f };
    static const GLenum attachments[] = { GL_COLOR_ATTACHMENT0 };
    static const vmath::mat4 scale_bias_matrix = vmath::mat4(vmath::vec4(0.5f, 0.0f, 0.0f, 0.0f),
                                                             vmath::vec4(0.0f, 0.5f, 0.0f, 0.0f),
                                                             vmath::vec4(0.0f, 0.0f, 0.5f, 0.0f),
                                                             vmath::vec4(0.5f, 0.5f, 0.5f, 1.0f));

    glBindFramebuffer(GL_FRAMEBUFFER, depth_fbo);

    glDrawBuffers(1, attachments);
    glViewport(0, 0, GetScreenWidth(), GetScreenHeight());
    glClearBufferfv(GL_COLOR, 0, gray);
    glClearBufferfv(GL_DEPTH, 0, ones);
    glUseProgram(view_program);
    glUniformMatrix4fv(uniforms.view.proj_matrix, 1, GL_FALSE, camera_proj_matrix);

    glClearBufferfv(GL_DEPTH, 0, ones);

    int i;
    for (i = 0; i < OBJECT_COUNT; i++)
    {
        vmath::mat4& model_matrix = objects[i].model_matrix;
        glUniformMatrix4fv(uniforms.view.mv_matrix, 1, GL_FALSE, camera_view_matrix * objects[i].model_matrix);
        glUniform3fv(uniforms.view.diffuse_albedo, 1, objects[i].diffuse_albedo);
        objects[0].obj.render();
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void shadowmapping_app::v_Keyboard(GLFWwindow * window, int key, int scancode, int action, int mode)
{
    if (action)
    {
        switch (key)
        {
            case 'Q':
                focal_distance *= 1.1f;
                break;
            case'A':
                focal_distance /= 1.1f;
                break;
            case 'W':
                focal_depth *= 1.1f;
                break;
            case 'S':
                focal_depth /= 1.1f;
                break;
            case 'P':
                paused = !paused;
                break;
        }
    }
}

void shadowmapping_app::init_shader()
{
	ViewShader.init();
	ViewShader.attach(GL_VERTEX_SHADER, "render.vert");
	ViewShader.attach(GL_FRAGMENT_SHADER, "render.frag");
	ViewShader.link();
    view_program = ViewShader.GetProgram();

    uniforms.view.proj_matrix = glGetUniformLocation(view_program, "proj_matrix");
    uniforms.view.mv_matrix = glGetUniformLocation(view_program, "mv_matrix");
    uniforms.view.full_shading = glGetUniformLocation(view_program, "full_shading");
    uniforms.view.diffuse_albedo = glGetUniformLocation(view_program, "diffuse_albedo");

	DisplayShader.init();
    DisplayShader.attach( GL_VERTEX_SHADER, "display.vert");
    DisplayShader.attach( GL_FRAGMENT_SHADER, "display.frag");
	DisplayShader.link();
    display_program = DisplayShader.GetProgram();

    uniforms.dof.focal_distance = glGetUniformLocation(display_program, "focal_distance");
    uniforms.dof.focal_depth = glGetUniformLocation(display_program, "focal_depth");

	FilterShader.init();
    FilterShader.attach( GL_COMPUTE_SHADER, "gensat.comp");
	FilterShader.link();
    filter_program = FilterShader.GetProgram();
}

int main(int argc, const char **argv)
{
	auto app = std::make_shared<shadowmapping_app>();

	app->Run(app);

	return 0;
}
