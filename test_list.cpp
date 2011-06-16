///////////////////////////////////////////////////////////////////////////////
//                       Copyright (c) 2011 - 2012 by                        //
//                                Simon Pratt                                //
//                           (All rights reserved)                           //
///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// FILE:    test_list.cpp                                                    //
//                                                                           //
// MODULE:  Persistent List                                                  //
//                                                                           //
// NOTES:   None.                                                            //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include "PersistentList.h"
#include "PointPersistentList.h"
#include "array_utilities.h"
#include "control_utilities.h"

using namespace std;
using namespace array_utilities;
using namespace persistent_list;

int main(int argv, char** argc) {
  const int MAX_POINTS_DISPLAY = 16;
  time_t before, after;
  PointPersistentList ppl;
  vector< Point2d > v;
  Point2d* p;
  int size, type, n, qi;
  /////////////////////////////////////////////////////////////////////////////
  // Ensure the user has entered required parameters, otherwise print        //
  // a helpful message.                                                      //
  /////////////////////////////////////////////////////////////////////////////
  if(argv < 4) {
    cout << "Usage: test_list"
	 << " [number of points] [order of point insertion] [query iterations]"
	 << endl << endl
	 << "Orders available for insertion:" << endl
	 << "   0: x and y increasing, each point is NE of each previous point"
	 << endl
	 << "   1: x increasing, y decreasing, each point is SE of previous"
	 << endl
	 << "   2: x decreasing, y increasing, each point is NW of previous"
	 << endl
	 << "   3: x and y decreasing, each point is SW of previous"
	 << endl;
    return 1;
  }
  // parse number of points
  n = atoi(argc[1]);
  // parse order of point insertion
  type = atoi(argc[2]);
  // parse query iterations
  qi = atoi(argc[3]);
  /////////////////////////////////////////////////////////////////////////////
  // insert points                                                           //
  /////////////////////////////////////////////////////////////////////////////
  before = time(0);
  for(int i = 0; i < n; i++) {
    switch(type) {
    case 0:
      ppl.insertPoint(i,i);
      break;
    case 1:
      ppl.insertPoint(i,n-i-1);
      break;
    case 2:
      ppl.insertPoint(n-i-1,i);
      break;
    case 3:
      ppl.insertPoint(n-i-1,n-i-1);
    default:
      assert(false);
    }
  }
  after = time(0);
  cout << "Inserting " << n << " points took: " << (after-before) << endl;
  control_utilities::waitForAnyKey();
  if(n <= MAX_POINTS_DISPLAY) {
    cout << "Points: "; ppl.printArray();
  }
  /////////////////////////////////////////////////////////////////////////////
  // Print the structure if the number of points is small                    //
  /////////////////////////////////////////////////////////////////////////////
  if(n <= MAX_POINTS_DISPLAY) {
    for(int i = 0; i < n; i++) {
      cout << i << ": ";
      if(ppl.getList(i) == NULL)
	cout << "NULL";
      else
	ppl.getList(i)->printList(i);
      cout << endl;
    }
  }
  /////////////////////////////////////////////////////////////////////////////
  // enumerateNE(0,0)                                                        //
  /////////////////////////////////////////////////////////////////////////////
  before = time(0);
  for(int i = 0; i < qi; i++)
    v = ppl.enumerateNE(0,0);
  after = time(0);
  size = (int)v.size();
  cout << qi << " iterations of ";
  cout << "enumerateNE(0,0) returned " << size << " points"
       <<" and took: " << (after-before) << endl;
  if(size > 0 && size <= MAX_POINTS_DISPLAY) {
    cout << "Found: "; print(vectorToArray(v),size);
  }
  /////////////////////////////////////////////////////////////////////////////
  // enumerateNE(n/2,n/2)                                                    //
  /////////////////////////////////////////////////////////////////////////////
  before = time(0);
  for(int i = 0; i < qi; i++)
    v = ppl.enumerateNE(n/2,n/2);
  after = time(0);
  size = (int)v.size();
  cout << qi << " iterations of ";
  cout << "enumerateNE(" << n/2 << "," << n/2 << ")"
       << " returned " << size << " points"
       <<" and took: " << (after-before) << endl;
  if(size > 0 && size <= MAX_POINTS_DISPLAY) {
    cout << "Found: "; print(vectorToArray(v),size);
  }
  /////////////////////////////////////////////////////////////////////////////
  // enumerateNE(n,n)                                                        //
  /////////////////////////////////////////////////////////////////////////////
  before = time(0);
  for(int i = 0; i < qi; i++)
    v = ppl.enumerateNE(n,n);
  after = time(0);
  size = (int)v.size();
  cout << qi << " iterations of ";
  cout << "enumerateNE(" << n << "," << n << ")"
       << " returned " << size << " points"
       <<" and took: " << (after-before) << endl;
  if(size > 0 && size <= MAX_POINTS_DISPLAY) {
    cout << "Found: "; print(vectorToArray(v),size);
  }
  /////////////////////////////////////////////////////////////////////////////
  // highestNE(0,0)                                                          //
  /////////////////////////////////////////////////////////////////////////////
  before = time(0);
  for(int i = 0; i < qi; i++)
    p = ppl.highestNE(0,0);
  after = time(0);
  cout << qi << " iterations of ";
  cout << "highestNE(0,0) took: " << (after-before) << endl;
  cout << "Found: " << *p << endl;
  /////////////////////////////////////////////////////////////////////////////
  // highestNE(n/2,n/2)                                                      //
  /////////////////////////////////////////////////////////////////////////////
  before = time(0);
  for(int i = 0; i < qi; i++)
    p = ppl.highestNE(n/2,n/2);
  after = time(0);
  cout << qi << " iterations of ";
  cout << "highestNE(" << n/2 << "," << n/2 << ") took: "
       << (after-before) << endl
       << "Found: " << *p << endl;
  /////////////////////////////////////////////////////////////////////////////
  // highestNE(n,n)                                                          //
  /////////////////////////////////////////////////////////////////////////////
  before = time(0);
  for(int i = 0; i < qi; i++)
    p = ppl.highestNE(n,n);
  after = time(0);
  cout << qi << " iterations of ";
  cout << "highestNE(" << n << "," << n << ") took: "
       << (after-before) << endl
       << "Found: " << *p << endl;
  /////////////////////////////////////////////////////////////////////////////
  // leftMostNE(0,0)                                                         //
  /////////////////////////////////////////////////////////////////////////////
  before = time(0);
  for(int i = 0; i < qi; i++)
    p = ppl.leftMostNE(0,0);
  after = time(0);
  cout << qi << " iterations of ";
  cout << "leftMostNE(0,0) took: "
       << (after-before) << endl
       << "Found: " << *p << endl;
  /////////////////////////////////////////////////////////////////////////////
  // leftMostNE(n/2,n/2)                                                     //
  /////////////////////////////////////////////////////////////////////////////
  before = time(0);
  for(int i = 0; i < qi; i++)
    p = ppl.leftMostNE(n/2,n/2);
  after = time(0);
  cout << qi << " iterations of ";
  cout << "leftMostNE(" << n/2 << "," << n/2 << ") took: "
       << (after-before) << endl
       << "Found: " << *p << endl;
  /////////////////////////////////////////////////////////////////////////////
  // leftMostNE(n,n)                                                         //
  /////////////////////////////////////////////////////////////////////////////
  before = time(0);
  for(int i = 0; i < qi; i++)
    p = ppl.leftMostNE(n,n);
  after = time(0);
  cout << qi << " iterations of ";
  cout << "leftMostNE(" << n << "," << n << ") took: "
       << (after-before) << endl
       << "Found: " << *p << endl;
  return 0;
}
