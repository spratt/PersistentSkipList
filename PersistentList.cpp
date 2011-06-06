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
  ostream& operator<<(ostream& os, const point2d& p) {
    os << "(" << p.x << "," << p.y << ")";
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
    point2d p = points_sorted_by_x[binarySearchX(x)];
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
      point2d p = points_sorted_by_x[index];
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
    point2d p;
    p.x = x;
    p.y = y;
    // binary search
    int n = (int)points_sorted_by_x.size();
    int index = 0;
    if(n > 0)
      index = binarySearchX(x);
    // add point to end of vector
    points_sorted_by_x.push_back(p);
    points_right.push_back(NULL);
    // block permute points between the new point and its final location
    if(n > 0) {
      block_permute(vectorToArray(points_sorted_by_x),index,n-1,n);
      block_permute(vectorToArray(points_right),index,n-1,n);
    }
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
      }
    }
    // add point all left neighbors
    int t = index;
    PointListNode* newpln = new PointListNode(p);
    newpln->next = searchY(index,p.y);
    while(t >= 0) {
      pln = points_right[t];
      if(pln == NULL) {
	points_right[t] = newpln;
      } else {
	while(pln->next != NULL && pln->point.y < p.y)
	  pln = pln->next;
	pln->next = newpln;
      }
      t--;
    }
    // success
    return 0;
  }
}
