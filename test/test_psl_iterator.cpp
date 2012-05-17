///////////////////////////////////////////////////////////////////////////////
//                       Copyright (c) 2011 - 2012 by                        //
//                                Simon Pratt                                //
//                           (All rights reserved)                           //
///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// FILE:    test_psl_iterator.cpp                                            //
//                                                                           //
// MODULE:  Persistent Skip List                                             //
//                                                                           //
// NOTES:   None.                                                            //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include <iostream>

#include "../PSLIterator.hpp"

int main(int argc, char** argv) {
  // build some list nodes
  SmartPointer<ListNode<int> > tallerNode(new ListNode<int>(1));
  SmartPointer<ListNode<int> > shorterNode(new ListNode<int>(2));
  if(tallerNode->getHeight() < shorterNode->getHeight()) {
    SmartPointer<ListNode<int> > temp = shorterNode;
    shorterNode = tallerNode;
    tallerNode = temp;
  }
  TimeStampedArray<SmartPointer<ListNode<int> > >* tsa
    = new TimeStampedArray<SmartPointer<ListNode<int> > >(0,tallerNode->getHeight());
  for(int i = 0; i < tsa->getSize(); ++i) {
    tsa->setElement(i,tallerNode);
  }
  shorterNode->addNext(tsa);

  cout << "Creating iterator on stack...";
  PSLIterator<int> iter(shorterNode,0);
  cout << "success." << endl;

  assert(*iter == 1 || *iter == 2);
  cout << "First node datum:  " << *iter << endl;

  cout << "Incrementing iterator...";
  ++iter;
  cout << "success." << endl;
  
  assert(*iter == 1 || *iter == 2);
  cout << "Second node datum: " << *iter << endl;
  
  // success
  return 0;
}
