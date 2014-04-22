#include <vector>
#include "glm/glm.hpp"
#include "qtnode.h"

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
    ~Polygon();

    void  clip(const Polygon& clip_box);
    void  clipOneSide(vec2 a, vec2 b);
    float area();

    // Static Helpers. TODO: Is there a better place for them?
    static int  onLeftSide(vec2 a, vec2 b, vec2 p);
    static vec2 intersectLineSegments(vec2 a1, vec2 a2, vec2 b1, vec2 b2);
};
