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
// PURPOSE: Uses a persistent list to store points and perform queries.      //
//                                                                           //
// NOTES:   None.                                                            //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// Public Variable:                     Description:                         //
// ----------------                     ------------                         //
// coord_t Point2d.x                    the x coordinate of a point          //
// coord_t Point2d.y                    the y coordinate of a point          //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//                             Public Methods:                               //
//                                                                           //
// Point2d::Point2d(coord_t x, coord_t y)                                    //
// bool operator>(const Point2d& a, const Point2d& b)                        //
// ostream& operator<<(ostream& os, const Point2d& p)                        //
//                                                                           //
// PointListNode(const Point2d& p)                                           //
// int PointListNode::setNext(int t, PointListNode* ln)                      //
//                                                                           //
// PersistentList::PointPersistentList()                                     //
// int PersistentList::insertPoints(Point2d* points, int npoints)            //
// int lock()                                                                //
// vector<Point2d> PersistentList::enumerateNE(coord_t x, coord_t y)         //
// Point2d* PersistentList::highestNE(coord_t x, coord_t y)                  //
// ListNode<Point2d>* PersistentList::getList(int t)                         //
// void PersistentList::printArray()                                         //
// size_t PersistentList::size()                                             //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
#ifndef POINTPERSISTENTLIST_H
#define POINTPERSISTENTLIST_H

#include <map>
#include "PersistentList.h"

namespace persistent_list {
  // Defines the implementation and precision of coordinates
  typedef int coord_t;
  
  /////////////////////////////////////////////////////////////////////////////
  // Point2d interface                                                       //
  /////////////////////////////////////////////////////////////////////////////
  class Point2d {
  public:
    coord_t x, y;

    ///////////////////////////////////////////////////////////////////////////
    //                                                                       //
    // FUNCTION NAME: Point2d                                                //
    //                                                                       //
    // PURPOSE:       Empty constructor                                      //
    //                                                                       //
    // SECURITY:      public                                                 //
    //                                                                       //
    // PARAMETERS                                                            //
    //   Type/Name:   Void.                                                  //
    //   Description: None.                                                  //
    //                                                                       //
    // NOTES:         None.                                                  //
    //                                                                       //
    ///////////////////////////////////////////////////////////////////////////
    Point2d()
      : x(0), y(0)
    {}
    
    ///////////////////////////////////////////////////////////////////////////
    //                                                                       //
    // FUNCTION NAME: Point2d                                                //
    //                                                                       //
    // PURPOSE:       Basic constructor.                                     //
    //                                                                       //
    // SECURITY:      public                                                 //
    //                                                                       //
    // PARAMETERS                                                            //
    //   Type/Name:   coord_t/x                                              //
    //   Description: The x coordinate of a point in 2d.                     //
    //                                                                       //
    //   Type/Name:   coord_t/y                                              //
    //   Description: The y coordinate of a point in 2d.                     //
    //                                                                       //
    // NOTES:         None.                                                  //
    //                                                                       //
    ///////////////////////////////////////////////////////////////////////////
    Point2d(coord_t x, coord_t y)
      : x(x), y(y)
    {}

    struct yxasc {
      bool operator()(const Point2d& a, const Point2d& b) const {
	if(a.y == b.y)
	  return a.x < b.x;
	return a.y < b.y;
      }
    };
  };

  /////////////////////////////////////////////////////////////////////////////
  //                                                                         //
  // FUNCTION NAME: operator>                                                //
  //                                                                         //
  // PURPOSE:       Compares two Point2d objects based on their x coordinate.//
  //                                                                         //
  // SECURITY:      public                                                   //
  //                                                                         //
  // PARAMETERS                                                              //
  //   Type/Name:   Point2d/a                                                //
  //   Description: The first point to compare.                              //
  //                                                                         //
  //   Type/Name:   Point2d/b                                                //
  //   Description: The second point to compare.                             //
  //                                                                         //
  // RETURN:        True if a is greater than b, false otherwise.            //
  //                                                                         //
  // NOTES:         None.                                                    //
  //                                                                         //
  /////////////////////////////////////////////////////////////////////////////
  bool operator>(const Point2d& a, const Point2d& b);

  /////////////////////////////////////////////////////////////////////////////
  //                                                                         //
  // FUNCTION NAME: operator<<                                               //
  //                                                                         //
  // PURPOSE:       Formats a Point2d nicely for an ostream.                 //
  //                                                                         //
  // SECURITY:      public                                                   //
  //                                                                         //
  // PARAMETERS                                                              //
  //   Type/Name:   ostream/os                                               //
  //   Description: An os to which to send the Point2d.                      //
  //                                                                         //
  //   Type/Name:   Point2d/p                                                //
  //   Description: The Point2d to format and send.                          //
  //                                                                         //
  // RETURN:        The ostream input.                                       //
  //                This is standard practice for chaining such as:          //
  //                cout << "Point: " << point << endl;                      //
  //                                                                         //
  // NOTES:         None.                                                    //
  //                                                                         //
  /////////////////////////////////////////////////////////////////////////////
  ostream& operator<<(ostream& os, const Point2d& p);

  /////////////////////////////////////////////////////////////////////////////
  // PointListNode interface                                                 //
  /////////////////////////////////////////////////////////////////////////////
  class PointListNode : public ListNode<Point2d> {
  public:
    PointListNode(const Point2d& p)
      : ListNode<Point2d>(p)
    {}
    ///////////////////////////////////////////////////////////////////////////
    //                                                                       //
    // FUNCTION NAME: setNext                                                //
    //                                                                       //
    // PURPOSE:       Set the next pointer at time t                         //
    //                                                                       //
    // SECURITY:      public                                                 //
    //                                                                       //
    // PARAMETERS                                                            //
    //   Type/Name:   int/t                                                  //
    //   Description: The time at which to set the pointer.                  //
    //                                                                       //
    //   Type/Name:   PointListNode*/ln                                      //
    //   Description: The pointer to which to assign next.                   //
    //                                                                       //
    // RETURN:        int return code.  0 means success                      //
    //                                                                       //
    // NOTES:         Since this node type knows the ordering, it can        //
    //                also set the next pointer in future versions.          //
    //                                                                       //
    ///////////////////////////////////////////////////////////////////////////
    int setNext(int t, PointListNode* ln);
  };
  
  /////////////////////////////////////////////////////////////////////////////
  // PointPersistentList interface                                           //
  /////////////////////////////////////////////////////////////////////////////
  class PointPersistentList {
    bool _LOCKED;
    
    // An array of points sorted by x coordinate
    vector< Point2d > points_sorted_by_x;

    // A persistent list of points sorted by y coordinate
    PersistentList< Point2d > points_right;

    // A tree to speed up insertion of points
    map< Point2d, PointListNode*, Point2d::yxasc >* point_tree;

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
    int insertPoint(const Point2d& p);

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
    int binarySearchX(coord_t x);
    
  public:
    ///////////////////////////////////////////////////////////////////////////
    //                                                                       //
    // FUNCTION NAME: PointPersistentList                                    //
    //                                                                       //
    // PURPOSE:       Empty constructor.                                     //
    //                                                                       //
    // SECURITY:      public                                                 //
    //                                                                       //
    // PARAMETERS                                                            //
    //   Type/Name:   Void.                                                  //
    //   Description: None.                                                  //
    //                                                                       //
    // NOTES:         None.                                                  //
    //                                                                       //
    ///////////////////////////////////////////////////////////////////////////
    PointPersistentList()
      : _LOCKED(false), points_sorted_by_x(), points_right()
    {
      point_tree = new map< Point2d , PointListNode*, Point2d::yxasc >();
    }

    ///////////////////////////////////////////////////////////////////////////
    //                                                                       //
    // FUNCTION NAME: insertPoints                                           //
    //                                                                       //
    // PURPOSE:       Inserts an array of points efficiently and             //
    //                closes the structure against future changes.           //
    //                                                                       //
    // SECURITY:      public                                                 //
    //                                                                       //
    // PARAMETERS                                                            //
    //   Type/Name:   Point2d*/points                                        //
    //   Description: An array of points to be added.                        //
    //                                                                       //
    //   Type/Name:   int/npoints                                            //
    //   Description: The number of elements in the array.                   //
    //                                                                       //
    // RETURN:        An integer return code.  0 means success.              //
    //                                                                       //
    // NOTES:         The order of the points may change as a side effect.   //
    //                                                                       //
    ///////////////////////////////////////////////////////////////////////////
    int insertPoints(Point2d* points, int npoints);

    ///////////////////////////////////////////////////////////////////////////
    //                                                                       //
    // FUNCTION NAME: lock                                                   //
    //                                                                       //
    // PURPOSE:       Locks the data structure and frees all memory          //
    //                used while inserting points.                           //
    //                                                                       //
    // SECURITY:      public                                                 //
    //                                                                       //
    // PARAMETERS                                                            //
    //   Type/Name:   Void.                                                  //
    //   Description: None.                                                  //
    //                                                                       //
    // RETURN:        0  means success.                                      //
    //                -1 means already locked.                               //
    //                                                                       //
    // NOTES:         None.                                                  //
    //                                                                       //
    ///////////////////////////////////////////////////////////////////////////
    int lock();

    /////////////////////////////////////////////////////////////////////////////
    //                                                                         //
    // FUNCTION NAME: enumerateNE                                              //
    //                                                                         //
    // PURPOSE:       Returns the a vector of all points where the             //
    //                x-coordinate is greater than or equal to the given       //
    //                x, and the y-coordinate is greater than or equal         //
    //                to the given y.                                          //
    //                                                                         //
    // SECURITY:      public                                                   //
    //                                                                         //
    // PARAMETERS                                                              //
    //   Type/Name:   coord_t/x                                                //
    //   Description: The minimum x coordinate to consider.                    //
    //                                                                         //
    //   Type/Name:   coord_t/y                                                //
    //   Description: The minimum y coordinate to consider.                    //
    //                                                                         //
    // RETURN:        A vector of all points p s.t. p.x >= x, p.y >= y.        //
    //                                                                         //
    // NOTES:         None.                                                    //
    //                                                                         //
    /////////////////////////////////////////////////////////////////////////////
    vector< Point2d > enumerateNE(coord_t x, coord_t y);
    
    /////////////////////////////////////////////////////////////////////////////
    //                                                                         //
    // FUNCTION NAME: highestNE                                                //
    //                                                                         //
    // PURPOSE:       Returns the highest point in the set of all points       //
    //                where the x-coordinate is greater than or equal to       //
    //                the given x, and the y-coordinate is greater than        //
    //                or equal to the given y.                                 //
    //                                                                         //
    // SECURITY:      public                                                   //
    //                                                                         //
    // PARAMETERS                                                              //
    //   Type/Name:   coord_t/x                                                //
    //   Description: The minimum x coordinate to consider.                    //
    //                                                                         //
    //   Type/Name:   coord_t/y                                                //
    //   Description: The minimum y coordinate to consider.                    //
    //                                                                         //
    // RETURN:        The highest point p where p.x >= x, p.y >= y.            //
    //                                                                         //
    // NOTES:         None.                                                    //
    //                                                                         //
    /////////////////////////////////////////////////////////////////////////////
    Point2d* highestNE(coord_t x, coord_t y);

    ///////////////////////////////////////////////////////////////////////////
    //                                                                       //
    // FUNCTION NAME: leftMostNE                                             //
    //                                                                       //
    // PURPOSE:       Returns the point NE of a given point with             //
    //                lowest x-coordinate.                                   //
    //                                                                       //
    // SECURITY:      public                                                 //
    //                                                                       //
    // PARAMETERS                                                            //
    //   Type/Name:   coord_t/x                                              //
    //   Description: X coordinate                                           //
    //                                                                       //
    //   Type/Name:   coord_t/y                                              //
    //   Description: Y coordinate                                           //
    //                                                                       //
    // RETURN:                                                               //
    //   Type/Name:   Point2d                                                //
    //   Description: Left-most point NE of the given point.                 //
    //                                                                       //
    // NOTES:         None.                                                  //
    //                                                                       //
    ///////////////////////////////////////////////////////////////////////////
    Point2d* leftMostNE(coord_t x, coord_t y);
    
    /////////////////////////////////////////////////////////////////////////////
    //                                                                         //
    // FUNCTION NAME: getList                                                  //
    //                                                                         //
    // PURPOSE:       Returns the list of points at time t.                    //
    //                                                                         //
    // SECURITY:      public                                                   //
    //                                                                         //
    // PARAMETERS                                                              //
    //   Type/Name:   int/t                                                    //
    //   Description: The time (version) of the list structure.                //
    //                                                                         //
    // RETURN:                                                                 //
    //   Type/Name:   ListNode<Point2d>                                        //
    //   Description: The list at time t.                                      //
    //                                                                         //
    // NOTES:         None.                                                    //
    //                                                                         //
    /////////////////////////////////////////////////////////////////////////////
    ListNode<Point2d>* getList(int t);
    
    /////////////////////////////////////////////////////////////////////////////
    //                                                                         //
    // FUNCTION NAME: printArray                                               //
    //                                                                         //
    // PURPOSE:       Prints the points sorted by x to cout.                   //
    //                                                                         //
    // SECURITY:      public                                                   //
    //                                                                         //
    // PARAMETERS                                                              //
    //   Type/Name:   Void.                                                    //
    //   Description: None.                                                    //
    //                                                                         //
    // RETURN:        Void.                                                    //
    //                                                                         //
    // NOTES:         None.                                                    //
    //                                                                         //
    /////////////////////////////////////////////////////////////////////////////
    void printArray();

    /////////////////////////////////////////////////////////////////////////////
    //                                                                         //
    // FUNCTION NAME: size                                                     //
    //                                                                         //
    // PURPOSE:       Returns the number of points in the structure.           //
    //                                                                         //
    // SECURITY:      public                                                   //
    //                                                                         //
    // PARAMETERS                                                              //
    //   Type/Name:   Void.                                                    //
    //   Description: None.                                                    //
    //                                                                         //
    // RETURN:        size_t number of points in the structure.                //
    //                                                                         //
    // NOTES:         None.                                                    //
    //                                                                         //
    /////////////////////////////////////////////////////////////////////////////
    size_t size();
  };
}

#endif
