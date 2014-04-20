#ifndef __DOM_BB_H__
#define __DOM_BB_H__

#include <vector>
#include "constants.h"
#include "glm/glm.hpp"

using namespace glm;
using namespace std;

/**
 * A class to represent bounding boxes.
 */
class BB
{
  public:
    vec2 _min; //!< Bottom left corner position
    vec2 _max; //!< Top right corner position

    BB();
    BB(const vector<vec2>* vertices);
    BB(vec2 min, vec2 max);

    bool intersects(const BB& other);
};

#endif
