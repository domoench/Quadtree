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
Scene scene;

// Local Function Declarations
int  init();
void cleanUp();

// GLFW Callbacks
static void mouseClick(GLFWwindow* window, int button, int action, int mods)
{
  if (action == GLFW_PRESS)
  {
    double x, y;
    glfwGetCursorPos(window, &x, &y);
    scene.addUserVertex(x, y);
  }
}

static void keyPress(GLFWwindow* window, int key, int scancode, int action,
                     int mods)
{
  if (key == GLFW_KEY_ENTER && action == GLFW_PRESS)
  {
    scene.insertUserGeometry();
  }
}

using namespace glm;

int main(int argc, char** argv)
{
  // Initialize Scene
  if (init() == -1) return -1;

  // Run Tests
  if (DEBUG) runAllTests();

  // TEST: Empty Subdivided Quadtree
  vec2 base = vec2(0 - DEFAULT_W/2, 0 - DEFAULT_H/2);
  QTNode qt(0, base, NULL, NULL);

  // TEST: Square
  vector<vec2> sq_verts;
  sq_verts.push_back(vec2(-100, -225));
  sq_verts.push_back(vec2(225 + 112, 225));
  sq_verts.push_back(vec2(225 + 112, 225 + 112));
  sq_verts.push_back(vec2(225, 225 + 112));
  Geometry square = Geometry(1, sq_verts);
  if (qt.insert(square)) scene.addGeometry(square);

  // TEST: Triangle
  vector<vec2> tri_verts;
  tri_verts.push_back(vec2(-449, -449));
  tri_verts.push_back(vec2( 449, -449));
  tri_verts.push_back(vec2(-449,  449));
  Geometry triangle = Geometry(0, tri_verts);
  if (qt.insert(triangle)) scene.addGeometry(triangle);

	while(!glfwWindowShouldClose(scene._glfw_window))
  {
		// Clear the screen
		glClear(GL_COLOR_BUFFER_BIT);

    // Draw the quadtree
    qt.draw();

		// Draw all scene geometries
    for (const Geometry* g : scene._all_geometries)
    {
      g->draw();
    }

		// Swap buffers
		glfwSwapBuffers(scene._glfw_window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed

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
	scene._glfw_window = glfwCreateWindow(DEFAULT_W, DEFAULT_H, "Quadtree", NULL, NULL);
	if (scene._glfw_window == NULL)
  {
		fprintf(stderr, "Failed to open GLFW window.\n");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(scene._glfw_window);

  // Set callbacks
  glfwSetMouseButtonCallback(scene._glfw_window, mouseClick);
  glfwSetKeyCallback(scene._glfw_window, keyPress);

  // Set viewport to be the entire size of the window
  glViewport(0, 0, DEFAULT_W, DEFAULT_H);

  // White background
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);

  // Create and compile GLSL program from the shaders
	scene._prog_ID = LoadShaders("../shaders/VertexShader.glsl",
                               "../shaders/FragmentShader.glsl");

  // Set up Model-View-Projection.
  // As far as Shaders know, we work with 3D homogeneous coords => 4D Matrices
  scene._proj = perspective(
    97.0f,                // Field of View
    scene._window_aspect, // Apsect Ratio
    0.1f, 500.0f          // Display range down Z axis
  );

  // Camera matrix
	scene._view = lookAt(
    vec3(0,0,400), // Camera position in world space
    vec3(0,0,0),   // Looks at the origin
    vec3(0,1,0)    // Head is up
	);

  // Model Matrix is Identity matrix: models are what and where they are
  scene._model = mat4(1.0f);

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
