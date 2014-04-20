#include "polygon.h"
/**
 * Determine if two line segments intersect. If they do, returns point of
 * intersection. TODO: Otherwise what?
 */
/*
vec2 intersectLineSegments(vec2 a_1, vec2 a_2, vec2 b_1, vec2 b_2)
{
  // TODO: https://stackoverflow.com/questions/563198/how-do-you-detect-where-two-line-segments-intersect
}
*/

/**
 * Determine if point p lies in the left half of the halfplanes separated by
 * the axis-aligned vector (b - a).
 *
 * Returns:
 *  1  if left
 *  -1 if right
 *  0  if colinear
 */
int onLeftSide(vec2 a, vec2 b, vec2 p)
{
  vec2 axis = b - a;
  //printf("a: <%f, %f>\n", a[0], a[1]);
  //printf("b: <%f, %f>\n", b[0], b[1]);
  //printf("p: <%f, %f>\n", p[0], p[1]);
  vec2 v = p - b;
  //printf("axis: <%f, %f>\n", axis[0], axis[1]);
  //printf("v: <%f, %f>\n", v[0], v[1]);
  // 2D cross product
  float x_prod = axis[0] * v[1] - axis[1] * v[0];
  //printf("cross: %f\n", x_prod);
  if (x_prod < 0) return -1;
  else return (x_prod > 0);
}

/**
 * Mutate the given polygon so that it is clipped with respect to the
 * clipping polygon defined by the given quadtree node.
 */
void clipPolygon(vector<vec2>& poly, const QTNode& quad)
{
  // TODO
}

/**
 * Mutate the given polygon so that it is clipped with respect to the
 * given vector h which divides the plane into "left" and "right" half
 * spaces (where "up" would be the direction of vector h).
 */
void clipPolygonOneSide(vector<vec2>& poly, vec2 h)
{
  // TODO
}
