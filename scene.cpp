#include "scene.h"

/**
 * Reset this scene's window dimensions
 */
void Scene::setDimensions(unsigned int _w, unsigned int _h)
{
  window_width = _w;
  window_height = _h;
  window_aspect = (float) _w / (float) _h;
}

/**
 * TODO
 */
void drawGeometry(Geometry* geometry)
{
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
