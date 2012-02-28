//-*- mode: c++ -*-////////////////////////////////////////////////////////////
//                       Copyright (c) 2011 - 2012 by                        //
//                                Simon Pratt                                //
//                           (All rights reserved)                           //
///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// FILE:    PointPersistentSkipList.h                                        //
//                                                                           //
// MODULE:  Persistent Skip List                                             //
//                                                                           //
// PURPOSE: Uses a persistent skip list to store points and perform          //
//          queries.                                                         //
//                                                                           //
// NOTES:   None.                                                            //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// Public Variable:                     Description:                         //
// ----------------                     ------------                         //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//                             Public Methods:                               //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
#ifndef POINTPERSISTENTSKIPLIST_HPP
#define POINTPERSISTENTSKIPLIST_HPP

#include <vector>
#include "Point2D.hpp"
#include "PersistentSkipList.hpp"

using namespace point_2d;

namespace persistent_skip_list {
  /////////////////////////////////////////////////////////////////////////////
  // PointPersistentSkipList interface                                       //
  /////////////////////////////////////////////////////////////////////////////
  class PointPersistentSkipList {
    // An array of points sorted by x coordinate
    vector< Point2D > points_sorted_by_x;

    // A persistent list of points sorted by y coordinate
    PersistentSkipList< Point2D > points_right;

    /////////////////////////////////////////////////////////////////////////////
    //                                                                         //
    // FUNCTION NAME: binarySearchX                                            //
    //                                                                         //
    // PURPOSE:       determines the position of a 2d point in the sorted      //
    //                array.  If the point is not in the array, this           //
    //                method will return the index of where the element        //
    //                should be in the array.                                  //
    //                                                                         //
    // SECURITY:      private                                                  //
    //                                                                         //
    // PARAMETERS                                                              //
    //   Type/Name:   coord_t/x                                                //
    //   Description: The x coordinate for which to search.                    //
    //                                                                         //
    // RETURN:        The index of the x coordinate in the array, or the       //
    //                index where the x coordinate should be.                  //
    //                                                                         //
    // NOTES:         None.                                                    //
    //                                                                         //
    /////////////////////////////////////////////////////////////////////////////
    int binarySearchX(coord_t x);
    
  public:
    ///////////////////////////////////////////////////////////////////////////
    //                                                                       //
    // FUNCTION NAME: PointPersistentSkipList                                    //
    //                                                                       //
    // PURPOSE:       Empty constructor.                                     //
    //                                                                       //
    // SECURITY:      public                                                 //
    //                                                                       //
    // PARAMETERS                                                            //
    //   Type/Name:   int/size                                               //
    //   Description: The desired node size.                                 //
    //                                                                       //
    // NOTES:         None.                                                  //
    //                                                                       //
    ///////////////////////////////////////////////////////////////////////////
    PointPersistentSkipList()
      : points_sorted_by_x(), points_right()
    { }

    ///////////////////////////////////////////////////////////////////////////
    //                                                                       //
    // FUNCTION NAME: insertPoints                                           //
    //                                                                       //
    // PURPOSE:       Inserts a point.                                       //
    //                                                                       //
    // SECURITY:      public                                                 //
    //                                                                       //
    // PARAMETERS                                                            //
    //   Type/Name:   Point2D/point                                          //
    //   Description: A point to be added.                                   //
    //                                                                       //
    // RETURN:        An integer return code.  0 means success.              //
    //                                                                       //
    // NOTES:         None.                                                  //
    //                                                                       //
    ///////////////////////////////////////////////////////////////////////////
    int insertPoint(Point2D& point);

    /////////////////////////////////////////////////////////////////////////////
    //                                                                         //
    // FUNCTION NAME: enumerateNE                                              //
    //                                                                         //
    // PURPOSE:       Returns the a vector of all points where the             //
    //                x-coordinate is greater than or equal to the given       //
    //                x, and the y-coordinate is greater than or equal         //
    //                to the given y.                                          //
    //                                                                         //
    // SECURITY:      public                                                   //
    //                                                                         //
    // PARAMETERS                                                              //
    //   Type/Name:   coord_t/x                                                //
    //   Description: The minimum x coordinate to consider.                    //
    //                                                                         //
    //   Type/Name:   coord_t/y                                                //
    //   Description: The minimum y coordinate to consider.                    //
    //                                                                         //
    // RETURN:        A vector of all points p s.t. p.x >= x, p.y >= y.        //
    //                                                                         //
    // NOTES:         None.                                                    //
    //                                                                         //
    /////////////////////////////////////////////////////////////////////////////
    vector< Point2D > enumerateNE(coord_t x, coord_t y);
    
    /////////////////////////////////////////////////////////////////////////////
    //                                                                         //
    // FUNCTION NAME: highestNE                                                //
    //                                                                         //
    // PURPOSE:       Returns the highest point in the set of all points       //
    //                where the x-coordinate is greater than or equal to       //
    //                the given x, and the y-coordinate is greater than        //
    //                or equal to the given y.                                 //
    //                                                                         //
    // SECURITY:      public                                                   //
    //                                                                         //
    // PARAMETERS                                                              //
    //   Type/Name:   coord_t/x                                                //
    //   Description: The minimum x coordinate to consider.                    //
    //                                                                         //
    //   Type/Name:   coord_t/y                                                //
    //   Description: The minimum y coordinate to consider.                    //
    //                                                                         //
    // RETURN:        The highest point p where p.x >= x, p.y >= y.            //
    //                                                                         //
    // NOTES:         None.                                                    //
    //                                                                         //
    /////////////////////////////////////////////////////////////////////////////
    Point2D* highestNE(coord_t x, coord_t y);

    ///////////////////////////////////////////////////////////////////////////
    //                                                                       //
    // FUNCTION NAME: leftMostNE                                             //
    //                                                                       //
    // PURPOSE:       Returns the point NE of a given point with             //
    //                lowest x-coordinate.                                   //
    //                                                                       //
    // SECURITY:      public                                                 //
    //                                                                       //
    // PARAMETERS                                                            //
    //   Type/Name:   coord_t/x                                              //
    //   Description: X coordinate                                           //
    //                                                                       //
    //   Type/Name:   coord_t/y                                              //
    //   Description: Y coordinate                                           //
    //                                                                       //
    // RETURN:                                                               //
    //   Type/Name:   Point2D                                                //
    //   Description: Left-most point NE of the given point.                 //
    //                                                                       //
    // NOTES:         None.                                                  //
    //                                                                       //
    ///////////////////////////////////////////////////////////////////////////
    Point2D* leftMostNE(coord_t x, coord_t y);
    
    /////////////////////////////////////////////////////////////////////////////
    //                                                                         //
    // FUNCTION NAME: getHead                                                  //
    //                                                                         //
    // PURPOSE:       Returns the head of the skip list at time t.             //
    //                                                                         //
    // SECURITY:      public                                                   //
    //                                                                         //
    // PARAMETERS                                                              //
    //   Type/Name:   int/t                                                    //
    //   Description: The time (version) of the list structure.                //
    //                                                                         //
    // RETURN:                                                                 //
    //   Type/Name:   ListNode<Point2D>                                        //
    //   Description: The head of the list at time t.                          //
    //                                                                         //
    // NOTES:         None.                                                    //
    //                                                                         //
    /////////////////////////////////////////////////////////////////////////////
    // TimeStampedArray< ListNode< Point2D >* >* getHead(int t);
  };
}

#endif
