#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "constants.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <GLFW/glfw3.h>
#include <OpenGL/gl3.h>

#include "bb.h"
#include "geometry.h"
#include "polygon.h"
#include "qtnode.h"
#include "scene.h"
#include "shader.hpp"

using namespace glm;

bool runAllTests();
void testBBIntersection();
void testOnLeftSide();
void testPolygon();
void testClipPolygon();
void testPolygonArea();
void testLineIntersection();
void testClipOneSide();
void testBasicQTNode();
void testQTNodeIntersect();
void testQTNodeInsert();
