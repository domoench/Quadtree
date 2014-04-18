#include <stdio.h>
#include <stdlib.h>
#include "constants.h"
#include "scene.h"

#include "shader.hpp"
#include "glm/glm.hpp"
#include <GLFW/glfw3.h> // Leads to inclusion of gl.h
#include <OpenGL/gl3.h> // For mac

using namespace glm;

GLFWwindow* window;
Scene scene;

// Local Function Declarations
int init();
void cleanUp();
void display();
void keyHandler();
void mouseButton();
void mouseMotion();

int main(int argc, char** argv)
{
  // Initialize Scene
  if (init() == -1) return -1;

	do
  {
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw!
		glDrawArrays(GL_TRIANGLES, 0, 3); // 3 indices starting at 0 -> 1 triangle

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while(glfwGetKey(window, GLFW_KEY_ESCAPE)!= GLFW_PRESS &&
		    glfwWindowShouldClose(window) == 0);

  // Clean Up
  cleanUp();
  return 0;
}

/**
 * Sets up GLFW and initializes GLSL shaders and buffers and saves them to the
 * scene.
 */
int init()
{
  scene.setDimensions(DEFAULT_W, DEFAULT_H);

  // Initialise GLFW
	if (!glfwInit())
  {
		fprintf(stderr, "Failed to initialize GLFW\n");
		return -1;
	}
  glfwWindowHint(GLFW_SAMPLES, 4);                // MultiSampling
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);  // Min OpenGL API: 3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  // https://stackoverflow.com/questions/22213874/creating-opengl-3-3-context-with-glfw-in-mac-os-x-10-9
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Open a window and create its OpenGL context
	window = glfwCreateWindow(DEFAULT_W, DEFAULT_H, "Quadtree", NULL, NULL);
	if (window == NULL)
  {
		fprintf(stderr, "Failed to open GLFW window.\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

  // Ensure we can capture the escape key as a "sticky key"
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

  // White background
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

  // Create and compile GLSL program from the shaders
	scene.prog_ID = LoadShaders("../shaders/VertexShader.glsl",
                              "../shaders/FragmentShader.glsl");

  // Create a test Geometry
  vector<vec2>* tri_verts = new vector<vec2>;
  tri_verts->push_back(vec2(-1.0f, -1.0f));
  tri_verts->push_back(vec2(1.0f, -1.0f));
  tri_verts->push_back(vec2(0.0f, 1.0f));
  vector<int> edges;
  Geometry* triangle = new Geometry(0, tri_verts, &edges);

  // Create Vertex Position VBO
	glGenBuffers(1, &scene.vbo);
  //Create buffer object of vertex attribute data
	glBindBuffer(GL_ARRAY_BUFFER, scene.vbo);
  // Allocate memory on the GPU and transfer the app data to it
  glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(vec2), triangle->vertices->data(), GL_STATIC_DRAW);

  // Create VAO to manage Vertex and Color VBOs
  glGenVertexArrays(1, &scene.vao); // Find unused name
  glBindVertexArray(scene.vao);     // Make the VAO object current
	scene.vert_pos_loc = glGetAttribLocation(scene.prog_ID, "vertex_pos");
	glEnableVertexAttribArray(scene.vert_pos_loc);
	glBindBuffer(GL_ARRAY_BUFFER, scene.vbo); // Bind our vertex VBO to current VAO
  glVertexAttribPointer(
    scene.vert_pos_loc,  // The attribute we want to configure
    2,            // Number of elems per vertex for this attribute
    GL_FLOAT,     // type
    GL_FALSE,     // normalized?
    0,            // stride
    (void*)0      // array buffer offset
  );

  // Make shaders current
  glUseProgram(scene.prog_ID);

  scene.init();

  return 0;
}

/**
 * Deletes the GLSL shaders and buffers from the GPU.
 */
void cleanUp()
{
	glDisableVertexAttribArray(scene.vert_pos_loc);
	glDeleteBuffers(1, &scene.vbo);
	glDeleteProgram(scene.prog_ID);
	// Close OpenGL window and terminate GLFW
	glfwTerminate();
}

void display()
{
  scene.display();
}

void keyHandler()
{
  scene.keyHandler();
}

void mouseButton()
{
  scene.mouseButton();
}

void mouseMotion()
{
  scene.mouseMotion();
}
