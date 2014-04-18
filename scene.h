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
    unsigned int window_width;
    unsigned int window_height;
    float        window_aspect;
    GLuint       vao, vbo, prog_ID, vert_pos_loc; //! OpenGL-GLSL state

    //!< A list of all the Geometry objects contained in the scene
    vector<Geometry> all_geometries;
    //!< A bounding box for all the Geometries in the scene. Serves
    //!< as the root level octree
    BB top_bb;

    void setDimensions(unsigned int _w, unsigned int _h);
    void drawGeometry(Geometry* geometry);

    void init();
    void display();
    void keyHandler();
    void mouseButton();
    void mouseMotion();
};
#endif
