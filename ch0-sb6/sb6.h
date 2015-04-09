#ifndef SB6_H
#define SB6_H

#include <GL/glfw3.h>
#include <iostream>
#include <string>

namespace sb6 {
	class Application 
	{
	public:
		Application() {}
		virtual ~Application() {}

		virtual void run()
		{
			bool running = true;
			std::cout << "Starting GLFW context" << std::endl;
			if (!glfwInit()) 
			{
				std::cerr << "Failed to initialize GLFW" << std::endl;
				return;
			}
			initWindowInfo();
			GLFWwindow *window = glfwCreateWindow(windowInfo.Width, windowInfo.Height, windowInfo.title.c_str(), nullptr, nullptr);
			glfwSetWindowPos(window, windowInfo.posX, windowInfo.posY);
	        glfwMakeContextCurrent(window);

			glfwSetKeyCallback(window, key_callback);
			glfwSetCursorPosCallback(window, mouse_callback);
			glfwSetScrollCallback(window, scroll_callback);

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
	        std::cout << "------------------------------------------------------------------------------" << std::endl;
		    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major); //opengl 4.3
	        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //using opengl core file
	        glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
		    // Create a GLFWwindow object that we can use for GLFW's functions
		

		    init();
			glViewport(0, 0, windowInfo.Width, windowInfo.Height);
	
			// GLFW Options
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		    while (!glfwWindowShouldClose(window)) 
		    {
			   glfwPollEvents();
               render();
		       glfwSwapBuffers(window);
		    }
		    shotdown();
		    glfwTerminate();
        }//run

		virtual void initWindowInfo()
		{
		}
		virtual void init()
		{
		}

		virtual void render()
		{
		}
		virtual void shotdown()
		{
		}
		virtual void  key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {};
		virtual void  do_movement() {};
		virtual void  mouse_callback(GLFWwindow* window, double xpos, double ypos) {};
		virtual void  scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {};
	protected:
		struct WindowInfo
		{
		   WindowInfo():title("Bluebook"), Width(1200), Height(800), posX(400), posY(100){}
		   std::string title;
		   int Width;
		   int Height;
		   int posX, posY;
		}windowInfo;
	};  //class
}  //namespace 

#define DECLARE_MAIN(a)                             \
sb6::Application *app = 0;                          \
int main(int argc, const char **argv)              \
{                                                   \
    a *app = new a;                                 \
    app->run();                                  \
    delete app;                                     \
    return 0;                                       \
}


#endif  //SB6_H