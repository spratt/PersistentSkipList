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
  /////////////////////////////////////////////////////////////////////////////
  // Point2d implementation                                                  //
  /////////////////////////////////////////////////////////////////////////////
  ostream& operator<<(ostream& os, const Point2d& p) {
    os << "(" << p.x << "," << p.y << ")";
    return os;
  }

  /////////////////////////////////////////////////////////////////////////////
  // PointListNode implementation                                            //
  /////////////////////////////////////////////////////////////////////////////
  int PointListNode::setNext(int t, PointListNode* ln) {
    // find the nearest index at which to set the next pointer
    int index = getNextIndex(t);
    // if there are no next pointers
    if(index == -1) {
      // push this one
      time.push_back(t);
      next.push_back(ln);
    }
    // if there was already a pointer at that time
    else if(t == time[index]) {
      // set the new pointer
      ln->setNext(t,(PointListNode*)next[index]);
      next[index] = ln;
    }
    // if the nearest index is greater
    else if(time[index] > t) {
      // insert before
      ln->setNext(t,(PointListNode*)next[index]);
      time.insert(time.begin()+index,t);
      next.insert(next.begin()+index,ln);
    }
    // if the nearest index is smaller
    else {
      // insert after
      ++index;
      ln->setNext(t,(PointListNode*)next[index]);
      time.insert(time.begin()+index,t);
      next.insert(next.begin()+index,ln);
    }
    // iterate through the future versions
    ++index;
    while(index < (int)time.size()) {
      // if next should precede the new point
      if(next[index]->data.y < ln->data.y) {
	// set next on next
	return ((PointListNode*)next[index])
	  ->setNext(time[index],(PointListNode*)ln);
      }
      // otherwise, delete this change index so that the previous next
      // will continue through this time
      ln->setNext(time[index],(PointListNode*)next[index]);
      time.erase(time.begin() + index);
      next.erase(next.begin() + index);
    }
    // success
    return 0;
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
  
  ListNode<Point2d>* PointPersistentList::getNodeBefore(int t, coord_t y) {
    // search forward from the first node in the list at time t
    // and return the immediately preceding node
    return getNodeBefore(t,y,points_right.getList(t));
  }

  ListNode<Point2d>* PointPersistentList::getNodeBefore(int t,
							coord_t y,
							ListNode<Point2d>* start) {
    // if that node does not precede y, return NULL
    if(start != NULL && start->data.y >= y)
      return NULL;
    // otherwise, search forward to find the node which immediately precedes
    // the given y
    while(start != NULL &&
	  start->getNext(t) != NULL &&
	  start->getNext(t)->data.y < y)
      start = start->getNext(t);
    // return the immediately preceding node
    return start;
  }

  int PointPersistentList::insertPoint(coord_t x, coord_t y) {
    // build point on stack
    Point2d p(x,y);
    // add point to the array of points sorted by x
    int n = (int)points_sorted_by_x.size();
    int index = 0;
    if(n > 0) {
      index = binarySearchX(x);
      // binary search will return the nearest point already stored
      // if this point is larger than or equal to the new point,
      // we need to insert the new point _after_ the nearest
      // since we are storing the points in descending order
      if(points_sorted_by_x[index].x >= x) index++;
    }
    points_sorted_by_x.insert(points_sorted_by_x.begin()+index,p);
    // since we have just added a new point, increment the number of points
    n++;
    // create the version of the list at which the new node will be inserted
    points_right.newList(index);
    // determine if there is a preceding node
    ListNode<Point2d>* before = getNodeBefore(index,y);
    // create the new node
    ListNode<Point2d>* new_node = new PointListNode(p);
    points_right.registerNode(new_node);
    // while at time index there are no preceding nodes
    while(before == NULL) {
      // set the next of the current node to the previous head
      new_node->setNext(index,(PointListNode*)points_right.getList(index));
      // set the current node to be the new head
      points_right.setHead(index,new_node);
      // break if we have reached the end of the points
      if(++index >= (int)points_right.size())
	break;
      // otherwise, look for a preceding node at the current index
      before = getNodeBefore(index,y);
    }
    // if there is a preceding node
    if(before != NULL) {
      // set its next to the new node
      before->setNext(index,new_node);
    }
    // success
    return 0;
  }

  vector< Point2d > PointPersistentList::enumerateNE(coord_t x, coord_t y) {
    vector< Point2d > v;
    // determine the time at which to search by searching for the x
    int index = binarySearchX(x);
    // get the first node in this list at time index
    ListNode<Point2d>* pln = points_right.getList(index);
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
    // get the first node in this list at time index
    ListNode<Point2d>* pln = points_right.getList(index);
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
    // get the first node in this list at time index
    ListNode<Point2d>* pln = points_right.getList(index);
    // if there are no points NE of the given point, return null
    if(pln == NULL) return NULL;
    // take the first point as left most for now
    leftMost = &(pln->data);
    // set pln to its next pointer
    pln = pln->getNext(index);
    // iterate over nodes in the list
    while(pln != NULL) {
      // check if point is more left than current left most
      if(pln->data.x < leftMost->x)
	leftMost = &(pln->data);
      // set pln to its next pointer
      pln = pln->getNext(index);
    }
    // return the left most point
    return leftMost;
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
