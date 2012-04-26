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
    : sweep_points()
  {
  }

  PolygonalSubdivision::~PolygonalSubdivision() {
  }

  void PolygonalSubdivision::addLineSegment(LineSegment& ls) {
    line_segments.push_back(ls);
    sweep_points.insert(ls.getFirstEndPoint());
    sweep_points.insert(ls.getSecondEndPoint());
    // we don't need to sweep at line intersections because a
    // polygonal subdivision won't have intersections
  }

  void PolygonalSubdivision::addLineSegment(int ax, int ay, int bx, int by) {
    LineSegment a(ax,ay,bx,by);
    addLineSegment(a);
  }

  LineSegment& PolygonalSubdivision::locate_point(Point2D& p) {
    // basic error checking
    if(line_segments.empty())
      throw "No line segments";
    
    return line_segments[0];
  }

}
