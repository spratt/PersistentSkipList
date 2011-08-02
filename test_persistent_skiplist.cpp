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
#include "PersistentSkipList.h"

using namespace persistent_skip_list;
using namespace timestamped_array;

int main(int argv, char** argc) {
  /////////////////////////////////////////////////////////////////////////////
  // ListNode tests                                                          //
  /////////////////////////////////////////////////////////////////////////////
  cout << "======================================================================\n"
       << "| Testing ListNode                                                   |\n"
       << "======================================================================\n";
  ListNode<int>* ln = new ListNode<int>(2);
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
  ln->setNext(tsa);
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
  if(ln->removeReference() == 0)
    cout << "Structure successfully deleted." << endl;
  else
    assert(false);
  
  /////////////////////////////////////////////////////////////////////////////
  // SkipList tests                                                          //
  /////////////////////////////////////////////////////////////////////////////
  cout << "======================================================================\n"
       << "| Testing SkipList                                                   |\n"
       << "======================================================================\n";
  //PersistentSkipList psl();

  // success
  return 0;
}
