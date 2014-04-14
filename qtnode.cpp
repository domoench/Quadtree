#include "qtnode.h"

/**
 * Construct an leaf quadtree node. Consists of a single node with no children.
 */
QTNode::QTNode(unsigned int _level, vec2 _base, QTNode* _parent,
               vector<Geometry> _geometries) :
  level(_level), base(_base), parent(_parent), geometries(_geometries)
{
  children = NULL;
}
