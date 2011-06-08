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
  bool worst = false;
  /////////////////////////////////////////////////////////////////////////////
  // insert points                                                           //
  /////////////////////////////////////////////////////////////////////////////
  int n = 1000;
  if(argv > 1)
    n = atoi(argc[1]);
  if(argv > 2)
    worst = true;
  if(n <1)
    return 1;
  before = time(0);
  for(int i = n; i > 0; --i) {
    if(worst)
      ppl.insertPoint(i,n-i);
    else
      ppl.insertPoint(i,i);
  }
  after = time(0);
  cout << "Inserting " << n << " points took: " << (after-before) << endl;
  control_utilities::waitForAnyKey();
  if(n <= MAX_POINTS_DISPLAY) {
    cout << "Points: "; ppl.printArray();
  }
  /////////////////////////////////////////////////////////////////////////////
  // debug                                                                   //
  /////////////////////////////////////////////////////////////////////////////
  if(n <= MAX_POINTS_DISPLAY) {
    for(int i = 0; i < n; i++)
      cout << i << ": " << *(ppl.getList(i)) << endl;
  }
  /////////////////////////////////////////////////////////////////////////////
  // enumerateNE                                                             //
  /////////////////////////////////////////////////////////////////////////////
  before = time(0);
  vector< Point2d > v = ppl.enumerateNE(0,0);
  after = time(0);
  n = (int)v.size();
  cout << "Enumerating " << n << " points took: " << (after-before) << endl;
  if(n > 0 && n <= MAX_POINTS_DISPLAY) {
    cout << "Found: "; print(vectorToArray(v),n);
  }
  return 0;
}
