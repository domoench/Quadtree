#include "bb.h"
#include <limits>

using namespace std;

BB::BB() : _min(vec2(0.0f, 0.0f)), _max(vec2(0.0f, 0.0f))
{}

/**
 * Create a bounding box that contains the given vertex set.
 */
BB::BB(const vector<vec2>& vertices)
{
  float min_x, min_y;
  min_x = min_y = numeric_limits<int>::max();
  float max_x, max_y;
  max_x = max_y = numeric_limits<int>::min();
  for (vec2 v : vertices)
  {
    if (v[0] < min_x) min_x = v[0];
    else if (v[0] > max_x) max_x = v[0];

    if (v[1] < min_y) min_y = v[1];
    else if (v[1] > max_y) max_y = v[1];
  }

  _min[0] = min_x;
  _min[1] = min_y;
  _max[0] = max_x;
  _max[1] = max_y;
}

/**
 * Create a bounding box with the given corners.
 */
BB::BB(vec2 min, vec2 max) : _min(min), _max(max)
{}

/**
 * Use SAT-style intersection with X and Y axes.
 */
bool BB::intersects(const BB& other)
{
  // Check X axis projection
  bool x_intersect = (this->_min[0] <= other._max[0]) &&
                     (this->_max[0] >= other._min[0]);
  // Check Y axis projection
  bool y_intersect = (this->_min[1] <= other._max[1]) &&
                     (this->_max[1] >= other._min[1]);

  // If both axes projections intersect, then BBs intersect
  return x_intersect && y_intersect;
}
