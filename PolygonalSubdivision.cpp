///////////////////////////////////////////////////////////////////////////////
//                       Copyright (c) 2011 - 2012 by                        //
//                                Simon Pratt                                //
//                           (All rights reserved)                           //
///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// FILE:    PolygonalSubdivision.cpp                                         //
//                                                                           //
// MODULE:  Planar Point Location                                            //
//                                                                           //
// NOTES:   None.                                                            //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include "PolygonalSubdivision.hpp"

namespace geometry {

  PolygonalSubdivision::PolygonalSubdivision()
    : sweep_points(), line_crossings()
  {
  }

  PolygonalSubdivision::~PolygonalSubdivision() {
  }

  LineSegment& PolygonalSubdivision::locate_point(Point2D& p) {
    Point2D a,b;
    LineSegment ls(a,b);
    LineSegment& ls_ref = ls;
    return ls_ref;
  }

}
