/**
 * Some haphazard unit tests.
 */

#include "tests.h"
#include <stdio.h>

// TODO: Is this adequate precision?
#define PRECISION_EPSILON 0.000001

bool runAllTests()
{
  // Bounding Box
  testBBIntersection();

  // Polygons
  testOnLeftSide();
  testPolygon();
  testPolygonArea();
  testLineIntersection();
  testClipPolygon();
  testClipOneSide();

  // QTNode
  testBasicQTNode();
  testQTNodeIntersect();
  testQTNodeInsert();

  // If we made it here, all tests passed!
  assert(false);
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

void testPolygon()
{
  for (int i = 0; i < 3; ++i)
  {
    Polygon poly;
    poly.add(vec2(3,2));
  }
  // TODO: How to test there is no memory leak of dynamically allocated vertex
  // list?

  // Test constructing from a vertex vector
  vector<vec2> test_verts;
  test_verts.push_back(vec2(0,0));
  test_verts.push_back(vec2(1,0));
  test_verts.push_back(vec2(2,0));
  test_verts.push_back(vec2(3,0));
  test_verts.push_back(vec2(4,4));
  test_verts.push_back(vec2(3,4));
  test_verts.push_back(vec2(-1,-1));
  Polygon p(test_verts);
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
  printf("testClipPolygon(): \n");

  // Unit square clipping box
  Polygon clip_box;
  clip_box.add(vec2(0,0));
  clip_box.add(vec2(1,0));
  clip_box.add(vec2(1,1));
  clip_box.add(vec2(0,1));
  assert(clip_box._verts->size() == 4);

  Polygon square;
  square.add(vec2(0.5,0.5));
  square.add(vec2(1.5,0.5));
  square.add(vec2(1.5,1.5));
  square.add(vec2(0.5,1.5));
  assert(square._verts->size() == 4);

  square.clip(clip_box);
  assert(square.area() == 0.25);

  // Cookie cut a square out of a big triangle
  Polygon big_tri;
  big_tri.add(vec2(-100, -100));
  big_tri.add(vec2(100, -100));
  big_tri.add(vec2(15.632, 100));
  big_tri.clip(clip_box);
  assert(big_tri.area() == 1);

  printf("testClipPolygon() Passed.\n");
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
  vec2 p = Polygon::lineIntersect(a1, a2, b1, b2);
  assert(p == vec2(5,5));

  // Can't pass in parallel lines
  // p = Polygon::lineIntersect(a1, a2, a1, a2);

  // Test lines defined by non-overlapping segment arguments
  a1 = vec2(0, 0);
  a2 = vec2(0, 10);
  b1 = vec2(1, 1);
  b2 = vec2(10, 10);
  p  = Polygon::lineIntersect(a1, a2, b1, b2);
  assert(p == vec2(0,0));
}

void testClipOneSide()
{
  Polygon square;
  square.add(vec2(-1,-1));
  square.add(vec2(1,-1));
  square.add(vec2(1,1));
  square.add(vec2(-1,1));
  // printf("Square area: %f\n", square.area());
  assert(square.area() == 4);

  Polygon diamond;
  diamond.add(vec2(-1,0));
  diamond.add(vec2(0,-1));
  diamond.add(vec2(1,0));
  diamond.add(vec2(0,1));
  // printf("Diamond area: %f\n", diamond.area());
  // printf("Diamond n: %d\n", diamond._verts->size());
  assert(diamond.area() == 2);
  assert(diamond._verts->size() == 4);
  diamond.clipOneSide(vec2(0,0), vec2(0,1)); // Clip along y axis
  // printf("Diamond area: %f\n", diamond.area());
  // printf("Diamond n: %d\n", diamond._verts->size());
  assert(diamond.area() == 1);
  assert(diamond._verts->size() == 3);

  // Test Clip edge colinear with polygon edge
  diamond.clipOneSide(vec2(0,0), vec2(0,1)); // Clip along y axis again
  assert(diamond.area() == 1);
  assert(diamond._verts->size() == 3);

  diamond.clipOneSide(vec2(0,0), vec2(1,0));
  assert(diamond.area() == 0.5);
  assert(diamond._verts->size() == 3);

  // Missed cuts
  diamond.clipOneSide(vec2(0,0), vec2(1,1));
  assert(diamond.area() == 0.5);
  assert(diamond._verts->size() == 3);
  diamond.clipOneSide(vec2(0,-1), vec2(1,-1));
  assert(diamond.area() == 0.5);
  assert(diamond._verts->size() == 3);

  // Cut everything out
  // printf("diamond area: %f\n", diamond.area());
  diamond.clipOneSide(vec2(1,1), vec2(0,-10));
  assert(diamond.area() == 0);
  assert(diamond._verts->size() == 0);

  // Can you do this?
  /*
  int* x_p = new int;
  *x_p = 5;
  int& x_r = *x_p;
  delete x_p;
  */

  printf("testClipOneSide() Passed.\n");
}

void testBasicQTNode()
{
  vec2 base = vec2(0 - DEFAULT_W/2, 0 - DEFAULT_H/2);
  QTNode qt(0, base, NULL, NULL);
  assert(qt.size() == 1);
  qt.subdivide();
  assert(qt._children[0]->_base == vec2(base[0] + DEFAULT_W/2, base[1] + DEFAULT_W/2));
  assert(qt._children[1]->_base == vec2(base[0], base[1] + DEFAULT_W/2));
  assert(qt._children[2]->_base == vec2(base[0], base[1]));
  assert(qt._children[3]->_base == vec2(base[0] + DEFAULT_W/2, base[1]));
  assert(qt.size() == 5);
  qt._children[1]->subdivide();
  assert(qt._children[1]->_children[2]->_base == vec2(base[0], base[1] + DEFAULT_W/2));
  assert(qt.size() == 9);

  assert(qt._level == 0);
  assert(qt._children[0]->_level == 1);
  assert(qt._children[0]->_children[0] == NULL);
  assert(qt._children[1]->_children[0]->_level == 2);

  qt.clear();
  assert (qt.isLeaf());

  printf("testBasicQTNode() Passed.\n");
}

void testQTNodeIntersect()
{
  vector<vec2> tri_verts;
  tri_verts.push_back(vec2(0.0f, 0.0f));
  tri_verts.push_back(vec2(100.0f, 0.0f));
  tri_verts.push_back(vec2(0.0f, 100.0f));
  vector<int> edges;
  Geometry tri = Geometry(0, tri_verts, edges);

  vec2 base = vec2(0 - DEFAULT_W/2, 0 - DEFAULT_H/2);
  QTNode qt(0, base, NULL, NULL);

  float ratio = qt.intersects(tri);
  assert(fabs(ratio - 0.00617328) <= PRECISION_EPSILON);
  printf("%f\n", ratio);
}

void testQTNodeInsert()
{
  printf("Entering testQTNodeInsert()\n");
  // Triangle Geometry
  vector<vec2> tri_verts;
  tri_verts.push_back(vec2(0.0f, 0.0f));
  tri_verts.push_back(vec2(100.0f, 0.0f));
  tri_verts.push_back(vec2(0.0f, 100.0f));
  vector<int> edges;
  Geometry triangle = Geometry(0, tri_verts, edges);

  // Quadtree Root
  vec2 base = vec2(0 - DEFAULT_W/2, 0 - DEFAULT_H/2);
  QTNode qt(0, base, NULL, NULL);

  bool inserted = qt.insert(triangle);
  assert(inserted);
  qt.clear();

  // Insert all the squares of a grid
  printf("Testing Grid Cell Insertion\n");
  int n = 10;
  int w = DEFAULT_W / n;
  for (int i = 0; i < n; ++i)
  {
    for (int j = 0; j < n; ++j)
    {
      vector<vec2> grid_unit_verts;
      grid_unit_verts.push_back(qt._base + vec2(i * w, j * w));
      grid_unit_verts.push_back(qt._base + vec2(i * w + w * .99, j * w));
      grid_unit_verts.push_back(qt._base + vec2(i * w + w * .99, j * w + w * .99));
      grid_unit_verts.push_back(qt._base + vec2(i * w, j * w + w * .99));
      Geometry grid_unit = Geometry(i*n+j, grid_unit_verts, edges);

      assert(qt.insert(grid_unit));
    }
  }

}
