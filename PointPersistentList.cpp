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
#include <assert.h>
#include <iostream>
#include <vector>
#include <map>
#include "PersistentList.h"
#include "PointPersistentList.h"
#include "array_utilities.h"
#include "sort/heap_sort.h"

using namespace std;
using namespace array_utilities;

namespace persistent_list {
  /////////////////////////////////////////////////////////////////////////////
  // Point2d implementation                                                  //
  /////////////////////////////////////////////////////////////////////////////
  bool operator>(const Point2d& a, const Point2d& b) {
    return a.x > b.x;
  }
  
  ostream& operator<<(ostream& os, const Point2d& p) {
    os << "(" << p.x << "," << p.y << ")";
    return os;
  }

  /////////////////////////////////////////////////////////////////////////////
  // PointPersistentList implementation                                      //
  /////////////////////////////////////////////////////////////////////////////
  // assumes points_sorted_by_x is sorted in descending order
  int PointPersistentList::binarySearchX(coord_t x) {
    int index = -1;
    int begin = 0, end = (int)points_sorted_by_x.size()-1;
    while(begin <= end) {
      index = (begin+end)/2;
      Point2d p = points_sorted_by_x[index];
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

  int PointPersistentList::insertPoints(Point2d* points, int npoints) {
    assert(npoints > 0);
    // sort the points by x coordinate             O(nlogn)
    // descending
    sort::heap_sort(points,0,npoints-1);
    // for each point
    for(int i = npoints-1; i >= 0; --i) { //       O(n)
      // insert into structure
      if(points_right.insert(points[i]) == 0) //   O(logn)
	// if not a duplicate, add to points sorted by x
	points_sorted_by_x.push_back(points[i]);// O(1)
    }
    // lock the structure against any more points being inserted
    points_right.lock();
    // success
    return 0;
  }

  size_t PointPersistentList::memoryUsage() {
    return (sizeof(Point2d) * points_sorted_by_x.size()) +
      points_right.memoryUsage();
  }

  vector< Point2d > PointPersistentList::enumerateNE(coord_t x, coord_t y) {
    vector< Point2d > v;
    // determine the time at which to search by searching for the x
    int index = binarySearchX(x);
    // if set of points is empty, bail out
    if(index == -1) return v;
    // while the closest point is too small
    while(points_sorted_by_x[index].x < x) {
      // check the previous point, which should be larger since the
      // array is sorted by x descending
      --index;
      // if we have passed the beginning of the array, then there are no
      // points within the query region
      if(index < 0)
	return v;
    }
    // get the first node in this list at time index
    ListNode<Point2d, Point2d::yxdesc >* pln = points_right.getList(index);
    // while the current point is not null and has a greater or equal
    // y than the query
    while(pln != NULL && pln->data.y >= y) {
      // push the point onto the list to be returned
      v.push_back(pln->data);
      // move on to next point
      pln = pln->getNext(index);
    }
    return v;
  }

  Point2d* PointPersistentList::highestNE(coord_t x, coord_t y) {
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
    ListNode<Point2d, Point2d::yxdesc >* pln = points_right.getList(index);
    // if there are no points NE of the given point, return null
    if(pln == NULL || pln->data.y < y) return NULL;
    // since the list is sorted by y coordinate descending, the first
    // node is the highest
    return &(pln->data);
  }

  Point2d* PointPersistentList::leftMostNE(coord_t x, coord_t y) {
    Point2d* leftMost = NULL;
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
    ListNode<Point2d, Point2d::yxdesc >* pln = points_right.getList(index);
    // if there are no points NE of the given point, return null
    if(pln == NULL || pln->data.y < y) return NULL;
    // take the first point as left most for now
    leftMost = &(pln->data);
    // set pln to its next pointer
    pln = pln->getNext(index);
    // iterate over nodes in the list until:
    //  - we reach the end
    // OR
    //  - the points are no longer NE of the query point
    while(pln != NULL && pln->data.y >= y) {
      // check if point is more left than current left most
      if(pln->data.x < leftMost->x)
	leftMost = &(pln->data);
      // set pln to its next pointer
      pln = pln->getNext(index);
    }
    // return the left most point
    return leftMost;
  }
  
  ListNode<Point2d, Point2d::yxdesc >* PointPersistentList::getList(int t) {
    return points_right.getList(t);
  }

  void PointPersistentList::printArray() {
    print(vectorToArray(points_sorted_by_x),(int)points_sorted_by_x.size());
  }
  
  size_t PointPersistentList::size() {
    return points_sorted_by_x.size();
  }
}
