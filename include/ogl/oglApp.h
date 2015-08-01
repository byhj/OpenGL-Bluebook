#ifndef OGLAPP_H
#define OGLAPP_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <windows.h>
#include <memory>

#ifdef _WIN32
const int ScreenWidth = static_cast<int>( GetSystemMetrics(SM_CXSCREEN) * 0.75 );
const int ScreenHeight = static_cast<int>(  GetSystemMetrics(SM_CYSCREEN) * 0.75 );
const int PosX = (GetSystemMetrics(SM_CXSCREEN) - ScreenWidth) ;
const int PosY = (GetSystemMetrics(SM_CYSCREEN) - ScreenHeight);
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

	public:
		void Run(std::shared_ptr<byhj::Application> the_app);

		//Override
		virtual void v_InitInfo() = 0;
		virtual void v_Init()	  = 0;
		virtual void v_Render()	  = 0;
		virtual void v_Shutdown() = 0;

		virtual void v_KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mode) 
		{
			if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
				glfwSetWindowShouldClose(window, GL_TRUE);
			if (key ==  GLFW_KEY_C && action == GLFW_PRESS)
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}

		virtual void v_Movement(GLFWwindow *window) {}
		virtual void v_MouseCallback(GLFWwindow* window, double xpos, double ypos) {}
		virtual void v_ScrollCallback(GLFWwindow* window, double xoffset, double yoffset) {}

	protected:
		struct WindowInfo
		{
			WindowInfo():title("OpenGL BlueBook : "), 
				        Width(ScreenWidth), 
						Height(ScreenHeight),
				        posX(PosX), 
						posY(PosY) {}

			std::string title;
			int Width;
			int Height;
			int posX, posY;
		}windowInfo;

		float GetAspect();
		int GetScreenWidth();
		int GetScreenHeight();

	protected:
	    static  std::shared_ptr<byhj::Application> app;

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



#endif  //