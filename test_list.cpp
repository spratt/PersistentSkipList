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
  bool QUIET_MODE = false;
  time_t before, after;
  Point2d* p = NULL;
  int size, n, qi, ei, ns;
  /////////////////////////////////////////////////////////////////////////////
  // Seed the PRNG                                                           //
  /////////////////////////////////////////////////////////////////////////////
  srand( time(0) );
  /////////////////////////////////////////////////////////////////////////////
  // Ensure the user has entered required parameters, otherwise print        //
  // a helpful message.                                                      //
  /////////////////////////////////////////////////////////////////////////////
  if(argv < 5) {
    cout << "Usage: test_list [number of points] [query iterations] "
	 << "[enumerate iterations] [node size] [quiet]" << endl;
    return 1;
  }
  // parse number of points
  n = atoi(argc[1]);
  // parse query iterations
  qi = atoi(argc[2]);
  // parse query iterations
  ei = atoi(argc[3]);
  // parse node size
  ns = atoi(argc[4]);
  // check for quiet mode
  if(argv > 5)
    QUIET_MODE = true;
  PointPersistentList ppl(ns);
  /////////////////////////////////////////////////////////////////////////////
  // Create points                                                           //
  /////////////////////////////////////////////////////////////////////////////
  cout << "Creating " << n << " points..." << flush;
  before = time(0);
  Point2d* points = new Point2d[n]; // allocate on the heap
  for(int i = 1; i < n; i++) {
    Point2d p(rand() % n, rand() % n); // allocate on the stack
    points[i] = p;
  }
  after = time(0);
  cout << "took: " << (after-before) << endl;
  if(n <= MAX_POINTS_DISPLAY) {
    cout << "Points: ";
    print(points,n);
  }
  /////////////////////////////////////////////////////////////////////////////
  // Build Persistent List                                                   //
  /////////////////////////////////////////////////////////////////////////////
  cout << "Building persistent list with node size " << ns << "..." << flush;
  before = time(0);
  ppl.insertPoints(points,n);
  after = time(0);
  cout << "took: " << (after - before) << endl;
  delete points;
  /////////////////////////////////////////////////////////////////////////////
  // Check memory - quietly or manually                                      //
  /////////////////////////////////////////////////////////////////////////////
  if(QUIET_MODE) {
    cout << "Memory usage(%): " << flush;
    // this next line is system-specific, but should work on any
    // reasonable unix compatible system.  Procedure:
    //   1. Ask for detailed information on processes
    //   2. filter out all lines which don't have the name of the program
    //   3. filter out all lines which have the word grep
    //   4. filter out all lines which have the word "ps "
    //   5. print the 4th column (memory usage)
    assert(system("ps auxww | grep test_list | grep -v grep | grep -v ps | awk '{print $4}'") == 0);
  } else {
    control_utilities::waitForAnyKey();
  }
  /////////////////////////////////////////////////////////////////////////////
  // Print the structure if the number of points is small                    //
  /////////////////////////////////////////////////////////////////////////////
  if(n <= MAX_POINTS_DISPLAY) {
    for(int i = 0; i < (int)ppl.size(); i++) {
      cout << i << ": ";
      if(ppl.getList(i) == NULL)
	cout << "NULL";
      else
	ppl.getList(i)->printList(i);
      cout << endl;
    }
  }
  /////////////////////////////////////////////////////////////////////////////
  // leftMostNE                                                              //
  /////////////////////////////////////////////////////////////////////////////
  cout << qi << " iterations of leftMostNE..." << flush;
  before = time(0);
  for(int i = 0; i < qi; i++)
    p = ppl.leftMostNE(rand() % n, rand() % n);
  after = time(0);
  cout << "took: " << (after-before) << endl;
  /////////////////////////////////////////////////////////////////////////////
  // highestNE                                                               //
  /////////////////////////////////////////////////////////////////////////////
  cout << qi << " iterations of highestNE..." << flush;
  before = time(0);
  for(int i = 0; i < qi; i++)
    p = ppl.highestNE(rand() % n, rand() % n);
  after = time(0);
  cout << "took: " << (after-before) << endl;
  /////////////////////////////////////////////////////////////////////////////
  // enumerateNE                                                             //
  /////////////////////////////////////////////////////////////////////////////
  vector< Point2d > v;
  cout << ei << " iterations of enumerateNE" << flush;
  before = time(0);
  for(int i = 0; i < ei; i++) {
    int x = rand() % n, y = rand() % n;
    v = ppl.enumerateNE(x,y);
    if(i == 0 && (int)v.size() <= MAX_POINTS_DISPLAY) {
      cout << "(" << x << "," << y << "): " << flush;
      print(vectorToArray(v),v.size());
    }
  }
  after = time(0);
  size = (int)v.size();
  cout << "...took: " << (after-before) << endl;
  return 0;
}
