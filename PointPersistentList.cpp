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

  bool PointPersistentList::xInArray(coord_t x) {
    Point2d p = points_sorted_by_x[binarySearchX(x)];
    return x == p.x;
  }

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
