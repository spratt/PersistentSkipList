///////////////////////////////////////////////////////////////////////////////
//                       Copyright (c) 2011 - 2012 by                        //
//                                Simon Pratt                                //
//                           (All rights reserved)                           //
///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// FILE:    test_persistent_skiplist.cpp                                     //
//                                                                           //
// MODULE:  Persistent Skip List                                             //
//                                                                           //
// NOTES:   None.                                                            //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include "../TimeStampedArray.hpp"
#include "../PersistentSkipList.hpp"

using namespace persistent_skip_list;
using namespace timestamped_array;

void printBar() {
  cout << "======================================================================"
       << endl;
}

int main(int argv, char** argc) {
  /////////////////////////////////////////////////////////////////////////////
  // Test on int                                                             //
  /////////////////////////////////////////////////////////////////////////////
  
  cout << "Allocating PersistentSkipList<int> on stack...";
  PersistentSkipList<int> psl;
  cout << "success." << endl;
  psl.drawPresent();
  printBar();

  cout << "Inserting one value...";
  psl.insert(42);
  cout << "success." << endl;
  psl.drawPresent();
  printBar();
  
  cout << "Inserting several values at time 0...";
  psl.insert(8);
  psl.insert(69);
  psl.insert(25);
  cout << "success." << endl;
  psl.drawPresent();
  printBar();
  
  cout << "Incrementing time...";
  psl.incTime();
  cout << "success." << endl;

  cout << "Inserting several values at time 1...";
  psl.insert(53);
  psl.insert(17);
  cout << "success." << endl;
  psl.drawPresent();
  printBar();
  
  cout << "Incrementing time...";
  psl.incTime();
  cout << "success." << endl;

  cout << "Inserting several values at time 2...";
  psl.insert(7);
  psl.insert(72);
  cout << "success." << endl;
  psl.drawPresent();
  printBar();
  
  cout << "Incrementing time...";
  psl.incTime();
  cout << "success." << endl;

  cout << "Removing a value at time 3...";
  PSLIterator<int> found = psl.find(72,3);
  assert(found.getDatum() == 72);
  found.remove();
  cout << "success." << endl;

  cout << "Drawing all times of skiplist..." << endl;
  for(int i = 0; i <= psl.getPresent(); ++i) {
    psl.draw(i);
    printBar();
  }

  found = psl.find(72,0);
  cout << "Querying for 72 at time 0, found: " << *found << endl;
  found = psl.find(72,1);
#ifndef NDEBUG
  cout << "\t Search path:";
  for(vector<int>::iterator it = psl.lastSearchPath.begin();
      it != psl.lastSearchPath.end();
      ++it) {
    cout << *it << ", ";
  }
  cout << endl;
#endif
  cout << "Querying for 72 at time 1, found: " << *found << endl;
  found = psl.find(72,2);
#ifndef NDEBUG
  cout << "\t Search path:";
  for(vector<int>::iterator it = psl.lastSearchPath.begin();
      it != psl.lastSearchPath.end();
      ++it) {
    cout << *it << ", ";
  }
  cout << endl;
#endif
  cout << "Querying for 72 at time 2, found: " << *found << endl;
  found = psl.find(72,3);
  cout << "Querying for 72 at time 3, found: " << *found << endl;

  found = psl.find(8,0);
  cout << "Querying for 8 at time 0, found: " << *found << endl;
  found = psl.find(8,1);
  cout << "Querying for 8 at time 1, found: " << *found << endl;
  found = psl.find(8,2);
  cout << "Querying for 8 at time 2, found: " << *found << endl;
  found = psl.find(8,3);
  cout << "Querying for 8 at time 3, found: " << *found << endl;

  found = psl.find(17,0);
  cout << "Querying for 17 at time 0, found: " << *found << endl;
#ifndef NDEBUG
  cout << "\t Search path:";
  for(vector<int>::iterator it = psl.lastSearchPath.begin();
      it != psl.lastSearchPath.end();
      ++it) {
    cout << *it << ", ";
  }
  cout << endl;
#endif
  found = psl.find(17,1);
  cout << "Querying for 17 at time 1, found: " << *found << endl;
#ifndef NDEBUG
  cout << "\t Search path:";
  for(vector<int>::iterator it = psl.lastSearchPath.begin();
      it != psl.lastSearchPath.end();
      ++it) {
    cout << *it << ", ";
  }
  cout << endl;
#endif
  found = psl.find(17,2);
  cout << "Querying for 17 at time 2, found: " << *found << endl;
  found = psl.find(17,3);
  cout << "Querying for 17 at time 3, found: " << *found << endl;

  found = psl.find(10,0);
  cout << "Querying for 10 at time 0, found: " << *found << endl;
  found = psl.find(10,1);
  cout << "Querying for 10 at time 1, found: " << *found << endl;
  found = psl.find(10,2);
  cout << "Querying for 10 at time 2, found: " << *found << endl;
  found = psl.find(10,3);
  cout << "Querying for 10 at time 3, found: " << *found << endl;

  // success
  return 0;
}
