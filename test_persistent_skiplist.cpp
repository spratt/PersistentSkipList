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
#include "TimeStampedArray.hpp"
#include "PersistentSkipList.hpp"
#include "LineSegment.hpp"

using namespace persistent_skip_list;
using namespace timestamped_array;
using namespace geometry;

int main(int argv, char** argc) {
  /////////////////////////////////////////////////////////////////////////////
  // ListNode tests                                                          //
  /////////////////////////////////////////////////////////////////////////////
  cout << "======================================================================\n"
       << "| Testing ListNode                                                   |\n"
       << "======================================================================\n";
  SmartPointer<ListNode<int> > ln(ListNode<int>::create(2));
  cout << "Structure successfully allocated on heap." << endl
       << "Data: " << ln->getData() << endl
       << "Number of change indices: " << ln->numberOfNextChangeIndices() << endl;

  TimeStampedArray<SmartPointer<ListNode<int> > >* tsa =
    new TimeStampedArray<SmartPointer<ListNode<int> > >(0,1);
  tsa->setElement(0,ln);
  ln->addNext(tsa);
  cout << "Successfully added an array of next pointers." << endl
       << "Number of change indices: " << ln->numberOfNextChangeIndices() << endl;

  int index = ln->getNextChangeIndex(1);
  cout << "Successfully retrieved a change index." << endl;

  TimeStampedArray<SmartPointer<ListNode<int> > >* change =
    ln->getNextAtIndex(index);
  if(change == tsa)
    cout << "Successfully retrieved the array of next pointers from a change index."
	 << endl;
  else
    assert(false);

  TimeStampedArray<SmartPointer< ListNode<int> > >* change2 =
    ln->getNext(1);
  if(change2 == tsa)
    cout << "Successfully retrieved the array of next pointers directly." << endl;
  else
    assert(false);
  
  /////////////////////////////////////////////////////////////////////////////
  // SkipList tests                                                          //
  /////////////////////////////////////////////////////////////////////////////
  cout << "======================================================================\n"
       << "| Testing SkipList                                                   |\n"
       << "======================================================================\n";

  cout << "Allocating PersistentSkipList<int> on stack...";
  PersistentSkipList<int> psl;
  cout << "success." << endl;

  cout << "Getting head...";
  tsa = psl.getHead(0);
  cout << "success." << endl;

  cout << "Inserting one value...";
  psl.insert(42);
  cout << "success." << endl;

  cout << "Drawing skip list at present time..." << endl;
  psl.drawPresent();
  cout << endl;
  
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

  cout << "Drawing all times of skiplist..." << endl;
  for(int i = 0; i <= psl.getPresent(); ++i)
    psl.draw(i);
  cout << endl;
  
  /////////////////////////////////////////////////////////////////////////////
  // Test on advaced data type
  /////////////////////////////////////////////////////////////////////////////
  cout << "Allocating PersistentSkipList<LineSegment> on stack...";
  PersistentSkipList<LineSegment> lines;
  cout << "success." << endl;

  cout << "Inserting one structure into the skip list...";
  lines.insert(LineSegment(1,2,3,4));
  cout << "success." << endl;

  cout << "Inserting several structures into the skip list...";
  lines.insert(LineSegment(5,6,7,8));
  lines.insert(LineSegment(9,10,11,12));
  cout << "success." << endl;

  cout << "Drawing skip list at time 0..." << endl;
  lines.drawPresent();

  cout << "Incrementing time...";
  lines.incTime();
  cout << "success." << endl;

  cout << "Inserting several structures into the skip list at time 1...";
  lines.insert(LineSegment(13,14,15,16));
  lines.insert(LineSegment(17,18,19,20));
  cout << "success." << endl;

  cout << "Drawing all times of skip list..." << endl;
  for(int i = 0; i <= lines.getPresent(); ++i)
    lines.draw(i);
  
  // success
  return 0;
}
