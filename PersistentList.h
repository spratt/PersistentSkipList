//-*- mode: c++ -*-////////////////////////////////////////////////////////////
//                       Copyright (c) 2011 - 2012 by                        //
//                                Simon Pratt                                //
//                           (All rights reserved)                           //
///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// FILE:    PersistentList.h                                                 //
//                                                                           //
// MODULE:  Persistent List                                                  //
//                                                                           //
// PURPOSE: Implements a persistent list data structure.                     //
//                                                                           //
// NOTES:   None.                                                            //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// Public Variable:                     Description:                         //
// ----------------                     ------------                         //
// persistent_list::coord_t             the type of a coordinate             //
// persistent_list::Point2d             the type of a point in a 2d plane    //
// persistent_list::PersistentList      an implementation of a linked        //
//                                      list, in which previous              //
//                                      versions may be accessed and         //
//                                      modified                             //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//                             Public Methods:                               //
//                                                                           //
// PersistentList::PersistentList()                                          //
// PersistentList::insertPoint(coord_t x, coord_t y)                         //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
#ifndef PERSISTENTLIST_H
#define PERSISTENTLIST_H

#include <vector>

using namespace std;

namespace persistent_list {

  typedef double coord_t;
  
  class Point2d {
  public:
    // data
    coord_t x, y;

    // methods
    Point2d(const Point2d& p)
      : x(p.x), y(p.y)
    {}
    Point2d(coord_t x, coord_t y)
      : x(x), y(y)
    {}
  };

  class PointListNode {
  public:
    // data
    Point2d point;
    PointListNode* next;
    
    // methods
    PointListNode(Point2d p)
      : point(p), next((PointListNode*)NULL)
    {}
  };

  ostream& operator<<(ostream& os, const Point2d& p);
  ostream& operator<<(ostream& os, const PointListNode& pln);
  
  class PersistentList {
    // Data
    vector<Point2d> points_sorted_by_x;
    vector< PointListNode* > points_right;
    // Methods
    int binarySearchX(coord_t x);
    PointListNode* searchY(int t, coord_t y);
    bool xInArray(coord_t x);
  public:
    PersistentList()
      : points_sorted_by_x(), points_right()
    {}
    int insertPoint(coord_t x,coord_t y);
    vector< Point2d > enumerateNE(coord_t x, coord_t y);
    Point2d* highestNE(coord_t x, coord_t y);
    void printArray();
    PointListNode* getListAtTime(int t);
    size_t size();
  };
}

#endif
