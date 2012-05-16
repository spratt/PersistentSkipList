///////////////////////////////////////////////////////////////////////////////
//                       Copyright (c) 2011 - 2012 by                        //
//                                Simon Pratt                                //
//                           (All rights reserved)                           //
///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// FILE:    LineSegment.hpp                                                  //
//                                                                           //
// MODULE:  Geometry                                                         //
//                                                                           //
// PURPOSE: Stores the information associated with a line segment in         //
//          R2.  In other words, a pair of end points of the line segment.   //
//                                                                           //
// NOTES:   None.                                                            //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
// Public Variable:                     Description:                         //
// ----------------                     ------------                         //
///////////////////////////////////////////////////////////////////////////////
//                             Public Methods:                               //
///////////////////////////////////////////////////////////////////////////////
#ifndef LINESEGMENT_HPP
#define LINESEGMENT_HPP

#include "Point2D.hpp"
#include <ostream>

namespace geometry {
  class LineSegment {
  public:
    LineSegment(int ax=0, int ay=0, int bx=0, int by=0);
    LineSegment(Point2D&,Point2D&);
    ~LineSegment();

    const Point2D& getFirstEndPoint() const;
    const Point2D& getSecondEndPoint() const;

    bool operator<(const LineSegment&) const;
    bool operator>(const LineSegment&) const;

  private:
    Point2D first, second;
  };

  ostream& operator<<(ostream&,const LineSegment&);
    
}

#endif
