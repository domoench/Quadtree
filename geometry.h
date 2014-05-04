#ifndef __DOM_GEOMETRY_H__
#define __DOM_GEOMETRY_H__

#include <vector>
#include "constants.h"
#include "bb.h"
#include "polygon.h"
#include "glm/glm.hpp"
#include <GLFW/glfw3.h> // Leads to inclusion of gl.h
#include <OpenGL/gl3.h> // For mac

using namespace std;
using namespace glm;

/**
 * A class representing convex hull polygon and the OpenGL state needed to
 * render it.
 */
class Geometry
{
  public:
    unsigned int  _id;
    Polygon       _poly;
    BB            _bb;

    // OpenGL-GLSL State Members
    GLuint _vao, _vbo, _vert_pos_loc;

    Geometry(int id, const vector<vec2>& vertices);
    ~Geometry();
    float area() const;
    void  draw() const;
};

#endif
