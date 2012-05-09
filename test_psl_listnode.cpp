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
  
  // success
  return 0;
}
