#include <vector>
#include "glm/glm.hpp"
#include "qtnode.h"

using namespace std;
using namespace glm;

/*----------------------------------------------------------------------------*\
  POLYGON HELPER FUNCTIONS

  A polygon is defined by a set of points in counter-clockwise order. Edges are
  between adjacent points in this order. The following functions are helper
  functions to work with vertex lists as polygons.

  TODO: Consider whether its worth making a seperate class.
\*----------------------------------------------------------------------------*/

int  onLeftSide(vec2 a, vec2 b, vec2 p);
void clipPolygon(vector<vec2>& poly, const QTNode& quad);
void clipPolygonOneSide(vector<vec2>& poly, vec2 h);
