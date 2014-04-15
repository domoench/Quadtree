#include <stdio.h>
#include <stdlib.h>
#include "constants.h"
#include "scene.h"

#include "shader.hpp"
#include "glm/glm.hpp"
#include <GL/glfw.h>

using namespace glm;

GLFWwindow* window;
Scene scene;

// Local Function Declarations
void Init();
void Display();
void KeyHandler();
void MouseButton();
void MouseMotion();

int main(int argc, char** argv)
{
  // Initialise GLFW
	if (!glfwInit())
  {
		fprintf(stderr, "Failed to initialize GLFW\n");
		return -1;
	}
  glfwWindowHint(GLFW_SAMPLES, 4);                // MultiSampling
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);  // Min OpenGL API: 2
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

  // Open a window and create its OpenGL context
	window = glfwCreateWindow(DEFAULT_W, DEFAULT_H, "Quadtree", NULL, NULL);
	if (window == NULL)
  {
		fprintf(stderr, "Failed to open GLFW window.\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

  // Initialize GLEW
  /*
	if (glewInit() != GLEW_OK)
  {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}
  */

  // Ensure we can capture the escape key as a "sticky key"
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

  // White background
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

  // Create and compile GLSL program from the shaders
	GLuint programID = LoadShaders("VertexShader.glsl",
                                 "FragmentShader.glsl");

  // Get a handle for our buffers
	GLuint vertexPosition_modelspaceID = glGetAttribLocation(programID, "vertexPosition_modelspace");

  // TODO: Replace with a geometry
	static const GLfloat g_vertex_buffer_data[] = {
		-1.0f, -1.0f, 0.0f,
		 1.0f, -1.0f, 0.0f,
		 0.0f,  1.0f, 0.0f,
	};

	GLuint vertexbuffer;
	glGenBuffers(1, &vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	do
  {
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT);

		// Use shader
		glUseProgram(programID);

		// 1rst attribute buffer: vertices
		glEnableVertexAttribArray(vertexPosition_modelspaceID);
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		glVertexAttribPointer(
			vertexPosition_modelspaceID, // The attribute we want to configure
			3,                           // size
			GL_FLOAT,                    // type
			GL_FALSE,                    // normalized?
			0,                           // stride
			(void*)0                     // array buffer offset
		);

		// Draw!
		glDrawArrays(GL_TRIANGLES, 0, 3); // 3 indices starting at 0 -> 1 triangle

		glDisableVertexAttribArray(vertexPosition_modelspaceID);

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while(glfwGetKey(window, GLFW_KEY_ESCAPE)!= GLFW_PRESS &&
		    glfwWindowShouldClose(window) == 0);

	// Cleanup VBO
	glDeleteBuffers(1, &vertexbuffer);
	glDeleteProgram(programID);

	// Close OpenGL window and terminate GLFW
	glfwTerminate();

  scene.setDimensions(DEFAULT_W, DEFAULT_H);

  return 0;
}

void Init()
{
  scene.Init();
}

void Display()
{
  scene.Display();
}

void KeyHandler()
{
  scene.KeyHandler();
}

void MouseButton()
{
  scene.MouseButton();
}

void MouseMotion()
{
  scene.MouseMotion();
}
