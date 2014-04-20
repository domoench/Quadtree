#include "tests.h"
#include <stdio.h>

/**
 * Some haphazard unit tests.
 */
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
  assert(onLeftSide(a, b, p) == 1);
  p = vec2(1, 1);
  assert(onLeftSide(a, b, p) == -1);
  p = vec2(0, 10);
  assert(onLeftSide(a, b, p) == 0);
}

bool runAllTests()
{
  // Bounding Box
  testBBIntersection();

  // Polygons
  testOnLeftSide();

  // If we made it here, all tests passed!
  return true;
}
