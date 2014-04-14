#ifndef __DOM_BB_H__
#define __DOM_BB_H__

#include "constants.h"
#include "glm/glm.hpp"

using namespace glm;

/**
 * A class to represent square bounding boxes.
 */
class BB {
  public:
    vec2 min; //!< Bottom left corner position
    vec2 max; //!< Top right corner position

    BB(vec2 _min, vec2 _max);
};

#endif
