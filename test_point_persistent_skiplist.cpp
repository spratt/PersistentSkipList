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
#include "PointPersistentSkipList.hpp"
#include "array_utilities.hpp"
#include "control_utilities.hpp"

using namespace std;
using namespace array_utilities;
using namespace persistent_skip_list;

const int MAX_POINTS_DISPLAY = 16;

void test_list(Point2D* points, int nPoints, int queryIterations,
	       int enumIterations, int nodeSize) {
  time_t before, after;
  Point2D* p;
  PointPersistentSkipList ppsl;
  /////////////////////////////////////////////////////////////////////////////
  // Build Persistent List                                                   //
  /////////////////////////////////////////////////////////////////////////////
  cout << "Building persistent list with node size " << nodeSize
       << "..." << flush;
  before = time(0);
  // add points
  after = time(0);
  cout << "took: " << (after - before) << endl;
  /////////////////////////////////////////////////////////////////////////////
  // Check memory - quietly or manually                                      //
  /////////////////////////////////////////////////////////////////////////////
  //cout << "Memory usage: " << ppsl.memoryUsage() << " bytes" << endl;
  /////////////////////////////////////////////////////////////////////////////
  // Print the structure if the number of points is small                    //
  /////////////////////////////////////////////////////////////////////////////
  if(nPoints <= MAX_POINTS_DISPLAY) {
    cout << "Array: " << flush;
    //ppsl.printArray();
    // for(int i = 0; i < (int)ppsl.size(); i++) {
    //   cout << i << ": ";
    //   if(ppsl.getList(i) == NULL)
    // 	cout << "NULL";
    //   else
    // 	ppsl.getList(i)->printList(i);
    //   cout << endl;
    // }
  }
  /////////////////////////////////////////////////////////////////////////////
  // leftMostNE                                                              //
  /////////////////////////////////////////////////////////////////////////////
  cout << queryIterations << " iterations of leftMostNE..." << flush;
  before = time(0);
  for(int i = 0; i < queryIterations; i++)
    ppsl.leftMostNE(rand() % nPoints, rand() % nPoints);
  after = time(0);
  cout << "took: " << (after-before) << endl;
  if(nPoints <= MAX_POINTS_DISPLAY) {
    int max_x = rand() % nPoints;
    int max_y = rand() % nPoints;
    cout << "leftMostNE(" << max_x << "," << max_y << "): " << flush;
    p = ppsl.leftMostNE(max_x, max_y);
    if(p == NULL)
      cout << "NULL";
    else
      cout << *p;
    cout << endl;
  }
  /////////////////////////////////////////////////////////////////////////////
  // highestNE                                                               //
  /////////////////////////////////////////////////////////////////////////////
  cout << queryIterations << " iterations of highestNE..." << flush;
  before = time(0);
  for(int i = 0; i < queryIterations; i++)
    ppsl.highestNE(rand() % nPoints, rand() % nPoints);
  after = time(0);
  cout << "took: " << (after-before) << endl;
  if(nPoints <= MAX_POINTS_DISPLAY) {
    int max_x = rand() % nPoints;
    int max_y = rand() % nPoints;
    cout << "highestNE(" << max_x << "," << max_y << "): " << flush;
    p = ppsl.highestNE(max_x, max_y);
    if(p == NULL)
      cout << "NULL";
    else
      cout << *p;
    cout << endl;
  }
  /////////////////////////////////////////////////////////////////////////////
  // enumerateNE                                                             //
  /////////////////////////////////////////////////////////////////////////////
  vector< Point2D > v;
  cout << enumIterations << " iterations of enumerateNE..." << flush;
  before = time(0);
  for(int i = 0; i < enumIterations; i++) {
    ppsl.enumerateNE(rand() % nPoints, rand() % nPoints);
  }
  after = time(0);
  cout << "took: " << (after-before) << endl;
  if(nPoints <= MAX_POINTS_DISPLAY) {
    int max_x = rand() % nPoints;
    int max_y = rand() % nPoints;
    cout << "enumerateNE(" << max_x << "," << max_y << "): ";
    v = ppsl.enumerateNE(max_x,max_y);
    print(vectorToArray(v),v.size());
  }
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
  if(argv < 6) {
    cout << "Usage: test_list [number of points] [query iterations] "
	 << "[enumerate iterations] [min node size] [max node size]" << endl;
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
  int min_ns = atoi(argc[4]);
  int max_ns = atoi(argc[5]);
  /////////////////////////////////////////////////////////////////////////////
  // Create points                                                           //
  /////////////////////////////////////////////////////////////////////////////
  cout << "Creating " << n << " points..." << flush;
  Point2D* points = new Point2D[n]; // allocate on the heap
  for(int i = 1; i < n; i++) {
    Point2D p(rand() % n, rand() % n); // allocate on the stack
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
  for(int i = min_ns; i <= max_ns; ++i) {
    cout << "============================================================"
	 << endl;
    test_list(points,n,qi,ei,i);
  }
  return 0;
}
