//-*- mode: c++ -*-////////////////////////////////////////////////////////////
//                       Copyright (c) 2011 - 2012 by                        //
//                                Simon Pratt                                //
//                           (All rights reserved)                           //
///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// FILE:    PointPersistentList.h                                            //
//                                                                           //
// MODULE:  Persistent List                                                  //
//                                                                           //
// NOTES:   Uses a persistent list to store points and perform queries.      //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
#ifndef POINTPERSISTENTLIST_H
#define POINTPERSISTENTLIST_H

#include <vector>
#include "PersistentList.h"

namespace persistent_list {
  typedef int coord_t;
  
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
  ostream& operator<<(ostream& os, const Point2d& p);
  
  class PointPersistentList {
    // Data
    vector<Point2d> points_sorted_by_x;
    PersistentList< Point2d > points_right;
    // Methods
    int binarySearchX(coord_t x);
    ListNode<Point2d>* searchY(int t, coord_t y);
    int indexY(int t, coord_t y);
    bool xInArray(coord_t x);
  public:
    PointPersistentList()
      : points_sorted_by_x(), points_right()
    {}
    int insertPoint(coord_t x,coord_t y);
    vector< Point2d > enumerateNE(coord_t x, coord_t y);
    Point2d* highestNE(coord_t x, coord_t y);
    void printArray();
    ListNode<Point2d>* getList(int t);
    size_t size();
  };
}

#endif
