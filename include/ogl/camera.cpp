#include "camera.h"

namespace byhj
{

	// Processes input received from any keyboard-like input system. 
	//Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
	void Camera::ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime)
	{
		GLfloat velocity = this->MovementSpeed * deltaTime * 3.0f;
		if (direction == FORWARD)
			this->Position += this->Front * velocity;
		if (direction == BACKWARD)
			this->Position -= this->Front * velocity;
		if (direction == LEFT)
			this->Position -= this->Right * velocity;
		if (direction == RIGHT)
			this->Position += this->Right * velocity;
	}

	// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
	void Camera::ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch)
	{
		xoffset *= this->MouseSensitivity;
		yoffset *= this->MouseSensitivity;

		this->Yaw   += xoffset;
		this->Pitch += yoffset;

		// Make sure that when pitch is out of bounds, screen doesn't get flipped
		if (constrainPitch)
		{
			if (this->Pitch > 89.0f)
				this->Pitch = 89.0f;
			if (this->Pitch < -89.0f)
				this->Pitch = -89.0f;
		}

		// Update Front, Right and Up Vectors using the updated Eular angles
		if (this->ctr)
			this->updateCameraVectors();
	}

	// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
	void Camera::ProcessMouseScroll(GLfloat yoffset)
	{
		this->Zoom -= yoffset;
		if (this->Zoom <= 1.0f)
			this->Zoom = 1.0f;
		if (this->Zoom >= 179.0f)
			this->Zoom = 179.0f;
	}


	void Camera::updateCameraVectors()
	{
		glm::vec3 front;
		front.x = cos(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
		front.y = sin(glm::radians(this->Pitch));
		front.z = sin(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
		this->Front = glm::normalize(front);
		// Also re-calculate the Right and Up vector
		this->Right = glm::normalize(glm::cross(this->Front, this->WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		this->Up    = glm::normalize(glm::cross(this->Right, this->Front));
	}


	// Moves/alters the camera positions based on user input
	void Camera::movement(GLFWwindow *window)
	{
		if (keys[GLFW_KEY_ESCAPE] )
			glfwSetWindowShouldClose(window, GL_TRUE);

		// Camera controls
		if(keys[GLFW_KEY_W])
			ProcessKeyboard(FORWARD, deltaTime);
		if(keys[GLFW_KEY_S])
			ProcessKeyboard(BACKWARD, deltaTime);
		if(keys[GLFW_KEY_A])
			ProcessKeyboard(LEFT, deltaTime);
		if(keys[GLFW_KEY_D])
			ProcessKeyboard(RIGHT, deltaTime);
		if (keys[GLFW_KEY_C])
		{
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			ctr = false;
		}
	}

	// Is called whenever a key is pressed/released via GLFW
	void Camera::key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
	{
		//cout << key << std::endl;
		if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GL_TRUE);

		if(action == GLFW_PRESS)
			keys[key] = true;
		else if(action == GLFW_RELEASE)
			keys[key] = false;	
	}

	void Camera::mouse_callback(GLFWwindow* window, double xpos, double ypos)
	{
		if(firstMouse)
		{
			lastX = static_cast<GLfloat>(xpos);
			lastY = static_cast<GLfloat>(ypos);
			firstMouse = false;
		}

		GLfloat xoffset = static_cast<GLfloat>(xpos) - lastX;
		GLfloat yoffset = lastY - static_cast<GLfloat>(ypos);  // Reversed since y-coordinates go from bottom to left

		lastX = static_cast<GLfloat>(xpos);
		lastY = static_cast<GLfloat>(ypos);
		ProcessMouseMovement(xoffset, yoffset);
	}	


	void Camera::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
	{
		ProcessMouseScroll( static_cast<GLfloat>(yoffset) );
	}

	// Returns the view matrix calculated using Eular Angles and the LookAt Matrix
	glm::mat4 Camera::GetViewMatrix() const
	{
		return glm::lookAt(this->Position, this->Position + this->Front, this->Up);
	}
	float Camera::GetZoom() const
	{
		return glm::radians(Zoom);
	}

	void Camera::update(GLfloat dt)
	{
		deltaTime = dt;
	}

	void Camera::SetPos(const glm::vec3 &pos)
	{
		Position = pos;
	}

	glm::vec3 Camera::GetPos() const
	{
		return Position;
	}
	glm::vec3 Camera::GetFront() const 
	{
		return Front;
	}
}