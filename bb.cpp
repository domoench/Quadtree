#include "bb.h"

BB::BB() : _min(vec2(0.0f,0.0f)), _max(vec2(0.0f, 0.0f))
{}

/**
 * TODO
 */
BB::BB(vec2 min, vec2 max) : _min(min), _max(max)
{}
