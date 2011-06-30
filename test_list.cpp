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

const int MAX_POINTS_DISPLAY = 16;

void test_list(Point2d* points, int nPoints, int queryIterations, int enumIterations, int nodeSize) {
  Point2d* p = NULL;
  time_t before, after;
  PointPersistentList ppl(nodeSize);
  /////////////////////////////////////////////////////////////////////////////
  // Build Persistent List                                                   //
  /////////////////////////////////////////////////////////////////////////////
  cout << "Building persistent list with node size " << nodeSize << "..." << flush;
  before = time(0);
  ppl.insertPoints(points,nPoints);
  after = time(0);
  cout << "took: " << (after - before) << endl;
  /////////////////////////////////////////////////////////////////////////////
  // Check memory - quietly or manually                                      //
  /////////////////////////////////////////////////////////////////////////////
  cout << "Memory usage: " << ppl.memoryUsage() << " bytes" << endl;
  /////////////////////////////////////////////////////////////////////////////
  // Print the structure if the number of points is small                    //
  /////////////////////////////////////////////////////////////////////////////
  if(nPoints <= MAX_POINTS_DISPLAY) {
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
  cout << queryIterations << " iterations of leftMostNE..." << flush;
  before = time(0);
  for(int i = 0; i < queryIterations; i++)
    p = ppl.leftMostNE(rand() % nPoints, rand() % nPoints);
  after = time(0);
  cout << "took: " << (after-before) << endl;
  /////////////////////////////////////////////////////////////////////////////
  // highestNE                                                               //
  /////////////////////////////////////////////////////////////////////////////
  cout << queryIterations << " iterations of highestNE..." << flush;
  before = time(0);
  for(int i = 0; i < queryIterations; i++)
    p = ppl.highestNE(rand() % nPoints, rand() % nPoints);
  after = time(0);
  cout << "took: " << (after-before) << endl;
  /////////////////////////////////////////////////////////////////////////////
  // enumerateNE                                                             //
  /////////////////////////////////////////////////////////////////////////////
  vector< Point2d > v;
  cout << enumIterations << " iterations of enumerateNE..." << flush;
  before = time(0);
  for(int i = 0; i < enumIterations; i++) {
    int x = rand() % nPoints, y = rand() % nPoints;
    v = ppl.enumerateNE(x,y);
  }
  after = time(0);
  cout << "took: " << (after-before) << endl;
}

int main(int argv, char** argc) {
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
	 << "[enumerate iterations] [max node size]" << endl;
    return 1;
  }
  // parse number of points
  int n = atoi(argc[1]);
  assert(n >= 0);
  // parse query iterations
  int qi = atoi(argc[2]);
  assert(qi >= 0);
  // parse query iterations
  int ei = atoi(argc[3]);
  assert(ei >= 0);
  // parse node size
  int ns = atoi(argc[4]);
  assert(ns >= 2);
  /////////////////////////////////////////////////////////////////////////////
  // Create points                                                           //
  /////////////////////////////////////////////////////////////////////////////
  cout << "Creating " << n << " points..." << flush;
  Point2d* points = new Point2d[n]; // allocate on the heap
  for(int i = 1; i < n; i++) {
    Point2d p(rand() % n, rand() % n); // allocate on the stack
    points[i] = p;
  }
  cout << "done." << endl;
  if(n <= MAX_POINTS_DISPLAY) {
    cout << "Points: ";
    print(points,n);
  }
  /////////////////////////////////////////////////////////////////////////////
  // Run tests                                                               //
  /////////////////////////////////////////////////////////////////////////////
  for(int i = 2; i <= ns; ++i) {
    cout << "============================================================" << endl;
    test_list(points,n,qi,ei,i);
  }
  return 0;
}
