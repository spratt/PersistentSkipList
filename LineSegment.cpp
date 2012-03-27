///////////////////////////////////////////////////////////////////////////////
//                       Copyright (c) 2011 - 2012 by                        //
//                                Simon Pratt                                //
//                           (All rights reserved)                           //
///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// FILE:    LineSegment.cpp                                                  //
//                                                                           //
// MODULE:  Geometry                                                         //
//                                                                           //
// NOTES:   None.                                                            //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include "LineSegment.hpp"

namespace geometry {

  LineSegment::LineSegment(int ax, int ay, int bx, int by)
    : first(ax,ay), second(bx,by)
  {
  }
  
  LineSegment::LineSegment(Point2D& a, Point2D& b)
    : first(a), second(b)
  {
  }

  LineSegment::~LineSegment() {}

  const Point2D& LineSegment::getFirstEndPoint() const { return first; }
  const Point2D& LineSegment::getSecondEndPoint() const { return second; }

  ostream& operator<<(ostream& o, const LineSegment& ls) {
    o << "(" << ls.getFirstEndPoint() << ","
      << ls.getSecondEndPoint() << ")";
    return o;
  }
  
}
