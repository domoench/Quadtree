#ifndef __DOM_BB_H__
#define __DOM_BB_H__

#include "constants.h"
#include "glm/glm.hpp"

using namespace glm;

/**
 * A class to represent square bounding boxes.
 */
class BB
{
  public:
    vec2 _min; //!< Bottom left corner position
    vec2 _max; //!< Top right corner position

    BB();
    BB(vec2 min, vec2 max);
};

#endif
