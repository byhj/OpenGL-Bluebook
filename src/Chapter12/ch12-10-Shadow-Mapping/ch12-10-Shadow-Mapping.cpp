#include <gl/glew.h>
#include <sb6/sb6.h>
#include <sb6/vmath.h>
#include <sb6/object.cpp>
#include <sb6/ktx.cpp>
#include <sb6/shader.h>

#define DEPTH_TEXTURE_SIZE      4096
#define FRUSTUM_DEPTH           1000

class shadowmapping_app : public byhj::Application
{
public:
    shadowmapping_app()
        : light_program(0),
          view_program(0),
          show_light_depth_program(0),
          mode(RENDER_LIGHT),
          paused(false)
    {
    }
	void v_Init();
	void v_Render();
	void v_Keyboard(GLFWwindow * window, int key, int scancode, int action, int mode);
protected:


    void render_scene(double currentTime, bool from_light);
    void init_shader();


    GLuint          light_program;
    GLuint          view_program;
    GLint           show_light_depth_program;
	Shader LightShader;
	Shader ViewShader;
	Shader ShowShader;
    struct
    {
        struct
        {
            GLint   mvp;
        } light;
        struct
        {
            GLint   mv_matrix;
            GLint   proj_matrix;
            GLint   shadow_matrix;
            GLint   full_shading;
        } view;
    } uniforms;

    GLuint          depth_fbo;
    GLuint          depth_tex;
    GLuint          depth_debug_tex;

    enum { OBJECT_COUNT = 4 };
    struct
    {
        sb6::Object     obj;
        vmath::mat4     model_matrix;
    } objects[OBJECT_COUNT];

    vmath::mat4     light_view_matrix;
    vmath::mat4     light_proj_matrix;

    vmath::mat4     camera_view_matrix;
    vmath::mat4     camera_proj_matrix;

    GLuint          quad_vao;

    enum
    {
        RENDER_FULL,
        RENDER_LIGHT,
        RENDER_DEPTH
    } mode;

    bool paused;
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
        "../../../media/objects/torus.sbm"
    };

    for (i = 0; i < OBJECT_COUNT; i++)
    {
        objects[i].obj.load(object_names[i]);
    }

    glGenFramebuffers(1, &depth_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, depth_fbo);

    glGenTextures(1, &depth_tex);
    glBindTexture(GL_TEXTURE_2D, depth_tex);
    glTexStorage2D(GL_TEXTURE_2D, 11, GL_DEPTH_COMPONENT32F, DEPTH_TEXTURE_SIZE, DEPTH_TEXTURE_SIZE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);

    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depth_tex, 0);

    glGenTextures(1, &depth_debug_tex);
    glBindTexture(GL_TEXTURE_2D, depth_debug_tex);
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_R32F, DEPTH_TEXTURE_SIZE, DEPTH_TEXTURE_SIZE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, depth_debug_tex, 0);

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

    vmath::vec3 light_position = vmath::vec3(20.0f, 20.0f, 20.0f);
    vmath::vec3 view_position = vmath::vec3(0.0f, 0.0f, 40.0f);

    light_proj_matrix = vmath::frustum(-1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 200.0f);
    light_view_matrix = vmath::lookat(light_position,
                                      vmath::vec3(0.0f), vmath::vec3(0.0f, 1.0f, 0.0f));

    camera_proj_matrix = vmath::perspective(50.0f,
                                            GetAspect(),
                                            1.0f,
                                            200.0f);

    camera_view_matrix = vmath::lookat(view_position,
                                       vmath::vec3(0.0f),
                                       vmath::vec3(0.0f, 1.0f, 0.0f));

    objects[0].model_matrix = vmath::rotate(f * 14.5f, 0.0f, 1.0f, 0.0f) *
                              vmath::rotate(20.0f, 1.0f, 0.0f, 0.0f) *
                              vmath::translate(0.0f, -4.0f, 0.0f);

    objects[1].model_matrix = vmath::rotate(f * 3.7f, 0.0f, 1.0f, 0.0f) *
                              vmath::translate(sinf(f * 0.37f) * 12.0f, cosf(f * 0.37f) * 12.0f, 0.0f) *
                              vmath::scale(2.0f);

    objects[2].model_matrix = vmath::rotate(f * 6.45f, 0.0f, 1.0f, 0.0f) *
                              vmath::translate(sinf(f * 0.25f) * 10.0f, cosf(f * 0.25f) * 10.0f, 0.0f) *
                              vmath::rotate(f * 99.0f, 0.0f, 0.0f, 1.0f) *
                              vmath::scale(2.0f);

    objects[3].model_matrix = vmath::rotate(f * 5.25f, 0.0f, 1.0f, 0.0f) *
                              vmath::translate(sinf(f * 0.51f) * 14.0f, cosf(f * 0.51f) * 14.0f, 0.0f) *
                              vmath::rotate(f * 120.3f, 0.707106f, 0.0f, 0.707106f) *
                              vmath::scale(2.0f);

    glEnable(GL_DEPTH_TEST);
    render_scene(total_time, true);

    if (mode == RENDER_DEPTH)
    {
        glDisable(GL_DEPTH_TEST);
        glBindVertexArray(quad_vao);
        glUseProgram(show_light_depth_program);
        glBindTexture(GL_TEXTURE_2D, depth_debug_tex);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }
    else
    {
        render_scene(total_time, false);
    }
}

void shadowmapping_app::render_scene(double currentTime, bool from_light)
{
    static const GLfloat ones[] = { 1.0f };
    static const GLfloat zero[] = { 0.0f };
    static const GLfloat gray[] = { 0.1f, 0.1f, 0.1f, 0.0f };
    static const vmath::mat4 scale_bias_matrix = vmath::mat4(vmath::vec4(0.5f, 0.0f, 0.0f, 0.0f),
                                                             vmath::vec4(0.0f, 0.5f, 0.0f, 0.0f),
                                                             vmath::vec4(0.0f, 0.0f, 0.5f, 0.0f),
                                                             vmath::vec4(0.5f, 0.5f, 0.5f, 1.0f));
    vmath::mat4 light_vp_matrix = light_proj_matrix * light_view_matrix;
    vmath::mat4 shadow_sbpv_matrix = scale_bias_matrix * light_proj_matrix * light_view_matrix;

    if (from_light)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, depth_fbo);
        glViewport(0, 0, DEPTH_TEXTURE_SIZE, DEPTH_TEXTURE_SIZE);
        glEnable(GL_POLYGON_OFFSET_FILL);
        glPolygonOffset(4.0f, 4.0f);
        glUseProgram(light_program);
        static const GLenum buffs[] = { GL_COLOR_ATTACHMENT0 };
        glDrawBuffers(1, buffs);
        glClearBufferfv(GL_COLOR, 0, zero);
    }
    else
    {
        glViewport(0, 0, GetScreenWidth(), GetScreenHeight());
        glClearBufferfv(GL_COLOR, 0, gray);
        glUseProgram(view_program);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, depth_tex);
        glUniformMatrix4fv(uniforms.view.proj_matrix, 1, GL_FALSE, camera_proj_matrix);
        glDrawBuffer(GL_BACK);
    }

    glClearBufferfv(GL_DEPTH, 0, ones);

    int i;
    for (i = 0; i < 4; i++)
    {
        vmath::mat4& model_matrix = objects[i].model_matrix;
        if (from_light)
        {
            glUniformMatrix4fv(uniforms.light.mvp, 1, GL_FALSE, light_vp_matrix * objects[i].model_matrix);
        }
        else
        {
            vmath::mat4 shadow_matrix = shadow_sbpv_matrix * model_matrix;
            glUniformMatrix4fv(uniforms.view.shadow_matrix, 1, GL_FALSE, shadow_matrix);
            glUniformMatrix4fv(uniforms.view.mv_matrix, 1, GL_FALSE, camera_view_matrix * objects[i].model_matrix);
            glUniform1i(uniforms.view.full_shading, mode == RENDER_FULL ? 1 : 0);
        }
        objects[i].obj.render();
    }

    if (from_light)
    {
        glDisable(GL_POLYGON_OFFSET_FILL);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
    else
    {

        glBindTexture(GL_TEXTURE_2D, 0);
    }
}

void shadowmapping_app::v_Keyboard(GLFWwindow * window, int key, int scancode, int action, int mode)
{
    if (action)
    {
        switch (key)
        {
            case '1':
                mode = RENDER_FULL;
                break;
            case '2':
                mode = RENDER_LIGHT;
                break;
            case '3':
                mode = RENDER_DEPTH;
                break;
                break;
            case 'P':
                paused = !paused;
                break;
        }
    }
}

void shadowmapping_app::init_shader()
{
    LightShader.attach( GL_VERTEX_SHADER, "light.vert");
    LightShader.attach( GL_FRAGMENT_SHADER, "light.frag");
	LightShader.link();
    light_program = LightShader.GetProgram();
    uniforms.light.mvp = glGetUniformLocation(light_program, "mvp");

    ViewShader.attach(GL_VERTEX_SHADER, "light-view.vert");
    ViewShader.attach(GL_FRAGMENT_SHADER, "light-view.frag");
	ViewShader.link();
    view_program = ViewShader.GetProgram();
    uniforms.view.proj_matrix = glGetUniformLocation(view_program, "proj_matrix");
    uniforms.view.mv_matrix = glGetUniformLocation(view_program, "mv_matrix");
    uniforms.view.shadow_matrix = glGetUniformLocation(view_program, "shadow_matrix");
    uniforms.view.full_shading = glGetUniformLocation(view_program, "full_shading");

    ShowShader.attach( GL_VERTEX_SHADER, "camera.vert");
    ShowShader.attach( GL_FRAGMENT_SHADER, "camera.frag");
	ShowShader.link();
	show_light_depth_program = ShowShader.GetProgram();

}

CALL_MAIN(shadowmapping_app)
