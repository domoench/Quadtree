#ifndef __DOM_GEOMETRY_H__
#define __DOM_GEOMETRY_H__

#include <vector>
#include "constants.h"
#include "bb.h"
#include "glm/glm.hpp"

using namespace std;
using namespace glm;

/**
 * A class representing convex hull geometries.
 */
class Geometry
{
  public:
    unsigned int id;
    vector<vec2> vertices;
    vector<int>  edges;
    BB*          bb;

    Geometry(int _id, vector<vec2> _vertices, vector<int> _edges);
};
#endif
