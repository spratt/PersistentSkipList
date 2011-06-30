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
      if(p.x == x) {
	break;
      } else if(p.x < x) { // descending
	end = index -1;
      } else {
	begin = index +1;
      }
    }
    return index;
  }

  int PointPersistentList::insertPoints(Point2d* points, int npoints) {
    assert(npoints > 0);
    // sort the points by x coordinate             O(nlogn)
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
    // get the first node in this list at time index
    ListNode<Point2d, Point2d::yxasc >* pln = points_right.getList(index);
    // find the first point with a y coordinate equal to or greater than y
    while(pln != NULL && pln->data.y < y)
      pln = pln->getNext(index);
    // return all points with y coordinate higher than that point
    while(pln != NULL) {
      v.push_back(pln->data);
      pln = pln->getNext(index);
    }
    return v;
  }

  Point2d* PointPersistentList::highestNE(coord_t x, coord_t y) {
    // determine the time at which to search by searching for the x
    int index = binarySearchX(x);
    // if set of points is empty, bail out
    if(index == -1) return NULL;
    // if we have gone too far
    if(points_sorted_by_x[index].x < x) {
      // go back one
      --index;
      // if we are now before the beginning, the smallest x in the set
      // of points is larger than the x we are searching for, so bail
      if(index < 0)
	return NULL;
    }
    // get the first node in this list at time index
    ListNode<Point2d, Point2d::yxasc >* pln = points_right.getList(index);
    // if there are no points NE of the given point, return null
    if(pln == NULL) return NULL;
    // since the list is sorted by y coordinate ascending, find the
    // last node in the list
    while(pln->getNext(index) != NULL) pln = pln->getNext(index);
    // return the address of the point contained in the last node
    // which must be the highest y if the invariant of sorted by
    // y coordinate ascending is upheld
    return &(pln->data);
  }

  Point2d* PointPersistentList::leftMostNE(coord_t x, coord_t y) {
    Point2d* leftMost = NULL;
    // determine the time at which to search by searching for the x
    int index = binarySearchX(x);
    if(index == -1) return NULL;
    if(points_sorted_by_x[index].x < x) {
      --index;
      if(index < 0)
	return NULL;
    }
    // get the first node in this list at time index
    ListNode<Point2d, Point2d::yxasc >* pln = points_right.getList(index);
    // if there are no points NE of the given point, return null
    if(pln == NULL) return NULL;
    // take the first point as left most for now
    leftMost = &(pln->data);
    // set pln to its next pointer
    pln = pln->getNext(index);
    // iterate over nodes in the list
    while(pln != NULL) {
      assert(leftMost != NULL);
      // check if point is more left than current left most
      if(pln->data.x < leftMost->x)
	leftMost = &(pln->data);
      // set pln to its next pointer
      pln = pln->getNext(index);
    }
    // return the left most point
    return leftMost;
  }
  
  ListNode<Point2d, Point2d::yxasc >* PointPersistentList::getList(int t) {
    return points_right.getList(t);
  }

  void PointPersistentList::printArray() {
    print(vectorToArray(points_sorted_by_x),(int)points_sorted_by_x.size());
  }
  
  size_t PointPersistentList::size() {
    return points_sorted_by_x.size();
  }
}
