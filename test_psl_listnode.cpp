///////////////////////////////////////////////////////////////////////////////
//                       Copyright (c) 2011 - 2012 by                        //
//                                Simon Pratt                                //
//                           (All rights reserved)                           //
///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// FILE:    test_psl_listnode.cpp                                            //
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
  
  cout << "Allocating ListNode<int> on stack...";
  SmartPointer<ListNode<int> > ln1(new ListNode<int>(1));
  SmartPointer<ListNode<int> > ln2(new ListNode<int>(2));
  cout << "success." << endl;

  cout << "Number of change indices: "
       << ln1->numberOfNextChangeIndices() << endl;

  cout << "Allocating TimeStampedArray<SmartPointer<ListNode<int> > > on stack...";
  TimeStampedArray<SmartPointer<ListNode<int> > >* tsa
    = new TimeStampedArray<SmartPointer<ListNode<int> > >(0,1);
  cout << "success." << endl;

  cout << "Setting element at index 0 on tsa...";
  tsa->setElement(0,ln2);
  cout << "success." << endl;

  cout << "Adding next to list node...";
  ln1->addNext(tsa);
  cout << "success." << endl;

  cout << "Number of change indices: "
       << ln1->numberOfNextChangeIndices() << endl;

  cout << "Retrieving a change index...";
  int index = ln1->getNextChangeIndex(1);
  cout << "success." << endl;

  cout << "Getting next pointer at given index...";
  TimeStampedArray<SmartPointer<ListNode<int> > >* change =
    ln1->getNextAtIndex(index);
  assert(change == tsa);
  cout << "success." << endl;

  cout << "Getting next pointer at index+1...";
  TimeStampedArray<SmartPointer< ListNode<int> > >* change2 =
    ln1->getNext(1);
  assert(change2 == tsa);
  cout << "success." << endl;

  delete tsa;
  
  // success
  return 0;
}
