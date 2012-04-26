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
  ListNode<int>* ln = ListNode<int>::create(2);
  cout << "Structure successfully allocated on heap." << endl
       << "Data: " << ln->getData() << endl
       << "Number of change indices: " << ln->numberOfNextChangeIndices() << endl
       << "Number of references to this node: " << ln->addReference() << endl;

  ln->addIncomingNode(0,ln);
  cout << "Structure successfully added to incoming pointers." << endl;
  
  ln->removeIncomingNode(0);
  cout << "Structure successfully removed from incoming pointers." << endl;

  TimeStampedArray<ListNode<int>*>* tsa = new TimeStampedArray<ListNode<int>*>(0,1);
  tsa->setElement(0,ln);
  ln->addNext(tsa);
  cout << "Successfully added an array of next pointers." << endl
       << "Number of change indices: " << ln->numberOfNextChangeIndices() << endl;

  int index = ln->getNextChangeIndex(1);
  cout << "Successfully retrieved a change index." << endl;

  TimeStampedArray<ListNode<int>*>* change = ln->getNextAtIndex(index);
  if(change == tsa)
    cout << "Successfully retrieved the array of next pointers from a change index."
	 << endl;
  else
    assert(false);

  TimeStampedArray<ListNode<int>*>* change2 = ln->getNext(1);
  if(change2 == tsa)
    cout << "Successfully retrieved the array of next pointers directly." << endl;
  else
    assert(false);
  
  // destructor test
  tsa->setElement(0,NULL); // can't delete while a pointer to it exists
  ln->removeReference();
  if(ln->removeReference() == 0)
    cout << "Structure successfully deleted." << endl;
  else
    assert(false);
  ln = NULL;
  
  /////////////////////////////////////////////////////////////////////////////
  // SkipList tests                                                          //
  /////////////////////////////////////////////////////////////////////////////
  cout << "======================================================================\n"
       << "| Testing SkipList                                                   |\n"
       << "======================================================================\n";
  PersistentSkipList<int> psl;
  cout << "SkipList successfully allocated on stack." << endl;
  tsa = psl.getHead(0);
  cout << "Successfully got the head of the skip list." << endl;
  psl.insert(42);
  cout << "Successfully inserted one value into the skip list." << endl;
  psl.insert(8);
  psl.insert(69);
  psl.insert(25);
  cout << "Successfully inserted several values into the skip list." << endl;
  for(int i = 0; i <= psl.getPresent(); ++i)
    psl.draw(i);
  /////////////////////////////////////////////////////////////////////////////
  // Test on advaced data type
  /////////////////////////////////////////////////////////////////////////////
  PersistentSkipList<LineSegment> lines;
  cout << "SkipList<LineSegment> allocated on stack." << endl;
  lines.insert(LineSegment(1,2,3,4));
  cout << "Successfully inserted one structure into the skip list." << endl;
  lines.insert(LineSegment(5,6,7,8));
  lines.insert(LineSegment(9,10,11,12));
  cout << "Successfully inserted several structures into the skip list." << endl;
  for(int i = 0; i <= lines.getPresent(); ++i)
    lines.draw(i);
  // success
  return 0;
}
