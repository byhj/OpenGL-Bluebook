#ifndef SB6_H
#define SB6_H

#include <gl/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <functional>

#ifdef _WIN32
#include <windows.h>
//If PLATFORM IS WIN32, we put the render window to the middle of window
const int g_ScreenWidth = GetSystemMetrics(SM_CXSCREEN) * 0.75;
const int g_ScreenHeight = GetSystemMetrics(SM_CYSCREEN) * 0.75;
const GLfloat g_Aspect = float(g_ScreenWidth) / float(g_ScreenHeight);
const int g_PosX = (GetSystemMetrics(SM_CXSCREEN) - g_ScreenWidth) / 2;
const int g_PosY = (GetSystemMetrics(SM_CYSCREEN) - g_ScreenHeight) / 2;

#else
const int g_ScreenWidth = 1000;
const int g_ScreenHeight = 800;
const int g_PosX = 300;
const int g_PosY = 100;
#endif

namespace byhj {
	class Application 
	{
	public:
		Application() {}
		virtual ~Application() {}

		virtual void v_Run(byhj::Application *the_app)
		{
			app = the_app;
			std::cout << "Starting GLFW context" << std::endl;
			if (!glfwInit()) 
			{
				std::cerr << "Failed to initialize GLFW" << std::endl;
				return;
			}

			v_InitWindowInfo();

			GLFWwindow *window = glfwCreateWindow(windowInfo.Width, windowInfo.Height, 
				                                  windowInfo.title.c_str(), nullptr, nullptr);

			glfwSetWindowPos(window, windowInfo.posX, windowInfo.posY);
			glfwMakeContextCurrent(window);

			glfwSetKeyCallback(window, glfw_key);

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
			std::cout << "------------------------------------------------------------------------------"
				      << std::endl;
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
		virtual void v_Keyboard(GLFWwindow * window, int key, int scancode, int action, int mode)
		{
			if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
				glfwSetWindowShouldClose(window, GL_TRUE);
		}
	protected:
		struct WindowInfo
		{
			WindowInfo():title("Bluebook"), Width(g_ScreenWidth), Height(g_ScreenHeight), posX(g_PosX), posY(g_PosY){}
			std::string title;
			int Width;
			int Height;
			int posX, posY;
		}windowInfo;

		float GetAspect()
		{
			return static_cast<float>(g_ScreenWidth) / static_cast<float>(g_ScreenHeight);
		}
		int GetScreenWidth()
		{
			return g_ScreenWidth;
		}
		int GetScreenHeight()
		{
			return g_ScreenHeight;
		}
	protected:

	static byhj::Application *app;
	static void glfw_key(GLFWwindow * window, int key, int scancode, int action, int mode) 
	{
		app->v_Keyboard(window,  key,  scancode, action,  mode);
	}

	};  //class
}  //namespace 

byhj::Application * byhj::Application::app; 

#define CALL_MAIN(a)                                \
int main(int argc, const char **argv)               \
{                                                   \
	a *app = new a;                                 \
	app-> v_Run(app);                               \
	delete app;                                     \
	return 0;                                       \
}


#endif  //SB6_H