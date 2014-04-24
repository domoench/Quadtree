#include "polygon.h"

/**
 * Construct a polygon object, dynamically allocating its vertices
 */
Polygon::Polygon()
{
  // printf("Polygon created\n");
  _verts = new vector<vec2>();
}

/**
 * Construct a polygon object, dynamically allocating its vertices from
 * the given vertex list
 */
Polygon::Polygon(const vector<vec2>& vertices)
{
  // printf("Polygon created\n");
  _verts = new vector<vec2>();

  for(vec2 v : vertices)
  {
    _verts->push_back(v);
  }
}

/**
 * Destroy a polygon object, dynamically de-allocating its vertices
 */
Polygon::~Polygon()
{
  // printf("Polygon deconstructed\n");
  delete _verts;
}

/**
 * Add a vertex to the end of this polygon's vertex list
 */
void Polygon::add(vec2 vert)
{
  _verts->push_back(vert);
}

/**
 * Determine if two lines intersect. If they do, returns point of
 * intersection. TODO: Otherwise what?
 *
 * Arguments define the following 2 parametric line eqs:
 * L1 = a1 + s * u    (u = a2 - a1)
 * L1 = b1 + t * v    (v = b2 - a1)
 *
 * www.ahinson.com/algorithms_general/Sections/Geometry/ParametricLineIntersection.pdf
 */
vec2 Polygon::lineIntersect(vec2 a1, vec2 a2, vec2 b1, vec2 b2)
{
  vec2 u   = a2 - a1; // L1 direction vector
  vec2 v   = b2 - b1; // L2 direction vector
  vec2 u_n = normalize(u);
  vec2 v_n = normalize(v);
  assert(u_n != v_n); // Uh oh, parallel lines got in here!

  // Calculate s parameter of intersection point
  float s = ((b2[0]-b1[0])*(b1[1]-a1[1]) - (b1[0]-a1[0])*(b2[1]-b1[1])) /
            ((b2[0]-b1[0])*(a2[1]-a1[1]) - (a2[0]-a1[0])*(b2[1]-b1[1]));

  // Calculate intersection point p, along L1 for example
  vec2 sxu = vec2(s * u[0], s * u[1]); // s * u
  vec2 p = a1 + sxu;

  return p;
}

/**
 * Determine if point p lies in the left half of the halfplanes separated by
 * the axis-aligned vector (b - a).
 *
 * Returns:
 *  1  if left
 *  -1 if right
 *  0  if colinear
 */
int Polygon::onLeftSide(vec2 a, vec2 b, vec2 p)
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
 * Mutate this polygon's vertex list so that it is clipped with respect to the
 * clipping box. This is an implementation of the Sutherland-Hodgman
 * algorithm.
 */
void Polygon::clip(const Polygon& clip_box)
{
  // printf("Entering clip()\n");
  const vector<vec2>& clip_verts = *(clip_box._verts);

  // 4 clip edges
  for (int i = 0; i < 4; ++i)
  {
    clipOneSide(clip_verts[i], clip_verts[(i+1)%4]);
  }

  //printf("Leaving clip()\n");
}

/**
 * Mutate the given polygon so that it is clipped with respect to the line
 * defined by a->b which divides the plane into "left" and "right" half spaces
 * (where "up" would be the direction of vector a->b).
 */
void Polygon::clipOneSide(vec2 a, vec2 b)
{
  // printf("Entering clipOneSide()\n");

  // Can't clip nothing
  if (_verts->empty()) return;

  vector<vec2>* new_verts = new vector<vec2>; // Output Poly Vertices

  // Walk through
  vec2 prev = _verts->back();
  for (vec2 curr : *_verts)
  {
    // printf("Inspecting (%f,%f)\n", curr[0], curr[1]);
    int curr_test = onLeftSide(a, b, curr);
    int prev_test = onLeftSide(a, b, prev);
    bool curr_in = curr_test == 1 || curr_test == 0;
    bool prev_in = prev_test == 1 || prev_test == 0;
    // printf("\tcurr_test: %d, curr_in: %d\n", curr_test, curr_in);
    // printf("\tprev_test: %d, prev_in: %d\n", prev_test, prev_in);
    if (curr_in) // curr in
    {
      if (!prev_in) // prev out
      {
        if (curr_test != 0)
        {
          // Calculate and output intersection
          vec2 i = lineIntersect(a, b, prev, curr);
          // printf("\tCurr in, Prev out Intersection: (%f,%f)\n", i[0], i[1]);
          new_verts->push_back(i);
          // printf("\tOutput (%f,%f)\n", i[0], i[1]);
        }
      }
      new_verts->push_back(curr);
      // printf("\tOutput (%f,%f)\n", curr[0], curr[1]);
    }
    else if (prev_in) // curr out, prev in
    {
      // If prev lay on the clipping edge, its already been included.
      if (prev_test != 0)
      {
        vec2 i = lineIntersect(a, b, prev, curr);
        // printf("\tCurr out, prev in Intersection: (%f,%f)\n", i[0], i[1]);
        new_verts->push_back(i);
        // printf("\tOutput (%f,%f)\n", i[0], i[1]);
      }
    }
    // Else curr out and prev out => Ignore
    prev = curr;
  }

  // Deallocate the old vertex vector and replace it with the new
  delete _verts;
  _verts = new_verts;

  // printf("Leaving clipOneSide()\n");
}

/**
 * Return the area of this simple polygon. Follows technique described in
 * Graphic Gems II, pg 7.
 */
float Polygon::area()
{
  const vector<vec2>& verts = *_verts;
  int n = verts.size();
  if (n < 3) return 0.0f;
  //printf("n = %d\n", n);

  // Calculate product sum term
  float prod_sum  = 0.0f;
  for (int i = 0; i < n-1; ++i)
  {
    //printf("Sum Term: %f * %f = %f\n", verts[i][0], verts[i+1][1], (verts[i][0] * verts[i+1][1]));
    prod_sum += (verts[i][0] * verts[i+1][1]);
  }
  prod_sum += (verts[n-1][0] * verts[0][1]); // Wrap back to first

  // Calculate product difference term
  float prod_diff = 0.0f;
  for (int i = 0; i < n-1; ++i)
  {
    prod_diff -= (verts[i][1] * verts[i+1][0]);
    //printf("Diff Term = %f\n",(verts[i][1] * verts[i+1][0]));
  }
  prod_diff -= (verts[n-1][1] * verts[0][0]);

  return 0.5 * (prod_sum + prod_diff);
}
