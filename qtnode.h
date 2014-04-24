#ifndef __DOM_QTNODE_H__
#define __DOM_QTNODE_H__

#include <vector>
#include "constants.h"
#include "glm/glm.hpp"
#include "geometry.h"

using namespace std;
using namespace glm;

/**
 * A class to represent dynamic region quadtrees.
 *
 * Quadrant ordering:
 *    +–––––––+–––––––+
 *    |       |       |
 *    |  II   |   I   |
 *    |       |       |
 *    +–––––––––––––––+
 *    |       |       |
 *    |  III  |  IV   |
 *    |       |       |
 *    +–––––––+–––––––+
 */
class QTNode
{
  public:
    unsigned int _level;
    vec2 _base;          //!< (x,y) coordinate of this node's bottom left corner
    QTNode* _parent;

    /*!
      Array of 4 child pointers. If children == NULL, then this node is a
      leafnode.
     */
    QTNode* _children[4];

    /*!
      True if this is an occupied leaf node. False if this is an interior node
      or unoccupied.
     */
    bool _occupied;

    /*!
      The Geometry that occupies this node. NULL if this is an interior node
      or unoccupied.
     */
    const Geometry* _occupier;

    QTNode(unsigned int level, vec2 base, QTNode* parent,
           const Geometry* occupier);
    bool  intersects(const BB& box);
    float intersects(const Geometry& geom);
    bool  insert(const Geometry& geom);
    void  clear();
    void  subdivide();
    bool  isLeaf() const;
    float area() const;
    void  draw() const;
    void  draw_r(GLuint vao_ID) const;
    int   size() const;
};
#endif
