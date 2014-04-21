/**
 * Some haphazard unit tests.
 */

#include "tests.h"
#include <stdio.h>

bool runAllTests()
{
  // Bounding Box
  testBBIntersection();

  // Polygons
  testOnLeftSide();
  testDynamicPolygon();
  testClipPolygon();

  // If we made it here, all tests passed!
  return true;
}

void testBBIntersection()
{
  BB b1(vec2(0,0), vec2(1,1));
  BB b2(vec2(0.5,0.5), vec2(1.5,1.5));
  BB& b1_r = b1;
  BB& b2_r = b2;
  assert(b1_r.intersects(b2_r));
  assert(b2_r.intersects(b1_r));

  b2_r._min = vec2(10, 10);
  b2_r._max = vec2(100, 100);
  assert(!b1.intersects(b2));

  // Box enclosing other box
  b2_r._min = vec2(-100, -200);
  b2_r._max = vec2(222, 333);
  assert(b1_r.intersects(b2_r));
  assert(b2_r.intersects(b1_r));

  // Edges touching
  b2_r._min = vec2(1, 1);
  b2_r._max = vec2(222, 333);
  assert(b1_r.intersects(b2_r));
}

void testOnLeftSide()
{
  vec2 a = vec2(0, 0);
  vec2 b = vec2(0, 1);
  vec2 p = vec2(-1, 0);
  assert(Polygon::onLeftSide(a, b, p) == 1);
  p = vec2(1, 1);
  assert(Polygon::onLeftSide(a, b, p) == -1);
  p = vec2(0, 10);
  assert(Polygon::onLeftSide(a, b, p) == 0);
}

void testDynamicPolygon()
{
  for (int i = 0; i < 3; ++i)
  {
    Polygon poly;
    poly._verts->push_back(vec2(3,2));
  }
  // TODO: How to test there is no memory leak of dynamically allocated vertex
  // list?
}

void testClipPolygon()
{
  // Unit square clipping box
  Polygon clip_box;
  clip_box._verts->push_back(vec2(0,0));
  clip_box._verts->push_back(vec2(1,0));
  clip_box._verts->push_back(vec2(1,1));
  clip_box._verts->push_back(vec2(0,1));
}
