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

int main(int argv, char** argc) {
  /////////////////////////////////////////////////////////////////////////////
  // Test on int                                                             //
  /////////////////////////////////////////////////////////////////////////////
  
  cout << "Allocating PersistentSkipList<int> on stack...";
  PersistentSkipList<int> psl;
  cout << "success." << endl;
  psl.drawPresent();

  cout << "Inserting one value...";
  psl.insert(42);
  cout << "success." << endl;
  psl.drawPresent();
  
  cout << "Inserting several values at time 0...";
  psl.insert(8);
  psl.insert(69);
  psl.insert(25);
  cout << "success." << endl;
  psl.drawPresent();
  
  cout << "Incrementing time...";
  psl.incTime();
  cout << "success." << endl;

  cout << "Inserting several values at time 1...";
  psl.insert(53);
  psl.insert(17);
  cout << "success." << endl;
  psl.drawPresent();
  
  cout << "Incrementing time...";
  psl.incTime();
  cout << "success." << endl;

  cout << "Inserting several values at time 2...";
  psl.insert(7);
  psl.insert(72);
  cout << "success." << endl;

  cout << "Drawing all times of skiplist..." << endl;
  for(int i = 0; i <= psl.getPresent(); ++i)
    psl.draw(i);

  cout << "Querying for 72 at time 0, found: " << *(psl.find(72,0)) << endl;
  cout << "Querying for 72 at time 1, found: " << *(psl.find(72,1)) << endl;
  cout << "Querying for 72 at time 2, found: " << *(psl.find(72,2)) << endl;
  
  // success
  return 0;
}
