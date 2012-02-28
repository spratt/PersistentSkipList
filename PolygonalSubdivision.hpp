///////////////////////////////////////////////////////////////////////////////
//                       Copyright (c) 2011 - 2012 by                        //
//                                Simon Pratt                                //
//                           (All rights reserved)                           //
///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// FILE:    PolygonalSubdivision.hpp                                         //
//                                                                           //
// MODULE:  Planar Point Location                                            //
//                                                                           //
// PURPOSE: Solves the planar point location problem using a                 //
//          persistent skiplist.                                             //
//                                                                           //
// NOTES:   None.                                                            //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
// Public Variable:                     Description:                         //
// ----------------                     ------------                         //
///////////////////////////////////////////////////////////////////////////////
//                             Public Methods:                               //
///////////////////////////////////////////////////////////////////////////////

#include <vector>
#include "Point2D.hpp"
#include "PersistentSkipList.hpp"

using namespace point_2d;
using namespace persistent_skip_list;

#ifndef POLYGONALSUBDIVISION_HPP
#define POLYGONALSUBDIVISION_HPP

namespace planar_point_location {
  typedef struct {
    Point2D a, b;
  } LineSegment;
  
  class PolygonalSubdivision {
  private:
    vector< Point2D > sweep_points;
    PersistentSkipList< LineSegment > line_crossings;
  public:
    PolygonalSubdivision();
    ~PolygonalSubdivision();
    
    LineSegment locate_point(Point2D&);
  };
}

#endif
