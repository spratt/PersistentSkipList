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
#include "PersistentList.h"

using namespace std;
using namespace persistent_list;

int main(int argv, char** argc) {
  time_t before, after;
  PersistentList p;
  before = time(0);
  p.insertPoint(2,1);
  p.insertPoint(1,2);
  p.insertPoint(4,3);
  p.insertPoint(3,4);
  after = time(0);
  cout << "Inserting points took: " << (after-before) << endl;
  cout << "Points: "; p.printArray();
  return 0;
}
