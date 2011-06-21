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
  PointPersistentList ppl;
  vector< Point2d > v;
  Point2d* p = NULL;
  int size, type, n, qi;
  /////////////////////////////////////////////////////////////////////////////
  // Seed the PRNG                                                           //
  /////////////////////////////////////////////////////////////////////////////
  srand( time(0) );
  /////////////////////////////////////////////////////////////////////////////
  // Ensure the user has entered required parameters, otherwise print        //
  // a helpful message.                                                      //
  /////////////////////////////////////////////////////////////////////////////
  if(argv < 4) {
    cout << "Usage: test_list"
	 << " [number of points] [order of point insertion] [query iterations]"
	 << " [quiet]"
	 << endl << endl
	 << "Orders available for insertion:" << endl
	 << "   0: x and y increasing, each point is NE of each previous point"
	 << endl
	 << "   1: x increasing, y decreasing, each point is SE of previous"
	 << endl
	 << "   2: x decreasing, y increasing, each point is NW of previous"
	 << endl
	 << "   3: x and y decreasing, each point is SW of previous"
	 << endl
	 << "   4: x and y random"
	 << endl;
    return 1;
  }
  // parse number of points
  n = atoi(argc[1]);
  // parse order of point insertion
  type = atoi(argc[2]);
  // parse query iterations
  qi = atoi(argc[3]);
  // check for quiet mode
  if(argv > 4)
    QUIET_MODE = true;
  /////////////////////////////////////////////////////////////////////////////
  // insert points                                                           //
  /////////////////////////////////////////////////////////////////////////////
  cout << "Inserting " << n << " points..." << flush;
  before = time(0);
  for(int i = 0; i < n; i++) {
    switch(type) {
    case 0:
      // x increasing
      // y increasing
      ppl.insertPoint(i,i);
      break;
    case 1:
      // x increasing
      // y decreasing
      ppl.insertPoint(i,n-i-1);
      break;
    case 2:
      // x decreasing
      // y increasing
      ppl.insertPoint(n-i-1,i);
      break;
    case 3:
      // x decreasing
      // y decreasing
      ppl.insertPoint(n-i-1,n-i-1);
      break;
    case 4:
      // x random
      // y random
      ppl.insertPoint(rand() % n, rand() % n);
      break;
    default:
      assert(false);
    }
  }
  after = time(0);
  cout << "took: " << (after-before) << endl;
  if(n <= MAX_POINTS_DISPLAY) {
    cout << "Points: "; ppl.printArray();
  }
  /////////////////////////////////////////////////////////////////////////////
  // Wait for user input                                                     //
  /////////////////////////////////////////////////////////////////////////////
  if(QUIET_MODE) {
    cout << "Memory usage(%): " << flush;
    assert(system("ps auxww | grep test_list | grep -v grep | grep -v ps | awk '{print $4}'") == 0);
  } else {
    control_utilities::waitForAnyKey();
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
  // enumerateNE                                                             //
  /////////////////////////////////////////////////////////////////////////////
  cout << qi << " iterations of enumerateNE..." << flush;
  before = time(0);
  for(int i = 0; i < qi; i++)
    v = ppl.enumerateNE(rand() % n, rand() % n);
  after = time(0);
  size = (int)v.size();
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
  // leftMostNE                                                              //
  /////////////////////////////////////////////////////////////////////////////
  cout << qi << " iterations of leftMostNE..." << flush;
  before = time(0);
  for(int i = 0; i < qi; i++)
    p = ppl.leftMostNE(rand() % n, rand() % n);
  after = time(0);
  cout << "took: " << (after-before) << endl;
  return 0;
}
