#ifndef __DOM_QTNODE_H__
#define __DOM_QTNODE_H__

#include <vector>
#include "constants.h"
#include "geometry.h"
#include "glm/glm.hpp"

using namespace std;
using namespace glm;

/**
 * A class to represent dynamic region quadtrees.
 */
class QTNode
{
  public:
    unsigned int level; // TODO: Create global lookup table => width[level]
    vec2 base;          //!< (x,y) coordinate of this node's bottom left corner
    QTNode* parent;

    /*!
      Array of 4 child pointers. If children == NULL, then this node is a
      leafnode.
     */
    QTNode* children;

    /*!
      True if this is an occupied leaf node. False if this is an interior node
      or unoccupied.
     */
    bool occupied;

    /*!
      The geometry that occupies this node. NULL if this is an interior node
      or unoccupied.
     */
    const Geometry* occupier;

    QTNode(unsigned int _level, vec2 _base, QTNode* _parent,
           const Geometry* _occupier);
};
#endif
