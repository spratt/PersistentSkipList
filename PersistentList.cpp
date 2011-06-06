///////////////////////////////////////////////////////////////////////////////
//                       Copyright (c) 2011 - 2012 by                        //
//                                Simon Pratt                                //
//                           (All rights reserved)                           //
///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// FILE:    PersistentList.cpp                                               //
//                                                                           //
// MODULE:  Persistent List                                                  //
//                                                                           //
// NOTES:   None.                                                            //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include "PersistentList.h"
#include "array_utilities.h"

using namespace std;
using namespace array_utilities;

namespace persistent_list {
  ostream& operator<<(ostream& os, const Point2d& p) {
    os << "(" << p.x << "," << p.y << ")";
    return os;
  }
  ostream& operator<<(ostream& os, const PointListNode& pln) {
    os << pln.point << "->";
    if(pln.next != NULL)
      os << *(pln.next);
    else
      os << "NULL";
    return os;
  }

  void PersistentList::printArray() {
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
  bool PersistentList::xInArray(coord_t x) {
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
  int PersistentList::binarySearchX(coord_t x) {
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
  PointListNode* PersistentList::searchY(int t, coord_t y) {
    PointListNode* pln = points_right[t];
    while(pln != NULL && pln->point.y < y) pln = pln->next;
    return pln;
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
  int PersistentList::insertPoint(coord_t x, coord_t y) {
    // build point
    Point2d p = *(new Point2d(x,y));
    // binary search
    int n = (int)points_sorted_by_x.size();
    int index = 0;
    if(n > 0)
      index = binarySearchX(x);
    // add point to end of vector
    points_sorted_by_x.push_back(p);
    points_right.push_back((PointListNode*)NULL);
    // block permute points between the new point and its final location
    if(n > 0) {
      block_permute(vectorToArray(points_sorted_by_x),index,n-1,n);
      block_permute(vectorToArray(points_right),index,n-1,n);
    }
    n++;
    // copy right neighbor
    PointListNode* pln;
    PointListNode* temp;
    PointListNode* prev;
    if(index + 1 < n) {
      pln = points_right[index+1];
      temp = new PointListNode(pln->point);
      prev = temp;
      points_right[index] = temp;
      while(pln->next != NULL) {
	pln = pln->next;
	temp = new PointListNode(pln->point);
	prev->next = temp;
	prev = temp;
      }
    }
    // add point all left neighbors
    int t = index;
    PointListNode* newpln = new PointListNode(p);
    newpln->next = searchY(index,p.y);
    while(t >= 0) {
      pln = points_right[t];
      if(pln == NULL) { // empty list
	points_right[t] = newpln;
      } else if(pln->point.y > p.y) { // new element belongs before first in list
	points_right[t] = newpln;
      } else { // inserted element belongs somewhere else in list
	while(pln->next != NULL && pln->point.y < p.y)
	  pln = pln->next;
	pln->next = newpln;
      }
      t--;
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
  vector< Point2d > PersistentList::enumerateNE(coord_t x, coord_t y) {
    vector< Point2d > v;
    int index = binarySearchX(x);
    PointListNode* pln = points_right[index];
    while(pln != NULL && pln->point.y < y)
      pln = pln->next;
    while(pln != NULL) {
      v.push_back(pln->point);
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
  Point2d* PersistentList::highestNE(coord_t x, coord_t y) {
    vector< Point2d > v;
    int index = binarySearchX(x);
    PointListNode* pln = points_right[index];
    if(pln == NULL) return NULL;
    while(pln->next != NULL) pln = pln->next;
    return &(pln->point);
  }

  PointListNode* PersistentList::getListAtTime(int t) {
    return points_right[t];
  }

  size_t PersistentList::size() {
    return points_sorted_by_x.size();
  }
}
