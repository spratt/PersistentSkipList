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
#include <sstream>

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

  bool LineSegment::operator<(const LineSegment& other) const {
    // sort by y coordinate descending
    const int max_y       = (first.y > second.y) ?
      first.y : second.y;
    const int other_max_y = (other.first.y > other.second.y) ?
      other.first.y : other.second.y;
    // this precedes other iff this segment's y is higher
    return max_y > other_max_y;
  }

  bool LineSegment::operator>(const LineSegment& other) const {
    return !operator<(other);
  }

  ostream& operator<<(ostream& o, const LineSegment& ls) {
    stringstream s;
    s << "(" << ls.getFirstEndPoint() << ","
      << ls.getSecondEndPoint() << ")";
    o << s.str();
    return o;
  }
  
}
