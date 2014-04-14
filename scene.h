#ifndef __DOM_SCENE_H__
#define __DOM_SCENE_H__

#include <vector>
#include "constants.h"
#include "bb.h"
#include "geometry.h"
#include "glm/glm.hpp"

using namespace std;
using namespace glm;

/**
 * A class to package information about a scene to be rendered, including the
 * scene's geometries, associated quadtree, and window dimensions.
 */
class Scene {
  public:
    unsigned int window_width;
    unsigned int window_height;
    float        window_aspect;

    //!< A list of all the Geometry objects contained in the scene
    vector<Geometry> all_geometries;
    //!< A bounding box for all the Geometries in the scene. Serves
    //!< as the root level octree
    BB top_bb;

    void setDimensions(unsigned int _w, unsigned int _h);
    void Init();
    void Display();
    void KeyHandler();
    void MouseButton();
    void MouseMotion();
};
#endif
