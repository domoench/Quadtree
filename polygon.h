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
    vector<vec2>* _verts;

    Polygon();
    ~Polygon();

    void clipPolygon(Polygon& poly, const Polygon& clip_box);
    void clipPolygonOneSide(vector<vec2>& poly, vec2 h);

    // Helpers
    static int onLeftSide(vec2 a, vec2 b, vec2 p);
};
