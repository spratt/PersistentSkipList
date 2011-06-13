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
  
  ListNode<Point2d>* PointPersistentList::getNodeBefore(int t, coord_t y) {
    ListNode<Point2d>* pln = points_right.getList(t);
    if(pln != NULL && pln->data.y > y)
      return NULL;
    while(pln != NULL && pln->getNext(t) != NULL && pln->getNext(t)->data.y < y)
      pln = pln->getNext(t);
    return pln;
  }

  int PointPersistentList::insertPoint(coord_t x, coord_t y) {
    // build point on stack
    Point2d p(x,y);
    // add point to the array of points sorted by x
    int n = (int)points_sorted_by_x.size();
    int index = 0;
    if(n > 0) {
      index = binarySearchX(x) + 1;
    }
    points_sorted_by_x.insert(points_sorted_by_x.begin()+index,p);
    // add point to the list at time index
    points_right.newList(index);
    ListNode<Point2d>* before = getNodeBefore(index,y);
    ListNode<Point2d>* new_node = new ListNode<Point2d>(p);
    if(before == NULL)
      points_right.newHead(index,new_node);
    else
      points_right.insertAfterNode(index,before,new_node);
    // success
    return 0;
  }

  vector< Point2d > PointPersistentList::enumerateNE(coord_t x, coord_t y) {
    vector< Point2d > v;
    int index = binarySearchX(x);
    ListNode<Point2d>* pln = points_right.getList(index);
    while(pln != NULL && pln->data.y < y)
      pln = pln->getNext(index);
    while(pln != NULL) {
      v.push_back(pln->data);
      pln = pln->getNext(index);
    }
    return v;
  }

  Point2d* PointPersistentList::highestNE(coord_t x, coord_t y) {
    vector< Point2d > v;
    int index = binarySearchX(x);
    ListNode<Point2d>* pln = points_right.getList(index);
    if(pln == NULL) return NULL;
    while(pln->getNext(index) != NULL) pln = pln->getNext(index);
    return &(pln->data);
  }
  
  ListNode<Point2d>* PointPersistentList::getList(int t) {
    return points_right.getList(t);
  }

  void PointPersistentList::printArray() {
    print(vectorToArray(points_sorted_by_x),(int)points_sorted_by_x.size());
  }
  
  size_t PointPersistentList::size() {
    return points_sorted_by_x.size();
  }
}
