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
    unsigned int level;
    vec2 base;          //!< (x,y) coordinate of this node's bottom left corner
    QTNode* parent;
    QTNode* children;   //!< Array of 4 child pointers. NULL if this is leafnode.

    //! A list of Geometry objects that intersect this node
    vector<Geometry> geometries;

    QTNode(unsigned int _level, vec2 _base, QTNode* _parent,
           vector<Geometry> geometries);
};
#endif
