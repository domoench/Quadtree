#ifndef __DOM_SCENE_H__
#define __DOM_SCENE_H__

#include <vector>
#include "constants.h"
#include "bb.h"
#include "geometry.h"
#include "glm/glm.hpp"
#include <GLFW/glfw3.h> // Leads to inclusion of gl.h
#include <OpenGL/gl3.h> // For mac

using namespace std;
using namespace glm;

/**
 * A class to package information about a scene to be rendered, including the
 * scene's geometries, associated quadtree, and window dimensions.
 */
class Scene
{
  public:
    unsigned int _window_width;
    unsigned int _window_height;
    float        _window_aspect;

    //<! OpenGL Shaders ID
    GLuint       _prog_ID;

    //!< A list of all the Geometry objects contained in the scene
    vector<const Geometry*> _all_geometries;
    //!< A bounding box for all the Geometries in the scene. Serves
    //!< as the root level octree
    BB _top_bb;

    void setDimensions(unsigned int w, unsigned int h);
    void addGeometry(const Geometry* geometry);
    void drawGeometry(const Geometry* geometry);

    void init();
    void display();
    void keyHandler();
    void mouseButton();
    void mouseMotion();
};
#endif
