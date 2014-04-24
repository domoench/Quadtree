#include "scene.h"

/**
 * Reset this scene's window dimensions
 */
void Scene::setDimensions(unsigned int w, unsigned int h)
{
  _window_width = w;
  _window_height = h;
  _window_aspect = (float) w / (float) h;
}

/**
 * Add a reference to the given Geometry object to the scene.
 */
void Scene::addGeometry(const Geometry& g)
{
  _all_geometries.push_back(&g);
}

/**
 * Render the referenced Geometry object.
 */
void Scene::drawGeometry(const Geometry& g)
{
  // Make default shaders current
  glUseProgram(_prog_ID);

  // Pass default MVP to the GPU
  mat4 MVP = _proj * _view * _model;
  GLuint MVP_loc = glGetUniformLocation(_prog_ID, "MVP");
  glUniformMatrix4fv(MVP_loc, 1, GL_FALSE, &MVP[0][0]);

  // Geometries are red
  GLuint color_loc = glGetUniformLocation(_prog_ID, "color");
  glUniform4f(color_loc, 1.0f, 0.0f, 0.0f, 1.0f);

  // Make this geometry's VAO current
  glBindVertexArray(g._vao);
  glDrawArrays(GL_LINE_LOOP, 0, g._vertices->size());
}

/**
 * TODO
 */
void Scene::init()
{
}

/**
 * TODO
 */
void Scene::display()
{
}

/**
 * TODO
 */
void Scene::keyHandler()
{
}

/**
 * TODO
 */
void Scene::mouseButton()
{
}

/**
 * TODO
 */
void Scene::mouseMotion()
{
}
