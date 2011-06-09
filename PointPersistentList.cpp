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
  
  ListNode<Point2d>* PointPersistentList::searchY(int t, coord_t y) {
    ListNode<Point2d>* pln = points_right.getList(t);
    while(pln != NULL && pln->data.y < y) pln = pln->next;
    return pln;
  }
  
  ListNode<Point2d>* PointPersistentList::getNodeBefore(int t, coord_t y) {
    ListNode<Point2d>* pln = points_right.getList(t);
    if(pln != NULL && pln->data.y > y)
      return NULL;
    while(pln != NULL && pln->next != NULL && pln->next->data.y < y)
      pln = pln->next;
    return pln;
  }

  int PointPersistentList::indexY(int t, coord_t y) {
    assert(t < (int)points_right.size());
    ListNode<Point2d>* pln = points_right.getList(t);
    int i = 0;
    while(pln != NULL && pln->data.y < y) {
      pln = pln->next;
      i++;
    }
    return i;
  }

  int PointPersistentList::duplicateListBeforeY(int t, coord_t y) {
    // duplicating before the 0th list gives an empty list at position 0
    if(t < 0) {
      points_right.newList(0);
    } else {
      // Otherwise, check that t is within bounds
      int n = (int)points_right.size();
      assert(t < n);
      // if so, get the first node in the list to copy
      ListNode<Point2d>* list = points_right.getList(t);
      // create a new (empty) list after the copied list
      points_right.newList(t+1);
      // if the list to copy is empty, or the first element comes after
      // the given y, simply make the new list a reference to the old list
      if(list == NULL || list->data.y >= y) {
	points_right.set(t+1,0,list);
      } else {
	// otherwise, duplicate the first node
	ListNode<Point2d>* dup = new ListNode<Point2d>(list);
	// Insert the duplicated node at position 0 in the new (empty) list
	points_right.insert(t+1,0,dup);
	// save the previous node copied (for next reference) 
	ListNode<Point2d>* prev = dup;
	// as long as the node isn't null, and the next node precedes
	// the given y coordinate
	while(list->next != NULL && list->next->data.y < y) {
	  // duplicate next node
	  list = list->next;
	  dup = new ListNode<Point2d>(list);
	  // preserve the list order
	  prev->next = dup;
	  // save the node just copied for next reference
	  prev = dup;
	}
	// finally, once all the preceding nodes have been copied, copy
	// the next pointer of the following node
	if(list != NULL)
	  dup->next = list->next;
      }
    }
    // success
    return 0;
  }

  int PointPersistentList::insertPoint(coord_t x, coord_t y) {
    // build point
    Point2d* p = new Point2d(x,y);
    int n = (int)points_sorted_by_x.size();
    // determine the position at which to insert the new point
    int index = 0;
    if(n > 0) {
      index = binarySearchX(x);
      if(points_sorted_by_x[index].x >= x)
	index++;
    }
    points_sorted_by_x.insert(points_sorted_by_x.begin()+index,*p);
    // first point
    if(n == 0) {
      points_right.newList(0);
      points_right.insert(0,0,new ListNode<Point2d>(*p));
    } else {
      // create a new time in the persistent list with all preceding elements
      duplicateListBeforeY(index-1,y);
      for(int t = index; t <= n; t++) {
	ListNode<Point2d>* node_before = getNodeBefore(t,y);
	ListNode<Point2d>* new_node = new ListNode<Point2d>(*p); 
	if(node_before == NULL) {
	  new_node->next = getList(t);
	  points_right.insert(t,0,new_node);
	} else
	  points_right.insertAfterNode(t,node_before,new_node);
      }
    }
    delete p;
    // success
    return 0;
  }

  vector< Point2d > PointPersistentList::enumerateNE(coord_t x, coord_t y) {
    vector< Point2d > v;
    int index = binarySearchX(x);
    ListNode<Point2d>* pln = points_right.getList(index);
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
    ListNode<Point2d>* pln = points_right.getList(index);
    if(pln == NULL) return NULL;
    while(pln->next != NULL) pln = pln->next;
    return &(pln->data);
  }
  
  size_t PointPersistentList::size() {
    return points_sorted_by_x.size();
  }
  
  ListNode<Point2d>* PointPersistentList::getList(int t) {
    return points_right.getList(t);
  }
}
