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
#include "Point2D.hpp"

namespace Point2D {
  /////////////////////////////////////////////////////////////////////////////
  // Point2D implementation                                                  //
  /////////////////////////////////////////////////////////////////////////////
  bool operator>(const Point2D& a, const Point2D& b) {
    return a.x > b.x;
  }
  
  ostream& operator<<(ostream& os, const Point2D& p) {
    os << "(" << p.x << "," << p.y << ")";
    return os;
  }
}
