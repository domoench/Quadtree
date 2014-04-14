//#include <GLUT/glut.h>
#include "constants.h"
#include "scene.h"

Scene scene;

// Local Function Declarations
void Init();
void Display();
void KeyHandler();
void MouseButton();
void MouseMotion();

int main(int argc, char** argv) {
  scene.setDimensions(DEFAULT_W, DEFAULT_H);

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
  glutInitWindowSize(DEFAULT_W, DEFAULT_H);
  glutInitWindowPosition(0, 0);
  glutCreateWindow("Quadtree");

  glutDisplayFunc(Display);
  glutKeyboardFunc(KeyHandler);
  glutMouseFunc(MouseButton);
  glutMotionFunc(MouseMotion);
  glutMainLoop();

  Init();
  return 0;
}

void Init() {
  scene.Init();
}

void Display() {
  scene.Display();
}

void KeyHandler() {
  scene.KeyHandler();
}

void MouseButton() {
  scene.MouseButton();
}

void MouseMotion() {
  scene.MouseMotion();
}
