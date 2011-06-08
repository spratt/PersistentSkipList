///////////////////////////////////////////////////////////////////////////////
//                       Copyright (c) 2011 - 2012 by                        //
//                                Simon Pratt                                //
//                           (All rights reserved)                           //
///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// FILE:    PointPersistentList.cpp                                          //
//                                                                           //
// MODULE:  Persistent List                                                  //
//                                                                           //
// NOTES:   None.                                                            //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include "PersistentList.h"
#include "PointPersistentList.h"
#include "array_utilities.h"

using namespace std;
using namespace array_utilities;

namespace persistent_list {
  ostream& operator<<(ostream& os, const Point2d& p) {
    os << "(" << p.x << "," << p.y << ")";
    return os;
  }

  void PointPersistentList::printArray() {
    print(vectorToArray(points_sorted_by_x),(int)points_sorted_by_x.size());
  }
  /////////////////////////////////////////////////////////////////////////////
  //                                                                         //
  // FUNCTION NAME: xInArray                                                 //
  //                                                                         //
  // PURPOSE:       Determines if a point with a given x coordinate is       //
  //                in the array.                                            //
  //                                                                         //
  // SECURITY:      private                                                  //
  //                                                                         //
  // PARAMETERS                                                              //
  //   Type/Name:   coord_t/x                                                //
  //   Description: The x coordinate for which to search.                    //
  //                                                                         //
  // RETURN:        True  - x coordinate exists in array                     //
  //                False - x coordinate does not exist in array             //
  //                                                                         //
  // NOTES:         None.                                                    //
  //                                                                         //
  /////////////////////////////////////////////////////////////////////////////
  bool PointPersistentList::xInArray(coord_t x) {
    Point2d p = points_sorted_by_x[binarySearchX(x)];
    return x == p.x;
  }
  
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
  int PointPersistentList::binarySearchX(coord_t x) {
    int index = -1;
    int begin = 0, end = (int)points_sorted_by_x.size();
    while(begin <= end) {
      index = (begin+end)/2;
      Point2d p = points_sorted_by_x[index];
      if(p.x == x) {
	break;
      } else if(p.x > x) {
	end = index -1;
      } else {
	begin = index +1;
      }
    }
    return index;
  }
  
  /////////////////////////////////////////////////////////////////////////////
  //                                                                         //
  // FUNCTION NAME: binarySearchY                                            //
  //                                                                         //
  // PURPOSE:       determines the position of a 2d point in array.  If      //
  //                the point is not in the array, this method will          //
  //                return the index of where the element should be in       //
  //                the array.                                               //
  //                                                                         //
  // SECURITY:      private                                                  //
  //                                                                         //
  // PARAMETERS                                                              //
  //   Type/Name:   int/t                                                    //
  //   Description: Since there are several versions of the list, this       //
  //                searches the list at time t.                             //
  //                                                                         //
  //   Type/Name:   coord_t/y                                                //
  //   Description: The y coordinate for which to search.                    //
  //                                                                         //
  // RETURN:        A pointer to the first point with y-coordinate           //
  //                higher than or equal to the given y.                     //
  //                                                                         //
  // NOTES:         None.                                                    //
  //                                                                         //
  /////////////////////////////////////////////////////////////////////////////
  ListNode<Point2d>* PointPersistentList::searchY(int t, coord_t y) {
    ListNode<Point2d>* pln = getList(t);
    while(pln != NULL && pln->data.y < y) pln = pln->next;
    return pln;
  }

  int PointPersistentList::indexY(int t, coord_t y) {
    assert(t < (int)points_right.size());
    ListNode<Point2d>* pln = getList(t);
    int i = 0;
    while(pln != NULL && pln->data.y < y) {
      pln = pln->next;
      i++;
    }
    return i;
  }

  /////////////////////////////////////////////////////////////////////////////
  //                                                                         //
  // FUNCTION NAME: insertPoint                                              //
  //                                                                         //
  // PURPOSE:       Inserts a point into the structure, with given x and y   //
  //                coordinates.                                             //
  //                                                                         //
  // SECURITY:      public                                                   //
  //                                                                         //
  // PARAMETERS                                                              //
  //   Type/Name:   coord_t/x                                                //
  //   Description: x-coordinate                                             //
  //                                                                         //
  //   Type/Name:   coord_t/y                                                //
  //   Description: y-coordinate                                             //
  //                                                                         //
  // RETURN:                                                                 //
  //   Type/Name:   int                                                      //
  //   Description: 0 for success.                                           //
  //                                                                         //
  // NOTES:         None.                                                    //
  //                                                                         //
  /////////////////////////////////////////////////////////////////////////////
  int PointPersistentList::insertPoint(coord_t x, coord_t y) {
    // build point
    Point2d p = *(new Point2d(x,y));
    // binary search
    int n = (int)points_sorted_by_x.size();
    int index = 0;
    if(n > 0)
      index = binarySearchX(x);
    // add point to end of vector
    points_sorted_by_x.push_back(p);
    // first point
    if(n == 0) {
      points_right.newList(0);
      points_right.insert(0,0,new ListNode<Point2d>(p));
    } else { // if this isn't the first point, need to rearrange other points
      // block permute points between the new point and its final location
      block_permute(vectorToArray(points_sorted_by_x),index,n-1,n);
      // create a new time in the persistent list with all preceding elements
      int t = n-index-1;
      points_right.duplicateList(t,indexY(t,y)-1);
      for(int i = t+1; i <= n; i++)
	points_right.insert(i,indexY(t,y),new ListNode<Point2d>(p));
    }
    // success
    return 0;
  }

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
  vector< Point2d > PointPersistentList::enumerateNE(coord_t x, coord_t y) {
    vector< Point2d > v;
    int index = binarySearchX(x);
    ListNode<Point2d>* pln = getList(index);
    while(pln != NULL && pln->data.y < y)
      pln = pln->next;
    while(pln != NULL) {
      v.push_back(pln->data);
      pln = pln->next;
    }
    return v;
  }

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
  Point2d* PointPersistentList::highestNE(coord_t x, coord_t y) {
    vector< Point2d > v;
    int index = binarySearchX(x);
    ListNode<Point2d>* pln = getList(index);
    if(pln == NULL) return NULL;
    while(pln->next != NULL) pln = pln->next;
    return &(pln->data);
  }

  size_t PointPersistentList::size() {
    return points_sorted_by_x.size();
  }
  ListNode<Point2d>* PointPersistentList::getList(int t) {
    int n = (int)size();
    return points_right.getList(n-t-1);
  }
}
