#ifndef __DOM_POLYGON_H__
#define __DOM_POLYGON_H__

#include <vector>
#include <OpenGL/gl3.h> // For mac
#include "glm/glm.hpp"

using namespace std;
using namespace glm;

/*----------------------------------------------------------------------------*\
  POLYGON

  A polygon is defined by a set of points in counter-clockwise order. Edges are
  between adjacent points in this order.

  This class helps to dynamically generate and work with vertex vectors
  representing polygons.
\*----------------------------------------------------------------------------*/

class Polygon
{
  public:
    /*!
      Vertices must be ordered counter-clockwise. First vertex is NOT repeated
      at the end of the list.
    */
    vector<vec2>* _verts;

    Polygon();
    Polygon(const vector<vec2>& vertices);
    Polygon(const Polygon& other);
    ~Polygon();
    Polygon& operator=(const Polygon& other);

    void    add(vec2 vert);
    void    clip(const Polygon& clip_box);
    void    clipOneSide(vec2 a, vec2 b);
    GLfloat area() const;

    // Static Helpers. TODO: Is there a better place for them?
    static int  onLeftSide(vec2 a, vec2 b, vec2 p);
    static vec2 lineIntersect(vec2 a1, vec2 a2, vec2 b1, vec2 b2);
};

#endif
