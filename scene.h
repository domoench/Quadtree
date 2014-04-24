#ifndef __DOM_SCENE_H__
#define __DOM_SCENE_H__

#include <vector>
#include "constants.h"
#include "bb.h"
#include "geometry.h"
#include "qtnode.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <GLFW/glfw3.h> // Leads to inclusion of gl.h
#include <OpenGL/gl3.h> // For mac

using namespace std;

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

    //<! OpenGL shader-pair program ID
    GLuint _prog_ID;

    //<! Default model, view, and projection matrices
    mat4 _proj;
    mat4 _model;
    mat4 _view;

    //!< A list of all the Geometry objects contained in the scene
    vector<const Geometry*> _all_geometries;

    //!< The quadtree node representing all geometries in the scene
    // QTNode quadtree;

    void setDimensions(unsigned int w, unsigned int h);
    void addGeometry(const Geometry& geometry);

    void init();
    void display();
    void keyHandler();
    void mouseButton();
    void mouseMotion();
};
#endif
