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
// PURPOSE: None.                                                            //
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
    LineSegment(Point2D&,Point2D&);
    ~LineSegment();

    const Point2D& getFirstEndPoint() const;
    const Point2D& getSecondEndPoint() const;

  private:
    Point2D first, second;
  };

  ostream& operator<<(ostream&,const LineSegment&);
    
}

#endif
