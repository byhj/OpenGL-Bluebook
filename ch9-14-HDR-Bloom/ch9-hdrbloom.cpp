#include <GL/glew.h>
#include <gl/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;

#include <object.h>
#include <ktx.cpp>

enum
{
    MAX_SCENE_WIDTH     = 2048,
    MAX_SCENE_HEIGHT    = 2048,
    SPHERE_COUNT        = 32,
};

    GLuint      tex_src;
    GLuint      tex_lut;

    GLuint      render_fbo;
    GLuint      filter_fbo[2];

    GLuint      tex_scene;
    GLuint      tex_brightpass;
    GLuint      tex_depth;
    GLuint      tex_filter[2];

    GLuint      program_render;
    GLuint      program_filter;
    GLuint      program_resolve;
    GLuint      vao;
    float       exposure;
    int         mode;
    bool        paused;
    float       bloom_factor;
    bool        show_bloom;
    bool        show_scene;
    bool        show_prefilter;
    float       bloom_thresh_min;
    float       bloom_thresh_max;

    struct
    {
        struct
        {
            int bloom_thresh_min;
            int bloom_thresh_max;
        } scene;
        struct
        {
            int exposure;
            int bloom_factor;
            int scene_factor;
        } resolve;
    } uniforms;

    GLuint      ubo_transform;
    GLuint      ubo_material;

Shader resloveShader("texture shader");
Shader filterShader("texture shader");
Shader sceneShader("texture shader");
object   obj;
bool     paused;
GLuint explode_factor_location;
bool depthClamp = true;

void init(void)  
{ //黑白相间纹理数据
        int i;
        static const GLenum buffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };

        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        load_shaders();

        static const GLfloat exposureLUT[20]   = { 11.0f, 6.0f, 3.2f, 2.8f, 2.2f, 1.90f, 1.80f, 1.80f, 1.70f, 1.70f,  1.60f, 1.60f, 1.50f, 1.50f, 1.40f, 1.40f, 1.30f, 1.20f, 1.10f, 1.00f };

        glGenFramebuffers(1, &render_fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, render_fbo);

        glGenTextures(1, &tex_scene);
        glBindTexture(GL_TEXTURE_2D, tex_scene);
        glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA16F, MAX_SCENE_WIDTH, MAX_SCENE_HEIGHT);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, tex_scene, 0);
        glGenTextures(1, &tex_brightpass);
        glBindTexture(GL_TEXTURE_2D, tex_brightpass);
        glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA16F, MAX_SCENE_WIDTH, MAX_SCENE_HEIGHT);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, tex_brightpass, 0);
        glGenTextures(1, &tex_depth);
        glBindTexture(GL_TEXTURE_2D, tex_depth);
        glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH_COMPONENT32F, MAX_SCENE_WIDTH, MAX_SCENE_HEIGHT);
        glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, tex_depth, 0);
        glDrawBuffers(2, buffers);

        glGenFramebuffers(2, &filter_fbo[0]);
        glGenTextures(2, &tex_filter[0]);
        for (i = 0; i < 2; i++)
        {
            glBindFramebuffer(GL_FRAMEBUFFER, filter_fbo[i]);
            glBindTexture(GL_TEXTURE_2D, tex_filter[i]);
            glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA16F, i ? MAX_SCENE_WIDTH : MAX_SCENE_HEIGHT, i ? MAX_SCENE_HEIGHT : MAX_SCENE_WIDTH);
            glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, tex_filter[i], 0);
            glDrawBuffers(1, buffers);
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glGenTextures(1, &tex_lut);
        glBindTexture(GL_TEXTURE_1D, tex_lut);
        glTexStorage1D(GL_TEXTURE_1D, 1, GL_R32F, 20);
        glTexSubImage1D(GL_TEXTURE_1D, 0, 0, 20, GL_RED, GL_FLOAT, exposureLUT);
        glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

        object.load("media/objects/sphere.sbm");

        glGenBuffers(1, &ubo_transform);
        glBindBuffer(GL_UNIFORM_BUFFER, ubo_transform);
        glBufferData(GL_UNIFORM_BUFFER, (2 + SPHERE_COUNT) * sizeof(vmath::mat4), NULL, GL_DYNAMIC_DRAW);

        struct material
        {
            vmath::vec3     diffuse_color;
            unsigned int    : 32;           // pad
            vmath::vec3     specular_color;
            float           specular_power;
            vmath::vec3     ambient_color;
            unsigned int    : 32;           // pad
        };

        glGenBuffers(1, &ubo_material);
        glBindBuffer(GL_UNIFORM_BUFFER, ubo_material);
        glBufferData(GL_UNIFORM_BUFFER, SPHERE_COUNT * sizeof(material), NULL, GL_STATIC_DRAW);

        material * m = (material *)glMapBufferRange(GL_UNIFORM_BUFFER, 0, SPHERE_COUNT * sizeof(material), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
        float ambient = 0.002f;
        for (i = 0; i < SPHERE_COUNT; i++)
        {
            float fi = 3.14159267f * (float)i / 8.0f;
            m[i].diffuse_color  = vmath::vec3(sinf(fi) * 0.5f + 0.5f, sinf(fi + 1.345f) * 0.5f + 0.5f, sinf(fi + 2.567f) * 0.5f + 0.5f);
            m[i].specular_color = vmath::vec3(2.8f, 2.8f, 2.9f);
            m[i].specular_power = 30.0f;
            m[i].ambient_color  = vmath::vec3(ambient * 0.025f);
            ambient *= 1.5f;
        }
        glUnmapBuffer(GL_UNIFORM_BUFFER);

}
static int tex = 0;

void ::(void)
{
    static const GLfloat black[] = { 0.0f, 0.0f, 0.0f, 0.0f };
    static const GLfloat one = 1.0f;

	static float currentTime ;
	currentTime = glutGet(GLUT_ELAPSED_TIME) / 1000.0f;

    float f =  0.9f; //(float)currentTime;

    glClearBufferfv(GL_COLOR, 0, black);
    glClearBufferfv(GL_DEPTH, 0, &one);
	glEnable(GL_DEPTH_CLAMP);

	glm::mat4 mv_matrix = glm::translate(glm::mat4(1.0), glm::vec3(0.0f, 0.0f, -2.0f)) 
		                  * glm::rotate(glm::mat4(1.0), f* 45.0f, glm::vec3(0.0f, 1.0f, 0.0f)) 
		                  * glm::rotate(glm::mat4(1.0), f* 81.0f, glm::vec3(1.0f, 0.0f, 0.0f));
	textureShader.setUniformMatrix4fv("mv_matrix", 1, 0, glm::value_ptr(mv_matrix));
   //  glUniform1f(4, sinf((float)currentTime * 8.0f) * cosf((float)currentTime * 6.0f) * 0.7f + 0.1f);
	 obj.render();
	glutSwapBuffers(); //强制绘图，去掉会变白色

}

void reshape(int w, int h) {
	glViewport(0, 0, w, h);  //视口调整
	static float aspect;
	if (w > h)
	  aspect = float(w) / float(h);
	else
	  aspect = float(h) / float(w);
	glm::mat4 proj_matrix = glm::perspective(50.0f, aspect, 1.8f, 1000.0f);
	textureShader.setUniformMatrix4fv("proj_matrix", 1, 0, glm::value_ptr(proj_matrix));

}

void idle(void )
{
 glutPostRedisplay();
}

void key(unsigned char  key, int x, int y)
{        switch (key)
        {
            case '1':
            case '2':
            case '3':
                    mode = key - '1';
                break;
            case 'B':
                    show_bloom = !show_bloom;
                break;
            case 'V':
                    show_scene = !show_scene;
                break;
            case 'A':
                    bloom_factor += 0.1f;
                break;
            case 'Z':
                    bloom_factor -= 0.1f;
                break;
            case 'S':
                    bloom_thresh_min += 0.1f;
                break;
            case 'X':
                    bloom_thresh_min -= 0.1f;
                break;
            case 'D':
                    bloom_thresh_max += 0.1f;
                break;
            case 'C':
                    bloom_thresh_max -= 0.1f;
                break;
            case 'N':
                    show_prefilter = !show_prefilter;
                    break;
            case 'M':
                    mode = (mode + 1) % 3;
                break;
            case 'P':
                    paused = !paused;
                break;
            case GLFW_KEY_KP_ADD:
                    exposure *= 1.1f;
                break;
            case GLFW_KEY_KP_SUBTRACT:
                    exposure /= 1.1f;
                break;
        }
}

int main(int argc, char ** argv)
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE); //显示模式，重要
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(640, 480);
	glutCreateWindow("Texture");
	//glewExperimental = GL_TRUE;
    glewInit();

	init();
	glutDisplayFunc(::);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);
	glutKeyboardFunc(key);
	glutMainLoop(); //循环调用注册函数display
	return 0;
}