#include "qtnode.h"
#include "scene.h"

extern Scene scene;

/**
 * Construct a leaf quadtree node. Consists of a single node with no children.
 */
QTNode::QTNode(unsigned int level, vec2 base, QTNode* parent,
               const Geometry* occupier) :
  _level(level), _base(base), _parent(parent), _occupier(occupier)
{
  for (int i = 0; i < 4; i++) {
    _children[i] = NULL;
  }
  _occupied = occupier != NULL;
}

/**
 * Check if this node intersects the given bounding box.
 */
bool QTNode::intersects(const BB& box)
{
  // TODO: SAT intersection test
  return false;
}

/**
 * Check if this node intersects the given Geometry polygon.
 * TODO: Return the area of intersection.
 */
bool QTNode::intersects(const Geometry& geom)
{
  return false;
}

/**
 * Recursively insert the given Geometry into this quadtree node.
 */
void QTNode::insert(const Geometry& geom)
{
  /*
  Pre: geom intersects this node
  ratio := geom.area() / QTNode.area()
  if ratio < threshold:
    this.subdivide()
    Determine which children geom intersects
    for each candidate in candidates:
      candidate.insert(geom);

  Calculate which of the 4 children it intersects => candidates

  Case II: geom doesn't intersect this node
  */
}

/*
 * Subdivide this quadtree node by creating 4 unoccupied children.
 */
void QTNode::subdivide()
{
  printf("Entering subdivide()\n");
  assert(_level != QT_MAX_LEVEL);
  // TODO: Create lookup table for qtnode dimensions, and calc just once
  double half_width = (double) scene._window_width / pow(2.0, _level);

  _children[0] = new QTNode(_level+1, _base + vec2(half_width, half_width),
                            this, NULL);
  _children[1] = new QTNode(_level+1, _base + vec2(0, half_width),
                            this, NULL);
  _children[2] = new QTNode(_level+1, _base + vec2(half_width, 0),
                            this, NULL);
  _children[3] = new QTNode(_level+1, _base, this, NULL);
}

/*
 * Recursively delete this QTNode tree.
 */
void QTNode::clear()
{
  // Base
  if (_children == NULL) return;

  // Recursive
  for (int i = 0; i < 4; ++i)
  {
    delete _children[i];
    _children[i] = NULL;
  }
}

/**
 * Check if this node is a leaf node.
 */
bool QTNode::isLeaf()
{
  return _children[0] == NULL;
}
