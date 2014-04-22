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
  testPolygonArea();
  testLineIntersection();
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

void testPolygonArea()
{
  Polygon square;
  square._verts->push_back(vec2(0,0));
  square._verts->push_back(vec2(1,0));
  square._verts->push_back(vec2(1,1));
  square._verts->push_back(vec2(0,1));
  assert(square.area() == 1.0);

  Polygon triangle;
  triangle._verts->push_back(vec2(0,0));
  triangle._verts->push_back(vec2(100,0));
  triangle._verts->push_back(vec2(100,100));
  //printf("Area of triangle: %f\n", triangle.area());
  assert(triangle.area() == 5000.0);
}

void testClipPolygon()
{
  // Unit square clipping box
  Polygon clip_box;
  clip_box._verts->push_back(vec2(0,0));
  clip_box._verts->push_back(vec2(1,0));
  clip_box._verts->push_back(vec2(1,1));
  clip_box._verts->push_back(vec2(0,1));
  assert(clip_box._verts->size() == 4);

  Polygon square;
  square._verts->push_back(vec2(0.5,0.5));
  square._verts->push_back(vec2(1.5,0.5));
  square._verts->push_back(vec2(1.5,1.5));
  square._verts->push_back(vec2(0.5,1.5));
  assert(square._verts->size() == 4);

  // TODO
  assert(false);
}

void testLineIntersection()
{
  vec2 a1 = vec2(0, 0);
  vec2 a2 = vec2(10, 10);
  vec2 b1 = vec2(10, 0);
  vec2 b2 = vec2(0, 10);

  // Quick tests of how vec2 type works
  vec2 test_a1 = vec2(0,0);
  assert(test_a1 == a1); // vec2 comparison can work like this
  // assert(2 * a2 == vec2(20, 20)); // Can't do this!
  assert(vec2(2 * a2[0], 2 * a2[1]) == vec2(20, 20));
  vec2 a2_n = normalize(a2); // Normalize generates a new vector
  assert(a2 == vec2(10,10)); // Leaving original unmodified

  // Test simple diagonal intersection
  vec2 p = Polygon::intersectLineSegments(a1, a2, b1, b2);
  assert(p == vec2(5,5));

  // Can't pass in parallel lines
  p = Polygon::intersectLineSegments(a1, a2, a1, a2);

  // Test lines defined by non-overlapping segment arguments
  a1 = vec2(0, 0);
  a2 = vec2(0, 10);
  b1 = vec2(1, 1);
  b2 = vec2(10, 10);
  p  = Polygon::intersectLineSegments(a1, a2, b1, b2);
  assert(p == vec2(0,0));
}
