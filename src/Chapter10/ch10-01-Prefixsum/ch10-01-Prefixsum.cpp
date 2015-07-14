#include <GL/glew.h>

#include "ogl/oglApp.h"
#include "ogl/vmath.h"
#include "ogl/shader.h"
#include "ogl/glDebug.h"

#include <cstdio>

#define NUM_ELEMENTS 2048

static inline float random_float()
{
    static unsigned int seed = 0x13371337;

    float res;
    unsigned int tmp;

    seed *= 16807;

    tmp = seed ^ (seed >> 4) ^ (seed << 15);

    *((unsigned int *) &res) = (tmp >> 9) | 0x3F800000;

    return (res - 1.0f);
}

class prefixsum_app : public byhj::Application
{
public:
    prefixsum_app()
        : prefix_sum_prog(0)
    {

    }

protected:
    void v_Init();
    void v_Render();

    void prefix_sum(const float * input, float * output, int elements);

    GLuint  data_buffer[2];

    float input_data[NUM_ELEMENTS];
    float output_data[NUM_ELEMENTS];

    void init_shader();
    Shader PrefixsumShader;
    GLuint  prefix_sum_prog;
};

void prefixsum_app::v_Init()
{
    glGenBuffers(2, data_buffer);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, data_buffer[0]);
    glBufferData(GL_SHADER_STORAGE_BUFFER, NUM_ELEMENTS * sizeof(float), NULL, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, data_buffer[1]);
    glBufferData(GL_SHADER_STORAGE_BUFFER, NUM_ELEMENTS * sizeof(float), NULL, GL_DYNAMIC_COPY);

    int i;

    for (i = 0; i < NUM_ELEMENTS; i++)
    {
        input_data[i] = random_float();
    }

    prefix_sum(input_data, output_data, NUM_ELEMENTS);

   init_shader();
}

void prefixsum_app::v_Render()
{
    float * ptr;
	double currentTime = glfwGetTime();
    glBindBufferRange(GL_SHADER_STORAGE_BUFFER, 0, data_buffer[0], 0, sizeof(float) * NUM_ELEMENTS);
    glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(float) * NUM_ELEMENTS, input_data);

    glBindBufferRange(GL_SHADER_STORAGE_BUFFER, 1, data_buffer[1], 0, sizeof(float) * NUM_ELEMENTS);

    glUseProgram(prefix_sum_prog);
    glDispatchCompute(1, 1, 1);

    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
    glFinish();

    glBindBufferRange(GL_SHADER_STORAGE_BUFFER, 0, data_buffer[1], 0, sizeof(float) * NUM_ELEMENTS);
    ptr = (float *)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, sizeof(float) * NUM_ELEMENTS, GL_MAP_READ_BIT);

    char buffer[1024];
    sprintf(buffer, "%2.2f %2.2f %2.2f %2.2f %2.2f %2.2f %2.2f %2.2f "
                    "%2.2f %2.2f %2.2f %2.2f %2.2f %2.2f %2.2f %2.2f\n",
                    ptr[0], ptr[1], ptr[2], ptr[3], ptr[4], ptr[5], ptr[6], ptr[7],
                    ptr[8], ptr[9], ptr[10], ptr[11], ptr[12], ptr[13], ptr[14], ptr[15]);
//    OutputDebugStringA(buffer);

    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

//    Sleep(500);
}


void prefixsum_app::init_shader()
{
    PrefixsumShader.attach(GL_COMPUTE_SHADER, "prefixsum.comp");
	PrefixsumShader.link();
    prefix_sum_prog = PrefixsumShader.GetProgram();
    /*
    prefix_sum_prog = glCreateProgram();
    glAttachShader(prefix_sum_prog, cs);

    glLinkProgram(prefix_sum_prog);

    int n;
    glGetIntegerv(GL_MAX_SHADER_STORAGE_BUFFER_BINDINGS, &n);
    */

    glShaderStorageBlockBinding(prefix_sum_prog, 0, 0);
    glShaderStorageBlockBinding(prefix_sum_prog, 1, 1);
}

void prefixsum_app::prefix_sum(const float * input, float * output, int elements)
{
    float f = 0.0f;
    int i;

    for (i = 0; i < elements; i++)
    {
        f += input[i];
        output[i] = f;
    }
}

CALL_MAIN(prefixsum_app);
