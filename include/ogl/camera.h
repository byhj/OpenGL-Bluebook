#ifndef CAMERA_H
#define CAMERA_H

// Std. Includes
#include <vector>

// GL Includes
#include <GL/glew.h> 
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace byhj
{


// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// Default camera values
const GLfloat YAW        = -90.0f;
const GLfloat PITCH      =  0.0f;
const GLfloat SPEED      =  1.0f;
const GLfloat SENSITIVTY =  0.05f;
const GLfloat ZOOM       =  45.0f;


// An abstract camera class that processes input and calculates the corresponding Eular Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
    // Constructor with vectors
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
		   glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), 
		   GLfloat yaw = YAW, GLfloat pitch = PITCH) 
		   : Front(glm::vec3(0.0f, 0.0f, -1.0f)), 
		     MovementSpeed(SPEED), 
		     MouseSensitivity(SENSITIVTY),
		     Zoom(ZOOM)
    {
        this->Position = position;
        this->WorldUp = up;
        this->Yaw = yaw;
        this->Pitch = pitch;
		this->ctr = true;
        this->updateCameraVectors();

		firstMouse = true;
		deltaTime = 0.0f;

		for (int i = 0; i != 1024; ++ i)
			keys[i] = false;
    }

    // Constructor with scalar values
    Camera(GLfloat posX, GLfloat posY, GLfloat posZ, 
		   GLfloat upX, GLfloat upY, 
		   GLfloat upZ, GLfloat yaw, GLfloat pitch) 
		   : Front(glm::vec3(0.0f, 0.0f, -1.0f)), 
		     MovementSpeed(SPEED), 
			 MouseSensitivity(SENSITIVTY), 
			 Zoom(ZOOM)
    {
        this->Position = glm::vec3(posX, posY, posZ);
        this->WorldUp = glm::vec3(upX, upY, upZ);
        this->Yaw = yaw;
        this->Pitch = pitch;
        this->updateCameraVectors();
    }

public:


	// Moves/alters the camera positions based on user input
	void movement(GLFWwindow *window);
	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
	void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

	float GetZoom() const;
    glm::mat4 GetViewMatrix() const;
	void update(GLfloat dt);
	void SetPos(const glm::vec3 &pos);
	glm::vec3 GetPos() const;
	glm::vec3 GetFront() const;

private:

	void ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime);
	void ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true);
	void ProcessMouseScroll(GLfloat yoffset);

	// Calculates the front vector from the Camera's (updated) Eular Angles
	void updateCameraVectors();


	// Camera Attributes
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;
	// Eular Angles
	GLfloat Yaw;
	GLfloat Pitch;
	// Camera options
	GLfloat MovementSpeed;
	GLfloat MouseSensitivity;
	GLfloat Zoom;

	bool ctr;
	bool keys[1024];
	GLfloat lastX, lastY;
	bool firstMouse;
	GLfloat deltaTime;
};


}

#endif