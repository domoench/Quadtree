#include <stdio.h>
#include <stdlib.h>
#include "constants.h"
#include "scene.h"
#include "shader.hpp"
#include "tests.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <GLFW/glfw3.h> // Leads to inclusion of gl.h
#include <OpenGL/gl3.h> // For mac

// Globals
GLFWwindow* window;
Scene       scene;

// Local Function Declarations
int  init();
void cleanUp();
void display();
void keyHandler();
void mouseButton();
void mouseMotion();

using namespace glm;

int main(int argc, char** argv)
{
  // Initialize Scene
  if (init() == -1) return -1;

  // Run Tests
  if (DEBUG) runAllTests();

  // TEST: Triangle
  vector<vec2>* tri_verts = new vector<vec2>;
  tri_verts->push_back(vec2(-0.4f, -1.0f));
  tri_verts->push_back(vec2(1.0f, -1.0f));
  tri_verts->push_back(vec2(0.0f, 1.0f));
  vector<int> edges;
  Geometry* triangle = new Geometry(0, tri_verts, &edges); // TODO: Where to free this?
  scene.addGeometry(triangle);

  vector<vec2>* sq_verts = new vector<vec2>;
  sq_verts->push_back(vec2(2.0f, 2.0f));
  sq_verts->push_back(vec2(3.0f, 2.0f));
  sq_verts->push_back(vec2(3.0f, 3.0f));
  sq_verts->push_back(vec2(2.0f, 3.0f));
  Geometry* square = new Geometry(1, sq_verts, &edges); // TODO: Where to free this?
  scene.addGeometry(square);

	do
  {
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT);

		// Draw all scene geometries
    for (const Geometry* g : scene._all_geometries)
    {
      scene.drawGeometry(g);
    }

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

  // Set viewport to be the entire size of the window
  glViewport(0, 0, DEFAULT_W, DEFAULT_H);

  // Ensure we can capture the escape key as a "sticky key"
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

  // White background
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

  // Create and compile GLSL program from the shaders
	scene._prog_ID = LoadShaders("../shaders/VertexShader.glsl",
                               "../shaders/FragmentShader.glsl");

  // Make shaders current
  glUseProgram(scene._prog_ID);

  // Set up Model-View-Projection.
  // As far as Shaders know, we work with 3D homogeneous coords => 4D Matrices
  mat4 Projection = perspective(
    60.0f,                // Field of View
    scene._window_aspect, // Apsect Ratio
    0.1f, 100.0f          // Display range down Z axis: 0.1 unit <-> 100 units
  );

  // Camera matrix
	mat4 View = lookAt(
    vec3(0,0,6), // Camera is at (4,3,3), in World Space
    vec3(0,0,0), // Looks at the origin
    vec3(0,1,0)  // Head is up
	);

  // Model Matrix is Identity matrix: models are what and where they are
  mat4 Model = mat4(1.0f);
  mat4 MVP = Projection * View * Model;

  // Pass to the GPU
  GLuint MatrixID = glGetUniformLocation(scene._prog_ID, "MVP");
  glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

  scene.init();

  return 0;
}

/**
 * Deletes the GLSL shaders and buffers from the GPU.
 */
void cleanUp()
{
	glDeleteProgram(scene._prog_ID);
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
