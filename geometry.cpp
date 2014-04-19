#include "geometry.h"
#include "scene.h"

// The scene state is global
extern Scene scene;

/**
 * Construct a Geometry and initialize its OpenGL State: VAO and VBOs.
 */
Geometry::Geometry(int id, vector<vec2>* vertices, vector<int>* edges) :
  _id(id), _vertices(vertices), _edges(edges)
{
  printf("Constructing Geometry object with %lu vertices\n", vertices->size());
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
    _vertices->size() * sizeof(vec2),
    _vertices->data(),
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

  _bb = new BB(vertices); // TODO: Initialize the BB correctly
}

/**
 * Destroy this Geometry object and clean up its OpenGL state.
 */
Geometry::~Geometry()
{
  // TODO: Find out if we need to delet the VAO on the GPU
	glDisableVertexAttribArray(_vert_pos_loc);
	glDeleteBuffers(1, &_vbo);
}
