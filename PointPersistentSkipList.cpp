///////////////////////////////////////////////////////////////////////////////
//                       Copyright (c) 2011 - 2012 by                        //
//                                Simon Pratt                                //
//                           (All rights reserved)                           //
///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// FILE:    PointPersistentSkipList.cpp                                      //
//                                                                           //
// MODULE:  Persistent Skip List                                             //
//                                                                           //
// NOTES:   None.                                                            //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
#include <assert.h>
#include <vector>
#include "PersistentSkipList.hpp"
#include "PointPersistentSkipList.hpp"
#include "array_utilities.hpp"

using namespace std;
using namespace array_utilities;

namespace persistent_skip_list {
  /////////////////////////////////////////////////////////////////////////////
  // PointPersistentSkipList implementation                                  //
  /////////////////////////////////////////////////////////////////////////////
  // assumes points_sorted_by_x is sorted in descending order
  int PointPersistentSkipList::binarySearchX(coord_t x) {
    int index = -1;
    int begin = 0, end = (int)points_sorted_by_x.size()-1;
    while(begin <= end) {
      index = (begin+end)/2;
      Point2D p = points_sorted_by_x[index];
      // found the point
      if(p.x == x) {
	break;
      }
      // current point is smaller than desired
      // since the array is sorted descending, we know that the
      // desired point must lie in the preceding portion
      else if(p.x < x) {
	// search the preceding portion of the array
	end = index -1;
      } else {
	// otherwise, search the following portion of the array
	begin = index +1;
      }
    }
    return index;
  }

  int PointPersistentSkipList::insertPoint(Point2D& point) {
    if(points_right.insert(point) == 0) {
      // assume points added in x-order,
      // add to end of points sorted by x
      points_sorted_by_x.push_back(point);
    }
    // success
    return 0;
  }

  vector< Point2D > PointPersistentSkipList::enumerateNE(coord_t x, coord_t y) {
    vector< Point2D > v;
    // determine the time at which to search by searching for the x
    int index = binarySearchX(x);
    // if set of points is empty, bail out
    if(index == -1) return v;
    // while the closest point is too large
    while(points_sorted_by_x[index].x > x) {
      // check the previous point, which should be smaller since the
      // array is sorted by x ascending
      --index;
      // if we have passed the beginning of the array, then there are no
      // points within the query region
      if(index < 0)
    	return v;
    }
    // get the first node in this list at time index
    TimeStampedArray< ListNode<Point2D>* >* tsa = points_right.getHead(index);
    // if there are no points, return the empty vector
    if(tsa == NULL)
      return v;
    // find the first ListNode to check
    //while(
    // while the current point is not null and has a greater or equal
    // y than the query
    // while(pln != NULL && pln->data.y >= y) {
    //   // push the point onto the list to be returned
    //   v.push_back(pln->data);
    //   // move on to next point
    //   pln = pln->getNext(index);
    // }
    return v;
  }

  Point2D* PointPersistentSkipList::highestNE(coord_t x, coord_t y) {
    // determine the time at which to search by searching for the x
    int index = binarySearchX(x);
    // if set of points is empty, bail out
    if(index == -1) return NULL;
    // while the closest point is too small
    while(points_sorted_by_x[index].x < x) {
      // check the previous point, which should be larger since the
      // array is sorted by x descending
      --index;
      // if we have passed the beginning of the array, then there are no
      // points within the query region
      if(index < 0)
    	return NULL;
    }
    // get the first node in this list at time index
    // ListNode<Point2D, Point2D::yxdesc >* pln = points_right.getList(index);
    // // if there are no points NE of the given point, return null
    // if(pln == NULL || pln->data.y < y) return NULL;
    // // since the list is sorted by y coordinate descending, the first
    // // node is the highest
    // return &(pln->data);
    return NULL;
  }

  Point2D* PointPersistentSkipList::leftMostNE(coord_t x, coord_t y) {
    Point2D* leftMost = NULL;
    // determine the time at which to search by searching for the x
    int index = binarySearchX(x);
    // if set of points is empty, bail out
    if(index == -1) return NULL;
    // while the closest point is too small
    while(points_sorted_by_x[index].x < x) {
      // check the previous point, which should be larger since the
      // array is sorted by x descending
      --index;
      // if we have passed the beginning of the array, then there are no
      // points within the query region
      if(index < 0)
    	return NULL;
    }
    // get the first node in this list at time index
    // ListNode<Point2D, Point2D::yxdesc >* pln = points_right.getList(index);
    // // if there are no points NE of the given point, return null
    // if(pln == NULL || pln->data.y < y) return NULL;
    // // take the first point as left most for now
    // leftMost = &(pln->data);
    // // set pln to its next pointer
    // pln = pln->getNext(index);
    // // iterate over nodes in the list until:
    // //  - we reach the end
    // // OR
    // //  - the points are no longer NE of the query point
    // while(pln != NULL && pln->data.y >= y) {
    //   // check if point is more left than current left most
    //   if(pln->data.x < leftMost->x)
    // 	leftMost = &(pln->data);
    //   // set pln to its next pointer
    //   pln = pln->getNext(index);
    // }
    // // return the left most point
    return leftMost;
  }
  
  // ListNode<Point2D, Point2D::yxdesc >* PointPersistentSkipList::getHead(int t) {
  //   return points_right.getHead(t);
  // }
}
