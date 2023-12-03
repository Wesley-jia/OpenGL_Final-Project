/*
Author: Wenyang Jia
Class:  ECE6122 
Last Date Modified: 01/12/2023

Description:

1.	Create an oversized floor with a textured image. There is one provided but you are
	free to use your own images.

2.  The four objects are rendered correctly with lighting and material properties. Have
	some general ambient and diffuse lighting effects.

3.	The four objects do not move and only start to move when the user presses the “g”
	key.

4.  The four objects start to move and rotate randomly around the area. The object
	shall collide and bounce off each other and the floor.

5.  Each of the four 3D objects has an internal light that randomly changes intensity.

6.  The camera view should always point towards the center of the scene.

7.  Pressing the up/down arrow keys should zoom in and out.

8.  Pressing the left/right arrow keys rotate either the camera view or the model left
	and right.

9.  Pressing the “u” and “d” keys causes the camera to rotate up or down. Pressing
	escape key ends the application.

*/

// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <vector>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <glfw3.h>
GLFWwindow* window;

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/norm.hpp>
using namespace glm;

// Include AntTweakBar
#include <AntTweakBar.h>


#include <common/shader.hpp>
#include <common/texture.hpp>
#include <common/controls.hpp>
#include <common/objloader.hpp>
#include <common/vboindexer.hpp>
#include <common/quaternion_utils.hpp> 
vec3 gPosition1(1.8f, 0.0f, 0.9f);
vec3 gOrientation1;

vec3 gPosition2(0.0f, -1.8f, 0.9f);
quat gOrientation2;

vec3 gPosition3(-1.8f, 0.0f, 0.9f);
vec3 gOrientation3;


vec3 gPosition4(0.0f, 1.8f, 0.9f);
vec3 gOrientation4;

//Generate a random number between 0 and 1
//Adjust to the specified range

float randomFloat(float a, float b) {
	float random = ((float)rand()) / (float)RAND_MAX; 
	float diff = b - a;
	return a + random * diff; 
}

bool isLightingEnabled = true; // Lighting enabled by default



int main(void)
{
	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		getchar();
		return -1;
	}


	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(1024, 768, "Tutorial 09 - Rendering several models", NULL, NULL);
	if (window == NULL) 
	{
		fprintf(stderr, "Failed to open GLFW window.\n");
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	if (glewInit() != GLEW_OK)
	{
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	// Hide the mouse and enable unlimited mouvement
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// Set the mouse at the center of the screen
	glfwPollEvents();
	glfwSetCursorPos(window, 1024 / 2, 768 / 2);

	// Dark blue background
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);

	// Create and compile our GLSL program from the shaders
	GLuint programID = LoadShaders("StandardShading.vertexshader", "StandardShading.fragmentshader");

	// Get a handle for our "MVP" uniform
	GLuint MatrixID = glGetUniformLocation(programID, "MVP");
	GLuint ViewMatrixID = glGetUniformLocation(programID, "V");
	GLuint ModelMatrixID = glGetUniformLocation(programID, "M");

	// Get a handle for our buffers
	GLuint vertexPosition_modelspaceID = glGetAttribLocation(programID, "vertexPosition_modelspace");
	GLuint vertexUVID = glGetAttribLocation(programID, "vertexUV");
	GLuint vertexNormal_modelspaceID = glGetAttribLocation(programID, "vertexNormal_modelspace");

	// Load the texture
	GLuint Texture = loadDDS("uvmap.DDS");

	// Get a handle for our "myTextureSampler" uniform
	GLuint TextureID = glGetUniformLocation(programID, "myTextureSampler");

	// Read our .obj file
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> normals;
	bool res = loadOBJ("suzanne.obj", vertices, uvs, normals);

	std::vector<unsigned short> indices;
	std::vector<glm::vec3> indexed_vertices;
	std::vector<glm::vec2> indexed_uvs;
	std::vector<glm::vec3> indexed_normals;
	indexVBO(vertices, uvs, normals, indices, indexed_vertices, indexed_uvs, indexed_normals);

	// Load it into a VBO

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(glm::vec3), &indexed_vertices[0], GL_STATIC_DRAW);

	GLuint uvbuffer;
	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_uvs.size() * sizeof(glm::vec2), &indexed_uvs[0], GL_STATIC_DRAW);

	GLuint normalbuffer;
	glGenBuffers(1, &normalbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
	glBufferData(GL_ARRAY_BUFFER, indexed_normals.size() * sizeof(glm::vec3), &indexed_normals[0], GL_STATIC_DRAW);

	// Generate a buffer for the indices as well
	GLuint elementbuffer;
	glGenBuffers(1, &elementbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);

	// Get a handle for our "LightPosition" uniform
	glUseProgram(programID);
	//GLuint LightID = glGetUniformLocation(programID, "LightPosition_worldspace");
	GLuint LightID = glGetUniformLocation(programID, "LightPosition_worldspace");

	double lastTime = glfwGetTime();
	double lastFrameTime = lastTime;
	int nbFrames = 0;

	do {
		// Measure speed
		double currentTime = glfwGetTime();
		float deltaTime = (float)(currentTime - lastFrameTime);
		lastFrameTime = currentTime;
		nbFrames++;
		if (currentTime - lastTime >= 1.0) { // If last prinf() was more than 1sec ago
			// printf and reset
			printf("%f ms/frame\n", 1000.0 / double(nbFrames));
			nbFrames = 0;
			lastTime += 1.0;
		}
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// Compute the MVP matrix from keyboard and mouse input
		computeMatricesFromInputs();
		glm::mat4 ProjectionMatrix = getProjectionMatrix();
		glm::mat4 ViewMatrix = getViewMatrix();


		////// Start of the rendering of the first object //////

		// Use our shader
		glUseProgram(programID);

		//temp
		// In the render environment @ 2023-09-25 , Update time : 01/12/2023
		if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
			isLightingEnabled = !isLightingEnabled; // Switching light state
			glUniform1i(glGetUniformLocation(programID, "useLighting"), isLightingEnabled);
		}

		//tempdowm

		glm::vec3 lightPos = glm::vec3(2, 2, 2);//def 554

		//glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);




		//Make the G important !!!!!   Key Point !!!!!
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]); // This one doesn't change between objects, so this can be done once for all objects that use "programID"
		//Make the G important !!!!!   Key Point !!!!!


		float angle = glm::radians(10.0f); // The rotation Angle is 45 degrees
		glm::vec3 axis = glm::vec3(0.0f, 1.0f, 0.0f); // The axis of rotation is the Y-axis


		//first OBJ

		glm::mat4 ModelMatrix1 = glm::mat4(1.0);
		ModelMatrix1 = glm::translate(glm::mat4(1.0f), glm::vec3(1.8f, 0.0f, 0.9f));
		angle = glm::radians(90.0f); //  The rotation Angle is 45 degrees
		axis = glm::vec3(1.0f, 0.0f, 0.0f); // The axis of rotation is the Y-axis
		ModelMatrix1 = glm::rotate(ModelMatrix1, angle, axis); // The model matrix is transformed by rotation
		angle = glm::radians(90.0f); // The rotation Angle is 45 degrees
		axis = glm::vec3(0.0f, 1.0f, 0.0f); //The axis of rotation is the Y-axis
		ModelMatrix1 = glm::rotate(ModelMatrix1, angle, axis); // The model matrix is transformed by rotation
		glm::mat4 MVP1 = ProjectionMatrix * ViewMatrix * ModelMatrix1;

		// Send our transformation to the currently bound shader, 
		// in the "MVP" uniform
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP1[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix1[0][0]);


		// Bind our texture in Texture Unit 0
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, Texture);
		// Set our "myTextureSampler" sampler to user Texture Unit 0
		glUniform1i(TextureID, 0);

		// 1rst attribute buffer : vertices
		glEnableVertexAttribArray(vertexPosition_modelspaceID);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			vertexPosition_modelspaceID, // The attribute we want to configure
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
		);

		// 2nd attribute buffer : UVs
		glEnableVertexAttribArray(vertexUVID);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glVertexAttribPointer(
			vertexUVID,                       // The attribute we want to configure
			2,                                // size : U+V => 2
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		// 3rd attribute buffer : normals
		glEnableVertexAttribArray(vertexNormal_modelspaceID);
		glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
		glVertexAttribPointer(
			vertexNormal_modelspaceID,        // The attribute we want to configure
			3,                                // size
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);

		// Index buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);

		//glm::vec3 lightPos = glm::vec3(4, 4, 4);
		glUniform3f(LightID, lightPos.x, lightPos.y, lightPos.z);

		{ // Euler

			// As an example, rotate arount the vertical axis at 180?sec
			gOrientation1.y += 3.14159f / 2.0f * deltaTime;

			glm::vec3 minBounds = glm::vec3(-5.0f, -5.0f, 0.0f); // Minimum boundary
			glm::vec3 maxBounds = glm::vec3(5.0f, 5.0f, 5.0f); // Maximum boundary

			glm::vec3 velocity = glm::vec3(1.5f, 0.5f, -0.5f); // initial velocity

			//float moveSpeed = 0.5f; // 控制移动速度
			// 更新速度向量（可以根据需要调整这里的随机数范围）

			velocity.x += randomFloat(-0.5f, 0.5f);
			velocity.y += randomFloat(-0.5f, 0.5f);
			velocity.z += randomFloat(-0.5f, 0.5f);
			

			
			if (gPosition1.x <= minBounds.x || gPosition1.x >= maxBounds.x || gPosition1.x == gPosition4.x || gPosition1.x == gPosition2.x || gPosition1.x == gPosition3.x) {
				velocity.x = -150*velocity.x; // Reverse X velocity , At the same time, the coordinates are detected, and if they coincide, the speed is reversed
			}
			if (gPosition1.y <= minBounds.y || gPosition1.y >= maxBounds.y || gPosition1.y == gPosition4.y || gPosition1.y == gPosition2.y || gPosition1.y == gPosition3.y) {
				velocity.y = -1900*velocity.y; //  Reverse Y velocity , At the same time, the coordinates are detected, and if they coincide, the speed is reversed
			}
			if (gPosition1.z <= minBounds.z || gPosition1.z >= maxBounds.z || gPosition1.z == gPosition4.z || gPosition1.z == gPosition2.z || gPosition1.z == gPosition3.z) {
				velocity.z = -1400*velocity.z; //  Reverse Z velocity , At the same time, the coordinates are detected, and if they coincide, the speed is reversed
			}

			// Update the position
			gPosition1 += velocity * deltaTime;
			//float x = velocity.x; // 
			//float y = velocity.y;
			//float z = velocity.z;
			// 控制速度不超过最大速度
			//velocity = glm::clamp(velocity, -moveSpeed, moveSpeed);

		

			

			// Make sure the object stays within the boundary
			gPosition1 = glm::clamp(gPosition1, minBounds, maxBounds);

			// Build the model matrix
			glm::mat4 RotationMatrix = eulerAngleYXZ(gOrientation1.y, gOrientation1.x, gOrientation1.z);
			
			glm::mat4 TranslationMatrix = translate(glm::mat4(1.0f), gPosition1);//glm::mat4 TranslationMatrix = translate(mat4(), gPosition1); // A bit to the left
			glm::mat4 ScalingMatrix = scale(mat4(), vec3(1.0f, 1.0f, 1.0f));
			glm::mat4 ModelMatrix = TranslationMatrix * RotationMatrix * ScalingMatrix;

			glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;



			// Send our transformation to the currently bound shader, 
			// in the "MVP" uniform
			glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
			glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);
			


			
		}

		
		
		// Draw the triangles !
		glDrawElements(
			GL_TRIANGLES,      // mode
			indices.size(),    // count
			GL_UNSIGNED_SHORT,   // type
			(void*)0           // element array buffer offset
		);

		// BUT the Model matrix is different (and the MVP too)
		glm::mat4 ModelMatrix2 = glm::mat4(1.0);
		ModelMatrix2 = glm::translate(ModelMatrix2, glm::vec3(0.0f, -1.8f, 0.9f));
		angle = glm::radians(90.0f); 
		axis = glm::vec3(1.0f, 0.0f, 0.0f); // 
		ModelMatrix2 = glm::rotate(ModelMatrix2, angle, axis); // The model matrix is transformed by rotation
		
		//ModelMatrix2 = glm::rotate(ModelMatrix2, angle, axis); // The model matrix is transformed by rotation
		glm::mat4 MVP2 = ProjectionMatrix * ViewMatrix * ModelMatrix2;

		// Send our transformation to the currently bound shader, 
		// in the "MVP" uniform
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP2[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix2[0][0]);


		// The rest is exactly the same as the first object

		// 1rst attribute buffer : vertices
		
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(vertexPosition_modelspaceID, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		// 2nd attribute buffer : UVs
		//glEnableVertexAttribArray(vertexUVID); // Already enabled
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glVertexAttribPointer(vertexUVID, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

		// 3rd attribute buffer : normals
		
		glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
		glVertexAttribPointer(vertexNormal_modelspaceID, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		// Index buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
		{ // Euler

			// As an example, rotate arount the vertical axis at 180?sec
			gOrientation2.y += 3.14159f / 2.0f * deltaTime;

			glm::vec3 minBounds = glm::vec3(-5.0f, -5.0f, 0.0f); 
			glm::vec3 maxBounds = glm::vec3(5.0f, 5.0f, 5.0f); 

			glm::vec3 velocity = glm::vec3(0.5f, 0.5f, 0.5f); 

			//float moveSpeed = 0.5f; // 控制移动速度
			// 更新速度向量（可以根据需要调整这里的随机数范围）

			velocity += randomFloat(-0.5f, 0.5f);
			velocity += randomFloat(-0.5f, 0.5f);
			velocity += randomFloat(-0.5f, 0.5f);



			if (gPosition2.x <= minBounds.x || gPosition2.x >= maxBounds.x|| gPosition2.x == gPosition4.x || gPosition2.x == gPosition1.x || gPosition2.x == gPosition3.x) {
				velocity.x = -1600 * velocity.x; 
			}
			if (gPosition2.y <= minBounds.y || gPosition2.y >= maxBounds.y|| gPosition2.y == gPosition4.y || gPosition2.y == gPosition1.y || gPosition2.y == gPosition3.y) {
				velocity.y = -1300 * velocity.y; 
			}
			if (gPosition2.z <= minBounds.z || gPosition2.z >= maxBounds.z|| gPosition2.z == gPosition4.z || gPosition2.z == gPosition1.z || gPosition2.z == gPosition3.z) {
				velocity.z = -900 * velocity.z; 
			}

			
			gPosition2 += velocity * deltaTime;
			//float x = velocity.x; // 根据需要调整范围
			//float y = velocity.y;
			//float z = velocity.z;
			// 控制速度不超过最大速度
			//velocity = glm::clamp(velocity, -moveSpeed, moveSpeed);





			
			gPosition2 = glm::clamp(gPosition2, minBounds, maxBounds);
			// Build the model matrix
			glm::mat4 RotationMatrix = eulerAngleYXZ(gOrientation2.y, gOrientation2.x, gOrientation2.z);
			glm::mat4 TranslationMatrix = translate(mat4(), gPosition2); // A bit to the left
			glm::mat4 ScalingMatrix = scale(mat4(), vec3(1.0f, 1.0f, 1.0f));
			glm::mat4 ModelMatrix = TranslationMatrix * RotationMatrix * ScalingMatrix;

			glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

			// Send our transformation to the currently bound shader, 
			// in the "MVP" uniform
			glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
			glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);
		}
		// Draw the triangles !
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, (void*)0);


		////// End of rendering of the second object //////



		//third OBJ
				// BUT the Model matrix is different (and the MVP too)
		glm::mat4 ModelMatrix3 = glm::mat4(1.0);
		ModelMatrix3 = glm::translate(ModelMatrix3, glm::vec3(-1.8f, 0.0f, 0.9f));
		angle = glm::radians(90.0f); 
		axis = glm::vec3(1.0f, 0.0f, 0.0f); 
		ModelMatrix3 = glm::rotate(ModelMatrix3, angle, axis);
		angle = glm::radians(-90.0f); 
		axis = glm::vec3(0.0f, 1.0f, 0.0f); 
		ModelMatrix3 = glm::rotate(ModelMatrix3, angle, axis); 
		glm::mat4 MVP3 = ProjectionMatrix * ViewMatrix * ModelMatrix3;

		// Send our transformation to the currently bound shader, 
		// in the "MVP" uniform
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP3[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix3[0][0]);


		// The rest is exactly the same as the first object

		// 1rst attribute buffer : vertices
		//glEnableVertexAttribArray(vertexPosition_modelspaceID); // Already enabled
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(vertexPosition_modelspaceID, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		// 2nd attribute buffer : UVs
		//glEnableVertexAttribArray(vertexUVID); // Already enabled
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glVertexAttribPointer(vertexUVID, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

		// 3rd attribute buffer : normals
		//glEnableVertexAttribArray(vertexNormal_modelspaceID); // Already enabled
		glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
		glVertexAttribPointer(vertexNormal_modelspaceID, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		// Index buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
		{ // Euler

			// As an example, rotate arount the vertical axis at 180?sec
			gOrientation3.y += 3.14159f / 2.0f * deltaTime;

			glm::vec3 minBounds = glm::vec3(-5.0f, -5.0f, 0.0f); 
			glm::vec3 maxBounds = glm::vec3(5.0f, 5.0f, 5.0f); 

			glm::vec3 velocity = glm::vec3(-1.5f, .0f, 0.5f); 

			//float moveSpeed = 0.5f; // 控制移动速度
			// 更新速度向量（可以根据需要调整这里的随机数范围）

			velocity += randomFloat(-0.5f, 0.5f);
			velocity += randomFloat(-0.5f, 0.5f);
			velocity += randomFloat(-0.5f, 0.5f);



			if (gPosition3.x <= minBounds.x || gPosition3.x >= maxBounds.x || 
				gPosition3.x ==gPosition1.x||gPosition3.x == gPosition2.x || gPosition3.x == gPosition4.x) {
				velocity.x = -2500 * velocity.x; 
			}
			if (gPosition3.y <= minBounds.y || gPosition3.y >= maxBounds.y|| gPosition3.y == gPosition1.y || gPosition3.y == gPosition2.y || gPosition3.y == gPosition4.y) {
				velocity.y = -1500 * velocity.y; 
			}
			if (gPosition3.z <= minBounds.z || gPosition3.z >= maxBounds.z|| gPosition3.z == gPosition1.z || gPosition3.z == gPosition2.z || gPosition3.z == gPosition4.z) {
				velocity.z = -1500 * velocity.z; 
			}

			
			gPosition3 += velocity * deltaTime;
			//float x = velocity.x; // 根据需要调整范围
			//float y = velocity.y;
			//float z = velocity.z;
			// 控制速度不超过最大速度
			//velocity = glm::clamp(velocity, -moveSpeed, moveSpeed);





			
			gPosition3 = glm::clamp(gPosition3, minBounds, maxBounds);
			// Build the model matrix
			glm::mat4 RotationMatrix = eulerAngleYXZ(gOrientation3.x, gOrientation3.y, gOrientation3.z);
			glm::mat4 TranslationMatrix = translate(mat4(), gPosition3); // A bit to the left
			glm::mat4 ScalingMatrix = scale(mat4(), vec3(1.0f, 1.0f, 1.0f));
			glm::mat4 ModelMatrix = TranslationMatrix * RotationMatrix * ScalingMatrix;

			glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

			// Send our transformation to the currently bound shader, 
			// in the "MVP" uniform
			glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
			glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);
		}
		// Draw the triangles !
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, (void*)0);


		////// End of rendering of the third object //////


	  //forth OBJ @ 2023-09-26, Update time : 01/12/2023
		glm::mat4 ModelMatrix4 = glm::mat4(1.0);
		ModelMatrix4 = glm::translate(ModelMatrix4, glm::vec3(0.0f, 1.8f, 0.9f));
		angle = glm::radians(90.0f); 
		axis = glm::vec3(1.0f, 0.0f, 0.0f); 
		ModelMatrix4 = glm::rotate(ModelMatrix4, angle, axis); 
		angle = glm::radians(180.0f); 
		axis = glm::vec3(0.0f, 1.0f, 0.0f); 
		ModelMatrix4 = glm::rotate(ModelMatrix4, angle, axis); 
		glm::mat4 MVP4 = ProjectionMatrix * ViewMatrix * ModelMatrix4;

		// Send our transformation to the currently bound shader, 
		// in the "MVP" uniform
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP4[0][0]);
		glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix4[0][0]);


		// The rest is exactly the same as the first object

		// 1rst attribute buffer : vertices
		
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(vertexPosition_modelspaceID, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		// 2nd attribute buffer : UVs
		
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glVertexAttribPointer(vertexUVID, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

		// 3rd attribute buffer : normals
		
		glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
		glVertexAttribPointer(vertexNormal_modelspaceID, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		// Index buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer);
		{ // Euler

			// As an example, rotate arount the vertical axis at 180?sec
			gOrientation4.y += 3.14159f / 2.0f * deltaTime;

			glm::vec3 minBounds = glm::vec3(-5.0f, -5.0f, 0.0f); 
			glm::vec3 maxBounds = glm::vec3(5.0f, 5.0f, 5.0f); 

			glm::vec3 velocity = glm::vec3(-0.5f, 1.5f, -0.5f); 

			//float moveSpeed = 0.5f; // 控制移动速度
			// 更新速度向量（可以根据需要调整这里的随机数范围）

			velocity += randomFloat(-0.5f, 0.5f);
			velocity += randomFloat(-0.5f, 0.5f);
			velocity += randomFloat(-0.5f, 0.5f);



			if (gPosition4.x <= minBounds.x || gPosition4.x >= maxBounds.x || gPosition4.x == gPosition1.x || gPosition4.x == gPosition2.x || gPosition4.x == gPosition3.x) {
				velocity.x = -500 * velocity.x; 
			}
			if (gPosition4.y <= minBounds.y || gPosition4.y >= maxBounds.y|| gPosition4.y == gPosition1.y || gPosition4.y == gPosition2.y || gPosition4.y == gPosition3.y) {
				velocity.y = -1700 * velocity.y; 
			}
			if (gPosition4.z <= minBounds.z || gPosition4.z >= maxBounds.z|| gPosition4.z == gPosition1.z || gPosition4.z == gPosition2.z || gPosition4.z == gPosition3.z) {
				velocity.z = -1400 * velocity.z; 
			}

			
			gPosition4 += velocity * deltaTime;
			//float x = velocity.x; // 根据需要调整范围
			//float y = velocity.y;
			//float z = velocity.z;
			// 控制速度不超过最大速度
			//velocity = glm::clamp(velocity, -moveSpeed, moveSpeed);





			// 确保物体保持在边界内
			gPosition4 = glm::clamp(gPosition4, minBounds, maxBounds);
			// Build the model matrix
			glm::mat4 RotationMatrix = eulerAngleYXZ(gOrientation4.x, gOrientation4.z, gOrientation4.y);
			glm::mat4 TranslationMatrix = translate(mat4(), gPosition4); // A bit to the left
			glm::mat4 ScalingMatrix = scale(mat4(), vec3(1.0f, 1.0f, 1.0f));
			glm::mat4 ModelMatrix = TranslationMatrix * RotationMatrix * ScalingMatrix;

			glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

			// Send our transformation to the currently bound shader, 
			// in the "MVP" uniform
			glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
			glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &ModelMatrix[0][0]);
			glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &ViewMatrix[0][0]);
		}
		// Draw the triangles !
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, (void*)0);

		glm::mat4 ModelMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(3.0f, 3.0f, 0.0f));
		glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

		// Send transformation to the shader
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

		// Set color to green
		glUniform3f(programID, 0.0f, 1.0f, 0.0f);

		// Define vertices and indices for a 4x4 rectangle
		GLfloat vertices[] = {
			-100.0f, -100.0f, 0.0f,
			 100.0f, -100.0f, 0.0f,
			 100.0f,  100.0f, 0.0f,
			-100.0f,  100.0f, 0.0f,
		};

		//Define the color for the rectangle (green)
	/*	GLfloat rectangleColors[] = {
				0.327f,  0.483f,  0.844f,
	            0.822f,  0.569f,  0.201f,
	            0.435f,  0.602f,  0.223f,
	            0.310f,  0.747f,  0.185f,
		};*/

		GLuint indices[] = {
			0, 1, 2,
			0, 2, 3
		};
		
		//glColorPointer(3, GL_FLOAT, 0, rectangleColors);

		// Vertex buffer object
		GLuint rectVertexBuffer;
		glGenBuffers(1, &rectVertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, rectVertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		// Index buffer object
		GLuint rectIndexBuffer;
		glGenBuffers(1, &rectIndexBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rectIndexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		// Vertex attribute pointers
		glEnableVertexAttribArray(vertexPosition_modelspaceID);
		glBindBuffer(GL_ARRAY_BUFFER, rectVertexBuffer);
		glVertexAttribPointer(vertexPosition_modelspaceID, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		// Draw the rectangle
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		
		///end of rendering of the  rectangle





		glDisableVertexAttribArray(vertexPosition_modelspaceID);
		glDisableVertexAttribArray(vertexUVID);
		glDisableVertexAttribArray(vertexNormal_modelspaceID);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);

	// Cleanup VBO and shader
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &uvbuffer);
	glDeleteBuffers(1, &normalbuffer);
	glDeleteBuffers(1, &elementbuffer);
	glDeleteProgram(programID);
	glDeleteTextures(1, &Texture);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

	return 0;
}

