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
  // printf("Constructing level %d node\n", level);
  for (int i = 0; i < 4; i++) {
    _children[i] = NULL;
  }
  _occupied = occupier != NULL;
}

/**
 * Check if this node intersects the given bounding box.
 */
bool QTNode::intersectsBB(const BB& box)
{
  // TODO: SAT intersection test
  double w = (double) scene._window_width / pow(2.0, _level);
  vec2 min = _base;
  vec2 max = _base + vec2(w, w);

  // Check X axis projection
  bool x_intersect = (min[0] <= box._max[0]) &&
                     (max[0] >= box._min[0]);
  // Check Y axis projection
  bool y_intersect = (min[1] <= box._max[1]) &&
                     (max[1] >= box._min[1]);

  // If both axes projections intersect, then BBs intersect
  return x_intersect && y_intersect;
}

/**
 * Check if this node intersects the given Geometry polygon.
 * Returns the intersection ratio (geom area / QTNode area)
 */
float QTNode::intersects(const Geometry& geom)
{
  // Quick Check. TODO: Test if this really gives a speedup
  if (!this->intersectsBB(geom._bb)) return false;

  // Construct clipping box for this node
  Polygon qt_clip_box;
  double w = (double) scene._window_width / pow(2.0, _level);
  qt_clip_box.add(_base);
  qt_clip_box.add(_base + vec2(w,0));
  qt_clip_box.add(_base + vec2(w,w));
  qt_clip_box.add(_base + vec2(0,w));

  // Create clippable temp polygon for geom
  Polygon geom_poly = geom._poly;
  geom_poly.clip(qt_clip_box);

  // printf("Leaving intersects()\n");
  return geom_poly.area() / qt_clip_box.area();
}

/**
 * Insert the given Geometry into this quadtree node.
 *
 * Returns true if successfully inserted, false if it can't be inserted.
 * Insertion fails if the geometry overlaps an existing geometry maintained
 * by the Quadtree.
 */
bool QTNode::insert(const Geometry& geom)
{
  // TODO: We're double computing intersections down the quadree in canInsert and
  //       insert_r(). How to avoid that?
  if (canInsert(geom))
  {
    insert_r(geom, -1);
    return true;
  }
  return false;
}

/**
 * Determine if geom can be inserted into this Quadtree without overlapping
 * any previously inserted geometries.
 */
bool QTNode::canInsert(const Geometry& geom) const
{
  // Occupied Leaf
  if (_occupied) return false;

  // Unoccupied Leaf
  if (isLeaf()) return true;

  // Interior Node. Find what quadrants geom wants to get into
  for (int i = 0; i < 4; ++i)
  {
    QTNode& child = *(_children[i]);
    float intersect_ratio = child.intersects(geom);
    if (intersect_ratio > 0)
    {
      // If any child insertion is impossible => All is lost
      if(!child.canInsert(geom)) return false;
    }
  }
  // All sub insertions will work
  return true;
}

/**
 * Recursive Insert Helper
 *
 * Returns true if successfully inserted, false if it can't be inserted.
 * Insertion fails if the geometry overlaps an existing geometry maintained
 * by the Quadtree.
 *
 * Pre: Geometry intersects this quadtree and doesn't conflict with any
 *      existing other geometries.
 *
 * @param geom The Geometry instance we're inserting into this quadtree
 * @param intersect_ratio The ratio (geom area / quadtree node area). Set to -1
 *        if ratio is unknown.
 * @return True if insertion successful. False if it can't be inserted.
 */
void QTNode::insert_r(const Geometry& geom, float intersect_ratio)
{
  // This is an occupied leaf node
  if (_occupied) return;

  // This is an unoccupied leaf
  if (isLeaf())
  {
    //printf("\tIts a leaf\n");
    if (intersect_ratio == -1)
    {
      intersect_ratio = intersects(geom);
    }
    // printf("intersect_ratio: %f\n", intersect_ratio);

    if (intersect_ratio > SUBDIV_THRESHOLD) // Become an occupied leaf
    {
      //printf("\tOccupied. Base: (%f, %f)\n", _base[0],_base[1]);
      _occupier = &geom;
      _occupied = true;
      return;
    }
    else // geom can't fully occupy, must subdivide
    {
      if (_level == QT_MAX_LEVEL)
      {
        // printf("Reached max subdivision level %d\n", QT_MAX_LEVEL);
        // Can't subdivide further, just mark this node as occupied
        _occupier = &geom;
        _occupied = true;
        return;
      }
      subdivide(); // => Now we've got kids!
    }
  }

  // Find which of the 4 children geom intersects with. We want to insert
  // geom into them.
  for (int i = 0; i < 4; ++i)
  {
    QTNode& child = *(_children[i]);
    float intersect_ratio = child.intersects(geom);
    // printf("\tIntersection ratio with Quadrant %d: %f\n", i, intersect_ratio);
    // If geom intersects this child, we must recursively insert
    if (intersect_ratio > 0) child.insert_r(geom, intersect_ratio);
  }
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
  _children[2] = new QTNode(_level+1, _base,
                            this, NULL);
  _children[3] = new QTNode(_level+1, _base + vec2(half_width, 0),
                            this, NULL);
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
  Model = translate(Model, vec3(_base[0],_base[1],0));
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
