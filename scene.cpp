#include "scene.h"

Scene::Scene()
{
  _window_width = DEFAULT_W;
  _window_height = DEFAULT_H;
  _window_aspect = (GLfloat) DEFAULT_W / (GLfloat) DEFAULT_H;

  _input_poly = new vector<vec2>();
}

Scene::~Scene()
{
  delete _input_poly;
  _input_poly = NULL;
}

/**
 * Reset this scene's window dimensions
 */
void Scene::setDimensions(unsigned int w, unsigned int h)
{
  _window_width = w;
  _window_height = h;
  _window_aspect = (GLfloat) w / (GLfloat) h;
}

/**
 * Add a reference to the given Geometry object to the scene.
 */
void Scene::addGeometry(const Geometry& g)
{
  _all_geometries.push_back(&g);
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
 * Add a vertex at position (x,y) to this scene's _input_poly buffer
 */
void Scene::addUserVertex(double x, double y)
{
  /**
   *  Screen Input Coordinate to World Coordinate transformation function
   *  f(<x,y>) = <x - Window Width/2, Window Height/2 - y>
   *  TODO: Figure out formula that can handle window resizing
   */
  printf("Click: (%f, %f) -> (%f, %f)\n", x, y, x - DEFAULT_W/2, DEFAULT_H/2 - y);
  _input_poly->push_back(vec2(x - DEFAULT_W/2, DEFAULT_H/2 - y));
}

/**
 * Allocate and insert a Geometry into the scene based on the current contents
 * of the _input_poly vertex buffer.
 */
bool Scene::insertUserGeometry()
{
  assert(!_input_poly->empty()); // TODO: What's best way to handle?

  // Allocate Geometry instance
  Geometry* user_geom = new Geometry(_all_geometries.size(), *_input_poly);
  // TODO: Delete these somewhere in cleanup

  // Add to scene + Insert to quadtree
  bool insert_success = _qt.insert(*user_geom);
  if (insert_success)
  {
    addGeometry(*user_geom);
    printf("Geometry %d with %lu vertices inserted.\n", user_geom->_id, user_geom->_poly._verts->size());
  }
  else
  {
    printf("Geometry insertion failed\n");
  }

  // Clear _input_poly
  _input_poly->clear();

  return insert_success;
}
