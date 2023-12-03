/*
Author: Wenyang Jia
Class: ECE6100
Last Date Modified: 2023-10-28
Description:

Responsible for keyboard control of the camera.

1. 'UP': Move forward
2. 'DOWN':Move backward
3. 'W': Strafe right
4. 'S': Strafe left
5. 'A': Strafe right
6. 'D': Strafe left
7. 'L': Turn light on/off


Set the camera Angle.

*/


// Include GLFW
#include <iostream>;
#include <glfw3.h>
extern GLFWwindow* window; // The "extern" keyword here is to access the variable "window" declared in tutorialXXX.cpp. This is a hack to keep the tutorials simple. Please avoid this.

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
using namespace glm;

#include "controls.hpp"

glm::mat4 ViewMatrix;
glm::mat4 ProjectionMatrix;

glm::mat4 getViewMatrix() {
	return ViewMatrix;
}
glm::mat4 getProjectionMatrix() {
	return ProjectionMatrix;
}


// Initial position : on +Z

float horizontalAngle = 3.14f;
// Initial vertical angle : none
float verticalAngle = 0.0f;
// Initial Field of View
float initialFoV = 45.0f;

float speed = 3.5f; // 3 units / second

float spinSpeed = glm::radians(99.0f);
float range = 5.0f;
float theta = 0.0f;
float phi = 0.0f;


void computeMatricesFromInputs() {

	// glfwGetTime is called only once, the first time this function is called
	static double lastTime = glfwGetTime();

	// Compute time difference between current and last frame
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);



	// Move forward
	/*if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		theta -= deltaTime;

	}*/
	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
	{
		theta -= deltaTime;

	}
	// Move backward
	/*if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		theta += deltaTime;
	}*/
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		theta += deltaTime;
	}
	// Strafe right
	//if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) 
	//{
	//	//position += right * deltaTime * speed;
	//	range -= deltaTime * speed;
	//}

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		//position += right * deltaTime * speed;
		range -= deltaTime * speed;
	}

	// Strafe left
	//if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) 
	//{
	//	//position -= right * deltaTime * speed;
	//	range += deltaTime * speed;
	//}


	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		//position -= right * deltaTime * speed;
		range += deltaTime * speed;
	}


	// Strafe right

	/*if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		phi -= deltaTime * spinSpeed;
	
	}*/

	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		phi -= deltaTime * spinSpeed;

	}



	// Strafe left
	/*if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		phi += deltaTime * spinSpeed;
		
		
	}*/
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		phi += deltaTime * spinSpeed;


	}
	// Turn light on/off
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
	{
		
		float LightPower = 0;
	}

	glm::vec3 position = glm::vec3(
		// Spherical coordinates -> xy coordinates
		range * sin(theta) * cos(phi),
		range * sin(theta) * sin(phi),
		range * cos(theta)
	);

	std::cout << position.x << " " << position.y << " " << position.z << std::endl;


	float FoV = initialFoV;// - 5 * glfwGetMouseWheel(); // Now GLFW 3 requires setting up a callback for this. It's a bit too complicated for this beginner's tutorial, so it's disabled instead.

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
	ProjectionMatrix = glm::perspective(FoV, 4.0f / 3.0f, 0.1f, 100.0f);
	// Camera matrix
	ViewMatrix = glm::lookAt(
		position,           // Camera is here
		glm::vec3(0.0f, 0.0f, 0.0f), // and looks here : at the same position, plus "direction"
		glm::vec3(cos(phi), sin(phi), 0)  // Head is up (set to 0,-1,0 to look upside-down)
	);

	// For the next frame, the "last time" will be "now"
	lastTime = currentTime;
}
