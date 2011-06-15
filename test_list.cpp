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
  int type = 0;
  /////////////////////////////////////////////////////////////////////////////
  // insert points                                                           //
  /////////////////////////////////////////////////////////////////////////////
  int n = 1000;
  if(argv > 1)
    n = atoi(argc[1]);
  if(argv > 2)
    type = atoi(argc[2]);
  if(n <1)
    return 1;
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
      break;
    case 4:
      ppl.insertPoint(i,n/2);
      break;
    case 5:
      ppl.insertPoint(n/2,i);
      break;
    case 6:
      ppl.insertPoint(n/2,n/2);
      break;
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
  // debug                                                                   //
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
  vector< Point2d > v = ppl.enumerateNE(0,0);
  after = time(0);
  int size = (int)v.size();
  cout << "enumerateNE(0,0) returned " << size << " points"
       <<" and took: " << (after-before) << endl;
  if(size > 0 && size <= MAX_POINTS_DISPLAY) {
    cout << "Found: "; print(vectorToArray(v),size);
  }
  /////////////////////////////////////////////////////////////////////////////
  // enumerateNE(n/2,n/2)                                                    //
  /////////////////////////////////////////////////////////////////////////////
  before = time(0);
  v = ppl.enumerateNE(n/2,n/2);
  after = time(0);
  size = (int)v.size();
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
  v = ppl.enumerateNE(n,n);
  after = time(0);
  size = (int)v.size();
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
  Point2d* p = ppl.highestNE(0,0);
  after = time(0);
  cout << "highestNE(0,0) took: " << (after-before) << endl;
  cout << "Found: " << *p << endl;
  /////////////////////////////////////////////////////////////////////////////
  // highestNE(n/2,n/2)                                                      //
  /////////////////////////////////////////////////////////////////////////////
  before = time(0);
  p = ppl.highestNE(n/2,n/2);
  after = time(0);
  cout << "highestNE(" << n/2 << "," << n/2 << ") took: "
       << (after-before) << endl
       << "Found: " << *p << endl;
  /////////////////////////////////////////////////////////////////////////////
  // highestNE(n,n)                                                          //
  /////////////////////////////////////////////////////////////////////////////
  before = time(0);
  p = ppl.highestNE(n,n);
  after = time(0);
  cout << "highestNE(" << n << "," << n << ") took: "
       << (after-before) << endl
       << "Found: " << *p << endl;
  return 0;
}
