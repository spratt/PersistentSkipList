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
  p.insertPoint(1,2);
  after = time(0);
  cout << "Took: " << (after-before) << endl;
  return 0;
}
