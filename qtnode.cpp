#include "qtnode.h"
#include "scene.h"
#include "polygon.h"

extern Scene scene;

/**
 * Construct a leaf quadtree node. Consists of a single node with no children.
 */
QTNode::QTNode(unsigned int level, vec2 base, QTNode* parent,
               const Geometry* occupier) :
  _level(level), _base(base), _parent(parent), _occupier(occupier)
{
  assert (level < 32);
  printf("Constructing level %d node\n", level);
  for (int i = 0; i < 4; i++) {
    _children[i] = NULL;
  }
  _occupied = occupier != NULL;
}

/**
 * Check if this node intersects the given bounding box.
 */
bool QTNode::intersects(const BB& box)
{
  // TODO: SAT intersection test
  return false;
}

/**
 * Check if this node intersects the given Geometry polygon.
 * Returns the intersection ratio (geom area / QTNode area)
 */
float QTNode::intersects(const Geometry& geom)
{
  // Construct clipping box for this node
  Polygon qt_clip_box;
  double w = (double) scene._window_width / pow(2.0, _level);
  qt_clip_box.add(_base);
  qt_clip_box.add(_base + vec2(w,0));
  qt_clip_box.add(_base + vec2(w,w));
  qt_clip_box.add(_base + vec2(0,w));

  // Generate a temp polygon for intersection test
  Polygon geom_poly(*(geom._vertices));
  geom_poly.clip(qt_clip_box);

  // printf("geom_poly.area(): %f\n", geom_poly.area());
  // printf("qt_clip_box.area(): %f\n", qt_clip_box.area());
  // printf("Leaving intersects()\n");
  return geom_poly.area() / qt_clip_box.area();
}

/**
 * Recursively insert the given Geometry into this quadtree node.
 *
 * Returns true if successfully inserted, false if it can't be inserted.
 * Insertion fails if the geometry overlaps an existing geometry maintained
 * by the Quadtree.
 *
 * Pre: Geometry intersects this node
 */
bool QTNode::insert(const Geometry& geom)
{
  printf("Inserting into node of level %d\n", _level);
  // This is an occupied leaf node
  if (_occupied) return false;

  // This is an unoccupied leaf
  if (isLeaf())
  {
    printf("\tIts a leaf\n");
    float ratio = geom.area() / this->area();

    if (ratio > SUBDIV_THRESHOLD) // Become an occupied leaf
    {
      printf("\tOccupied.\n");
      _occupier = &geom;
      _occupied = true;
      return true;
    }
    else // geom can't fully occupy, must subdivide
    {
      printf("\tMust Subdivide.\n");
      assert(_level != QT_MAX_LEVEL);
      subdivide(); // => Now we've got kids!
      // TODO: What if we've reached max level
    }
  }

  // Find which of the 4 children geom intersects with. We must insert geom
  // into them.
  for (int i = 0; i < 4; ++i)
  {
    QTNode& child = *(_children[i]);
    float intersect_area = child.intersects(geom);
    printf("Intersection area with Quadrant %d: %f\n", i+1, intersect_area);
    // If geom intersects this child, we must recursively insert
    if (intersect_area > 0)
    {
      bool insert_success = child.insert(geom);
      if (!insert_success) return false; // Any failure => Complete failure
    }
  }

  // We made it
  return true;
}

/*
 * Subdivide this quadtree node by creating 4 unoccupied children.
 */
void QTNode::subdivide()
{
  // printf("Entering subdivide()\n");
  assert(_level != QT_MAX_LEVEL);
  // TODO: Create lookup table for qtnode dimensions, and calc just once
  double half_width = (double) scene._window_width / pow(2.0, _level + 1);

  _children[0] = new QTNode(_level+1, _base + vec2(half_width, half_width),
                            this, NULL);
  _children[1] = new QTNode(_level+1, _base + vec2(0, half_width),
                            this, NULL);
  _children[2] = new QTNode(_level+1, _base + vec2(half_width, 0),
                            this, NULL);
  _children[3] = new QTNode(_level+1, _base, this, NULL);
}

/*
 * Recursively delete this QTNode tree.
 */
void QTNode::clear()
{
  // Base
  if (isLeaf()) return;

  // Recursive
  for (int i = 0; i < 4; ++i)
  {
    delete _children[i];
    _children[i] = NULL;
  }
}

/**
 * Check if this node is a leaf node.
 */
bool QTNode::isLeaf() const
{
  return _children[0] == NULL;
}

/**
 * Return the area of this QTNode instance
 */
float QTNode::area() const
{
  Polygon qt_poly;
  double w = (double) scene._window_width / pow(2.0, _level);
  qt_poly.add(_base);
  qt_poly.add(_base + vec2(w,0));
  qt_poly.add(_base + vec2(w,w));
  qt_poly.add(_base + vec2(0,w));

  return qt_poly.area();
}

/**
 * Render this quadtree.
 */
void QTNode::draw() const
{
  // Set up OpenGL stuff
  // Unit square for our instancing
  vector<vec2> square;
  square.push_back(vec2(0,0));
  square.push_back(vec2(1,0));
  square.push_back(vec2(1,1));
  square.push_back(vec2(0,1));

  // Use the default shader program
  glUseProgram(scene._prog_ID);

  // Create VAO to manage Vertex and Color VBOs
  GLuint vao_ID;
  glGenVertexArrays(1, &vao_ID);
  glBindVertexArray(vao_ID);

  // Create VBOs // TODO: Add color VBO
  GLuint vbo_ID;
	glGenBuffers(1, &vbo_ID);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_ID);
  glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(vec2), square.data(),
               GL_STATIC_DRAW);
	GLuint vert_pos_loc = glGetAttribLocation(scene._prog_ID, "vertex_pos");
	glEnableVertexAttribArray(vert_pos_loc); // TODO: Find out if you need to enable and disable for each call to scene.drawGeometry(g)

  // Bind our vertex VBO to current VAO
	glBindBuffer(GL_ARRAY_BUFFER, vbo_ID);
  glVertexAttribPointer(
    vert_pos_loc,  // The attribute we want to configure
    2,              // Number of elems per vertex for this attribute
    GL_FLOAT,       // type
    GL_FALSE,       // normalized?
    0,              // stride
    (void*)0        // array buffer offset
  );

  // QTNode boundaries are blue
  GLuint color_loc = glGetUniformLocation(scene._prog_ID, "color");
  glUniform4f(color_loc, 0.0f, 0.0f, 1.0f, 1.0f);

  // Recursively render
  draw_r(vao_ID);
}

/**
 * Recursive helper: Render this quadtree.
 */
void QTNode::draw_r(GLuint vao_ID) const
{
  // printf("Drawing a level %d node\n", _level);

  // Calculate MVP for this Node - Translate and Scale
  mat4 Model = scene._model;
  // Translate according to this node's position
  Model = translate(Model, vec3(_base[0],_base[0],0));
  // Scale according to this node's size
  double w = (double) scene._window_width / pow(2.0, _level);
  Model = scale(Model, vec3(w,w,w));
  // Form new MVP and send it to the vertex shader
  mat4 MVP = scene._proj * scene._view * Model;
  GLuint MVP_ID = glGetUniformLocation(scene._prog_ID, "MVP");
  glUniformMatrix4fv(MVP_ID, 1, GL_FALSE, &MVP[0][0]);

  // Render the transformed square for this node
  glDrawArrays(GL_LINE_LOOP, 0, 4);

  // Recursively draw children
  if (!isLeaf())
  {
    // printf("Not A Leaf\n");
    for (int i = 0; i < 4; ++i)
    {
      _children[i]->draw_r(vao_ID);
    }
  }
}

/**
 * Return the number of node objects in this quadtree
 */
int QTNode::size() const
{
  if (isLeaf()) return 1;
  else
  {
    int num_children = 0;
    for (int i = 0; i < 4; ++i)
    {
      num_children += _children[i]->size();
    }
    return 1 + num_children;
  }
}
