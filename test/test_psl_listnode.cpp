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
#include "../TimeStampedArray.hpp"
#include "../PersistentSkipList.hpp"

using namespace persistent_skip_list;
using namespace timestamped_array;

int main(int argv, char** argc) {
  
  cout << "Allocating ListNode<int> on stack...";
  SmartPointer<ListNode<int> > tallerNode(new ListNode<int>(1));
  SmartPointer<ListNode<int> > shorterNode(new ListNode<int>(2));
  if(tallerNode->getHeight() < shorterNode->getHeight()) {
    SmartPointer<ListNode<int> > temp = shorterNode;
    shorterNode = tallerNode;
    tallerNode = temp;
  }
  SmartPointer<ListNode<int> > lnPos(new ListNode<int>(true,tallerNode->getHeight()));
  SmartPointer<ListNode<int> > lnNeg(new ListNode<int>(false,tallerNode->getHeight()));
  cout << "success." << endl;

  cout << "Number of change indices: ";
  cout << shorterNode->numberOfNextChangeIndices() << endl;

  cout << "Testing operator<...";
  assert(*shorterNode   < *lnPos);
  assert(*tallerNode    < *lnPos);
  assert(*lnNeg         < *lnPos);
  assert(*lnNeg         < *shorterNode);  
  assert(*lnNeg         < *tallerNode);
  cout << "success." << endl;

  cout << "Testing operator>...";
  assert(*shorterNode   > *lnNeg);
  assert(*tallerNode    > *lnNeg);
  assert(*lnPos         > *lnNeg);
  assert(*lnPos         > *shorterNode);
  assert(*lnPos         > *tallerNode);
  cout << "success." << endl;

  cout << "Allocating TimeStampedArray<SmartPointer<ListNode<int> > > on stack...";
  TimeStampedArray<SmartPointer<ListNode<int> > >* tsa
    = new TimeStampedArray<SmartPointer<ListNode<int> > >(0,tallerNode->getHeight());
  cout << "success." << endl;

  cout << "Setting next element on tsa...";
  for(int i = 0; i < tsa->getSize(); ++i) {
    tsa->setElement(i,tallerNode);
  }
  cout << "success." << endl;

  cout << "Adding next to list node...";
  shorterNode->addNext(tsa);
  cout << "success." << endl;

  cout << "Number of change indices: ";
  cout << shorterNode->numberOfNextChangeIndices() << endl;

  cout << "Retrieving a change index...";
  int index = shorterNode->getNextChangeIndex(1);
  cout << "success." << endl;

  cout << "Getting next pointer at given index...";
  TimeStampedArray<SmartPointer<ListNode<int> > >* change =
    shorterNode->getNextAtIndex(index);
  assert(change == tsa);
  cout << "success." << endl;

  cout << "Getting next pointer at index+1...";
  TimeStampedArray<SmartPointer< ListNode<int> > >* change2 =
    shorterNode->getNext(1);
  assert(change2 == tsa);
  cout << "success." << endl;

  // success
  return 0;
}
