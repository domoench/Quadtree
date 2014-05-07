#ifndef __DOM_CONSTANTS_H__
#define __DOM_CONSTANTS_H__

//! Debug mode flag
#define DEBUG false

//! The maximum number of quadtree levels. Root is 0, Deepest is QT_N_LEVELS - 1.
#define QT_N_LEVELS  9
#define QT_MAX_LEVEL 8

//! The ratio of a QTNode that must be filled to mark it as occupied
#define SUBDIV_THRESHOLD 0.985f

//! Default window width in pixels
#define DEFAULT_W 860
//! Default window height in pixels
#define DEFAULT_H 860

#endif
