#include <GL/glew.h>
#include <sb6/sb6.h>

class WindowApp: public sb6::Application
{
public:
	WindowApp() {};
	~WindowApp() {};

	void vRender()
	{
		//Every frame we get the currentTime
		static double time = 0.0;
		time = glfwGetTime();

		//Use the current time to change the backgroud color
		const GLfloat bgColor[] = 
		{
			static_cast<float>( sin(time) ) * 0.5f + 0.5f,
			static_cast<float>( cos(time) ) * 0.5f + 0.5f,
			0.0f, 1.0f
		} ;

		//Clear the color buffer to the random color
		glClearBufferfv(GL_COLOR, 0, bgColor);
	}

};

//Call the main function
DECLARE_MAIN(WindowApp);