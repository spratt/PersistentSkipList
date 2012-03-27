///////////////////////////////////////////////////////////////////////////////
//                       Copyright (c) 2011 - 2012 by                        //
//                                Simon Pratt                                //
//                           (All rights reserved)                           //
///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// FILE:    Point2D.cpp                                                      //
//                                                                           //
// MODULE:  Geometry                                                         //
//                                                                           //
// NOTES:   None.                                                            //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
#include <assert.h>
#include "Point2D.hpp"

namespace geometry {
  /////////////////////////////////////////////////////////////////////////////
  // Point2D implementation                                                  //
  /////////////////////////////////////////////////////////////////////////////
  bool operator>(const Point2D& a, const Point2D& b) {
    return a.x > b.x;
  }

  bool operator<(const Point2D& a, const Point2D& b) {
    return a.x < b.x;
  }
  
  ostream& operator<<(ostream& os, const Point2D& p) {
    os << "(" << p.x << "," << p.y << ")";
    return os;
  }
}
namespace std {
  /////////////////////////////////////////////////////////////////////////////
  // Define limits for points                                                //
  /////////////////////////////////////////////////////////////////////////////
  geometry::Point2D numeric_limits<geometry::Point2D>::infinity() {
    assert(numeric_limits<geometry::Point2D>::has_infinity);
    return geometry::Point2D(numeric_limits<geometry::coord_t>::infinity(),
			     numeric_limits<geometry::coord_t>::infinity());
  }
  geometry::Point2D numeric_limits<geometry::Point2D>::max() {
    assert(! numeric_limits<geometry::Point2D>::has_infinity);
    return geometry::Point2D(numeric_limits<geometry::coord_t>::max(),
			     numeric_limits<geometry::coord_t>::max());
  }
  geometry::Point2D numeric_limits<geometry::Point2D>::min() {
    assert(! numeric_limits<geometry::Point2D>::has_infinity);
    return geometry::Point2D(numeric_limits<geometry::coord_t>::min(),
			     numeric_limits<geometry::coord_t>::min());
  }
}

