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
void Scene::Init()
{
}

/**
 * TODO
 */
void Scene::Display()
{
}

/**
 * TODO
 */
void Scene::KeyHandler()
{
}

/**
 * TODO
 */
void Scene::MouseButton()
{
}

/**
 * TODO
 */
void Scene::MouseMotion()
{
}
