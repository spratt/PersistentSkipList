///////////////////////////////////////////////////////////////////////////////
//                       Copyright (c) 2011 - 2012 by                        //
//                                Simon Pratt                                //
//                           (All rights reserved)                           //
///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// FILE:    test_polygonal_subdivision.cpp                                   //
//                                                                           //
// MODULE:  Polygonal Subdivision                                            //
//                                                                           //
// NOTES:   None.                                                            //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include "../Point2D.hpp"
#include "../LineSegment.hpp"
#include "../PolygonalSubdivision.hpp"

using namespace geometry;

int main(int argc, char** argv) {
  PolygonalSubdivision ps;
  ps.addLineSegment(0,0, 0,1);
  ps.addLineSegment(0,1, 1,0);
  ps.addLineSegment(1,0, 0,0);
  return 0;
}
