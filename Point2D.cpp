///////////////////////////////////////////////////////////////////////////////
//                       Copyright (c) 2011 - 2012 by                        //
//                                Simon Pratt                                //
//                           (All rights reserved)                           //
///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// FILE:    Point2D.cpp                                                      //
//                                                                           //
// MODULE:  Point2D                                                          //
//                                                                           //
// NOTES:   None.                                                            //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
#include <assert.h>
#include "Point2D.hpp"

namespace point_2d {
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
  point_2d::Point2D numeric_limits<point_2d::Point2D>::infinity() {
    assert(numeric_limits<point_2d::Point2D>::has_infinity);
    return point_2d::Point2D(numeric_limits<point_2d::coord_t>::infinity(),
			     numeric_limits<point_2d::coord_t>::infinity());
  }
  point_2d::Point2D numeric_limits<point_2d::Point2D>::max() {
    assert(! numeric_limits<point_2d::Point2D>::has_infinity);
    return point_2d::Point2D(numeric_limits<point_2d::coord_t>::max(),
			     numeric_limits<point_2d::coord_t>::max());
  }
  point_2d::Point2D numeric_limits<point_2d::Point2D>::min() {
    assert(! numeric_limits<point_2d::Point2D>::has_infinity);
    return point_2d::Point2D(numeric_limits<point_2d::coord_t>::min(),
			     numeric_limits<point_2d::coord_t>::min());
  }
}

