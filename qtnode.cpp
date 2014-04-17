#include "qtnode.h"

/**
 * Construct an leaf quadtree node. Consists of a single node with no children.
 */
QTNode::QTNode(unsigned int _level, vec2 _base, QTNode* _parent,
               const Geometry* _occupier) :
  level(_level), base(_base), parent(_parent), occupier(_occupier)
{
  children = NULL;
  occupied = _occupier != NULL;
}
