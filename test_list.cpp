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
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include "PersistentList.h"
#include "array_utilities.h"

using namespace std;
using namespace array_utilities;
using namespace persistent_list;

char waitForEnter() {
  cout << "Press any key to continue..." << flush;
  int ch;
  struct termios oldt;
  struct termios newt;
  tcgetattr(STDIN_FILENO, &oldt); // store old settings
  newt = oldt; // copy old settings to new settings
  newt.c_lflag &= ~(ICANON | ECHO); // make one change to old settings in new settings
  tcsetattr(STDIN_FILENO, TCSANOW, &newt); // apply the new settings immediatly
  ch = getchar(); // standard getchar call
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt); // reapply the old settings
  cout << endl;
  return ch; // return received char
}

int main(int argv, char** argc) {
  const int MAX_POINTS_DISPLAY = 16;
  time_t before, after;
  PersistentList pl;
  /////////////////////////////////////////////////////////////////////////////
  // insert points                                                           //
  /////////////////////////////////////////////////////////////////////////////
  int n = 1000;
  if(argv > 1)
    n = atoi(argc[1]);
  before = time(0);
  for(int i = n; i > 0; --i)
    pl.insertPoint(i,i);
  after = time(0);
  cout << "Inserting " << n << " points took: " << (after-before) << endl;
  waitForEnter();
  if(n > 0 && n <= MAX_POINTS_DISPLAY) {
    cout << "Points: "; pl.printArray();
  }
  /////////////////////////////////////////////////////////////////////////////
  // debug                                                                   //
  /////////////////////////////////////////////////////////////////////////////
  for(int i = 0; i < n; i++)
    cout << i << ": " << *(pl.getListAtTime(i)) << endl;
  /////////////////////////////////////////////////////////////////////////////
  // enumerateNE                                                             //
  /////////////////////////////////////////////////////////////////////////////
  before = time(0);
  vector< Point2d > v = pl.enumerateNE(0,0);
  after = time(0);
  n = (int)v.size();
  cout << "Enumerating " << n << " points took: " << (after-before) << endl;
  if(n > 0 && n <= MAX_POINTS_DISPLAY) {
    cout << "Found: "; print(vectorToArray(v),n);
  }
  return 0;
}
