#include "geometry.h"
#include "scene.h"

// The scene state is global
extern Scene scene;

/**
 * Construct a Geometry and initialize its OpenGL State: VAO and VBOs.
 */
Geometry::Geometry(int id, const vector<vec2>& vertices) :
  _id(id)
{
  _poly = Polygon(vertices);
  _bb = BB(vertices);

  // Create VAO to manage Vertex and Color VBOs
  // Find unused VAO ID
  glGenVertexArrays(1, &_vao);
  // Make the VAO object current
  glBindVertexArray(_vao);

  // Find unused VBO ID for Vertex Position Buffer
	glGenBuffers(1, &_vbo);
  // Create buffer object of vertex attribute data
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
  // Allocate memory on the GPU and transfer the app data to it
  glBufferData(
    GL_ARRAY_BUFFER,
    _poly._verts->size() * sizeof(vec2),
    _poly._verts->data(),
    GL_STATIC_DRAW
  );
  // Look up where on the GPU we can access our vertex position attribute buffer
	_vert_pos_loc = glGetAttribLocation(scene._prog_ID, "vertex_pos");
	glEnableVertexAttribArray(_vert_pos_loc); // TODO: Find out if you need to enable and disable for each call to scene.drawGeometry(g)

  // Bind our vertex VBO to current VAO
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);
  glVertexAttribPointer(
    _vert_pos_loc,  // The attribute we want to configure
    2,              // Number of elems per vertex for this attribute
    GL_FLOAT,       // type
    GL_FALSE,       // normalized?
    0,              // stride
    (void*)0        // array buffer offset
  );
}

/**
 * Destroy this Geometry object and clean up its OpenGL state.
 */
Geometry::~Geometry()
{
  // _poly destructor will take care of deallocating its vertices

  // TODO: Find out if we need to delete the VAO on the GPU
	glDisableVertexAttribArray(_vert_pos_loc);
	glDeleteBuffers(1, &_vbo);
  // printf("Geometry DESTROYED!\n");
}

/**
 * Return the area of this geometry
 */
GLfloat Geometry::area() const
{
  return _poly.area();
}

/**
 * Render this Geometry object.
 */
void Geometry::draw() const
{
  // Make default shaders current
  glUseProgram(scene._prog_ID);

  // Pass default MVP to the GPU
  mat4 MVP = scene._proj * scene._view * scene._model;
  GLuint MVP_loc = glGetUniformLocation(scene._prog_ID, "MVP");
  glUniformMatrix4fv(MVP_loc, 1, GL_FALSE, &MVP[0][0]);

  // Geometries are red
  GLuint color_loc = glGetUniformLocation(scene._prog_ID, "color");
  glUniform4f(color_loc, 1.0f, 0.0f, 0.0f, 1.0f);

  // Make this geometry's VAO current
  glBindVertexArray(_vao);
  glDrawArrays(GL_LINE_LOOP, 0, _poly._verts->size());
}

