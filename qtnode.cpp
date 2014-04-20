#include "qtnode.h"

/**
 * Construct an leaf quadtree node. Consists of a single node with no children.
 */
QTNode::QTNode(unsigned int level, vec2 base, QTNode* parent,
               const Geometry* occupier) :
  _level(level), _base(base), _parent(parent), _occupier(occupier)
{
  _children = NULL;
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
