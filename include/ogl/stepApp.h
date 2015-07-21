#ifndef LEARNAPP_H
#define LEARNAPP_H

#include <GL/glew.h>
#include <common/glDebug.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <functional>
#include <windows.h>

#ifdef WIN32
const int ScreenWidth = GetSystemMetrics(SM_CXSCREEN) * 0.75;
const int ScreenHeight = GetSystemMetrics(SM_CYSCREEN) * 0.75;
const int PosX = (GetSystemMetrics(SM_CXSCREEN) - ScreenWidth)  / 2;
const int PosY = (GetSystemMetrics(SM_CYSCREEN) - ScreenHeight) / 2;
#else
const int ScreenWidth = 1200;
const int ScreenHeight = 1000;
const int PosX = 300;
const int PosY = 100;
#endif

namespace byhj {
	class Application 
	{
	public:
		Application() {}
		virtual ~Application() {}

		void Run(byhj::Application *the_app)
		{
			app = the_app;
			std::cout << "Starting GLFW context" << std::endl;
			if (!glfwInit()) 
			{
				std::cerr << "Failed to initialize GLFW" << std::endl;
				return;
			}

			v_InitWindowInfo();

			GLFWwindow *window = glfwCreateWindow(windowInfo.Width, windowInfo.Height, windowInfo.title.c_str(), nullptr, nullptr);
			glfwSetWindowPos(window, windowInfo.posX, windowInfo.posY);
			glfwMakeContextCurrent(window);

			glfwSetKeyCallback(window, glfw_key);
			glfwSetCursorPosCallback(window, glfw_mouse);
			glfwSetScrollCallback(window, glfw_scroll);

			// GLFW Options
	       glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

			if (window == NULL)
			{
				std::cerr << "Failed to create GLFW window" << std::endl;
				glfwTerminate();
				return ;
			}	
			glewExperimental = GL_TRUE;

			//1���鿴GLSL��OpenGL�İ汾  
			if (glewInit() != GLEW_OK)
			{
				std::cerr << "Failed to initialize GLEW" << std::endl;
				return ;
			}
			const GLubyte *renderer = glGetString( GL_RENDERER );  
			const GLubyte *vendor = glGetString( GL_VENDOR );  
			const GLubyte *version = glGetString( GL_VERSION );  
			const GLubyte *glslVersion = glGetString( GL_SHADING_LANGUAGE_VERSION );  
			GLint major, minor;  
			glGetIntegerv(GL_MAJOR_VERSION, &major);  
			glGetIntegerv(GL_MINOR_VERSION, &minor);  
			std::cout << "GL Vendor    :" << vendor << std::endl;  
			std::cout << "GL Renderer  : " << renderer << std::endl;  
			std::cout << "GL Version (std::string)  : " << version << std::endl;  
			std::cout << "GL Version (integer) : " << major << "." << minor << std::endl;  
			std::cout << "GLSL Version : " << glslVersion << std::endl;    
			std::cout << "--------------------------------------------------------------------------------" << std::endl;
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major); //opengl 4.3
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //using opengl core file
			glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
			// Create a GLFWwindow object that we can use for GLFW's functions

			v_Init();

			glViewport(0, 0, windowInfo.Width, windowInfo.Height);

			while (!glfwWindowShouldClose(window)) 
			{
				glfwPollEvents();
				v_Movement(window);

				v_Render();

				glfwSwapBuffers(window);
			}
			v_Shutdown();
			glfwTerminate();
		}//run

		virtual void v_InitWindowInfo()
		{
		}
		virtual void v_Init()
		{
		}

		virtual void v_Render()
		{
		}
		virtual void v_Shutdown()
		{
		}

		virtual void v_KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) 
		{
			if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
				glfwSetWindowShouldClose(window, GL_TRUE);
		}
		virtual void v_Movement(GLFWwindow *window) {}
		virtual void v_MouseCallback(GLFWwindow* window, double xpos, double ypos) {}
		virtual void v_ScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {}

	protected:
		struct WindowInfo
		{
			WindowInfo():title("OpenGL-Step by Step:   "), 
				        Width(ScreenWidth), Height(ScreenHeight),
				        posX(PosX), posY(PosY){}
			std::string title;
			int Width;
			int Height;
			int posX, posY;
		}windowInfo;
		float GetAspect()
		{
			return static_cast<float>(ScreenWidth) / static_cast<float>(ScreenHeight);
		}
		int GetScreenWidth()
		{
			return ScreenWidth;
		}
		int GetScreenHeight()
		{
			return ScreenHeight;
		}
	protected:

	static byhj::Application *app;
	static void glfw_key(GLFWwindow * window, int key, int scancode, int action, int mode) 
	{
		app->v_KeyCallback(window,  key,  scancode, action,  mode);
	}
	static void glfw_mouse(GLFWwindow* window, double xpos, double ypos)
	{
		app->v_MouseCallback(window,  xpos, ypos);
	}
	static void glfw_scroll(GLFWwindow* window, double xoffset, double yoffset)
	{
		app->v_ScrollCallback(window,  xoffset, yoffset);
	}

	};  //class
}  //namespace 

byhj::Application * byhj::Application::app; //��̬��Ա��Ҫ����

#define CALL_MAIN(a)                                \
int main(int argc, const char **argv)               \
{                                                   \
	a *app = new a;                                 \
	app->Run(app);                                  \
	delete app;                                     \
	return 0;                                       \
}


#endif  //SB6_H