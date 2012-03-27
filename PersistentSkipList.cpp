///////////////////////////////////////////////////////////////////////////////
//                       Copyright (c) 2011 - 2012 by                        //
//                                Simon Pratt                                //
//                           (All rights reserved)                           //
///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// FILE:    PersistentSkipList.cpp                                           //
//                                                                           //
// MODULE:  Persistent Skip List                                             //
//                                                                           //
// NOTES:   None.                                                            //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
#ifndef PERSISTENTSKIPLIST_CPP
#define PERSISTENTSKIPLIST_CPP

#include "PersistentSkipList.hpp"

using namespace persistent_skip_list;

///////////////////////////////////////////////////////////////////////////////
// ListNode Implementation                                                   //
///////////////////////////////////////////////////////////////////////////////

template<class T>
bool ListNode<T>::_SEEDED = false;

template<class T>
ListNode<T>* ListNode<T>::create(const T& original_data) {
  return new ListNode<T>(original_data);
}

template<class T>
T ListNode<T>::getData() {
  assert(this != NULL);
  return data;
}

template<class T>
int ListNode<T>::getHeight() {
  assert(this != NULL);
  return height;
}
  
template<class T>
int ListNode<T>::getNextChangeIndex(int t) {
  assert(this != NULL);
  int index = -1;
  int begin = 0, end = numberOfNextChangeIndices() -1;
  int timeFound = 0;
  // binary search
  while(begin <= end) {
    index = (begin+end)/2;
    timeFound = getNextAtIndex(index)->getTime();
    if(timeFound == t) {
      // done, break out of loop
      break;
    } else if(timeFound > t) {
      // repeat binary search on left (earlier) half
      end = index -1;
    } else {
      // repeat binary search on right (later) half
      begin = index +1;
    }
  }
  // report final index
  return index;
}

template <class T>
TimeStampedArray<ListNode<T>*>* ListNode<T>::getNextAtIndex(int ci) {
  assert(this != NULL);
  assert(ci >= 0);
  assert(ci < numberOfNextChangeIndices());
  return next[ci];
}

template <class T>
int ListNode<T>::numberOfNextChangeIndices() {
  assert(this != NULL);
  return (int)next.size();
}
  
template <class T>
TimeStampedArray<ListNode<T>*>* ListNode<T>::getNext(int t) {
  assert(this != NULL);
  assert(t >= 0);
  // find nearest time
  int index = getNextChangeIndex(t);
  // if there are no pointers, bail
  if(index == -1) return NULL;
  // use this to store the node to be returned
  TimeStampedArray<ListNode<T>*>* tsa = getNextAtIndex(index);
  // since the nearest time might be later than the desired time, we
  // go backwards in time until we reach the point which immediately
  // precedes the query time
  while(tsa->getTime() > t) {
    --index;
    if(index < 0)
      return NULL;
    tsa = getNextAtIndex(index);
  }
  return tsa;
}
  
template <class T>
ListNode<T>* ListNode<T>::getNext(int t, int h) {
  assert(this != NULL);
  assert(t >= 0);
  TimeStampedArray<ListNode<T>*>* tsa = getNext(t);
  if(tsa == NULL)
    return NULL;
  assert(h < tsa->getSize());
  return tsa->getElement(h);
}

template <class T>
int ListNode<T>::addNext(TimeStampedArray<ListNode<T>*>* tsa) {
  assert(this != NULL);
  // since NULL is the default
  if(tsa == NULL)
    return -1; // bail if trying to set next to NULL
  // make sure time is strictly increasing
  int lastIndex = (int)next.size()-1;
  if(lastIndex >= 0)
    assert(tsa->getTime() > next[lastIndex]->getTime());
  // finally, save the new set of next pointers
  next.push_back(tsa);
  // make sure to count the references
  for(int i = 0; i < tsa->getSize(); ++i) {
    ListNode<T>* ln = tsa->getElement(i);
    if(ln != NULL)
      ln->addReference();
  }
  // success
  return 0;
}

template <class T>
int ListNode<T>::addIncomingNode(int h, ListNode<T>* in) {
  assert(this != NULL);
  assert(h >= 0);
  assert(h < height);
  assert(in_nodes[h] == NULL);
  in_nodes[h] = in;
  // Note that incoming node references are not counted, this is on purpose.
  // If incoming references are the only ones left, the node should
  // be deleted.
  // success
  return 0;
}

template <class T>
ListNode<T>* ListNode<T>::getIncomingNode(int h) {
  assert(this != NULL);
  assert(h >= 0);
  assert(h < height);
  return in_nodes[h];
}

template <class T>
int ListNode<T>::removeIncomingNode(int h) {
  assert(this != NULL);
  assert(h >= 0);
  assert(h < height);
  if(in_nodes[h] != NULL) {
    // Note that incoming node references are not counted, this is on purpose.
    // If incoming references are the only ones left, the node should
    // be deleted.
    in_nodes[h] = NULL;
  }
  // success
  return 0;
}

template <class T>
int ListNode<T>::addReference() {
  assert(this != NULL);
  return ++references;
}

template <class T>
int ListNode<T>::removeReference() {
  assert(this != NULL);
  assert(references > 0);
  int toReturn = --references;
  if(PSL_DEBUG_MODE) {
    clog << "Node " << data << " has " << references << " references." << endl;
  }
  if(references == 0) {
    if(PSL_DEBUG_MODE) {
      clog << "Deleting " << data << "..." << endl;
    }
    delete this;
  }
  return toReturn;
}

///////////////////////////////////////////////////////////////////////////////
// PersistentSkipList Implementation                                         //
///////////////////////////////////////////////////////////////////////////////

template <class T>
int PersistentSkipList<T>::getPresent() {
  assert(this != NULL);
  return data_set.size()-1;
}

template <class T>
void PersistentSkipList<T>::drawPresent() {
  assert(this != NULL);
  draw(getPresent());
}

template <class T>
void PersistentSkipList<T>::draw(int t) {
  assert(this != NULL);
  assert(t >= 0);
  cout << "Getting skip list at time " << t << "..." << endl;
  TimeStampedArray<ListNode<T>*>* head = getHead(t);
  if(head == NULL) {
    cout << "NULL" << endl;
    return;
  }
  vector<int> heights;
  vector<T> data;
  ListNode<T>* ln = head->getElement(0);
  int max_height = -1;
  while(ln != NULL) {
    int h = ln->getHeight();
    heights.push_back(h);
    if(h > max_height)
      max_height = h;
    data.push_back(ln->getData());
    ln = ln->getNext(t,0);
  }
  --max_height;
  while(max_height >= 0) {
    cout << max_height << ": ";
    for(int i = 0; i < (int)heights.size(); ++i) {
      cout << setw(3);
      if(heights[i] > max_height) {
	cout << data[i];
      } else {
	cout << "   ";
      }
      cout << " -> ";
    }
    cout << "NULL" << endl;
    --max_height;
  }
}

template <class T>
int PersistentSkipList<T>::addHead(TimeStampedArray<ListNode<T>*>* tsa) {
  assert(this != NULL);
  assert(tsa != NULL);
  // save the new head
  head.push_back(tsa);
  // make sure to count the references
  for(int i = 0; i < tsa->getSize(); ++i) {
    ListNode<T>* ln = tsa->getElement(i);
    if(ln != NULL)
      ln->addReference();
  }
  // success
  return 0;
}

template <class T>
TimeStampedArray<ListNode<T>*>* PersistentSkipList<T>::getHead(int t) {
  assert(this != NULL);
  int index = -1;
  int begin = 0, end = head.size() -1;
  int timeFound = 0;
  // binary search
  while(begin <= end) {
    index = (begin+end)/2;
    timeFound = head[index]->getTime();
    if(timeFound == t) {
      // done, break out of loop
      break;
    } else if(timeFound > t) {
      // repeat binary search on left (earlier) half
      end = index -1;
    } else {
      // repeat binary search on right (later) half
      begin = index +1;
    }
  }
  // closest may be after given time
  while(index > 0 && head[index]->getTime() > t)
    --index;
  if(index < 0)
    return NULL;
  // finally, return the found element
  return head[index];
}

/////////////////////////////////////////////////////////////////////////////
// INSERT METHOD                                                           //
/////////////////////////////////////////////////////////////////////////////
template <class T>
int PersistentSkipList<T>::insert(const T& data) {
  assert(this != NULL);
  TimeStampedArray<ListNode<T>*>* new_head = NULL;
  // check if data exists already
  if(data_set.count(data)>0)
    return 1;
  // otherwise, create node
  ListNode<T>* new_ln = ListNode<T>::create(data);
  int height = new_ln->getHeight();
  if(PSL_DEBUG_MODE) {
    clog << "New node (" << data << ") height: " << height << endl;
  }
  ///////////////////////////////////////////////////////////////////////////
  // FIRST ELEMENT CASE                                                    //
  ///////////////////////////////////////////////////////////////////////////
  if(data_set.empty()) {
    // initialize head
    new_head = new TimeStampedArray<ListNode<T>*>(0, height);
    // make the new node the head at all heights
    for(int i = 0; i < height; ++i) {
      new_head->setElement(i,new_ln);
    }
  }
  ///////////////////////////////////////////////////////////////////////////
  // NOT FIRST ELEMENT CASE                                                //
  ///////////////////////////////////////////////////////////////////////////
  else {
    // add node to list
    int last_time = getPresent();
    int new_time = last_time +1;
    TimeStampedArray<ListNode<T>*>* old_head = getHead(last_time);
    int start = height-1;
    /////////////////////////////////////////////////////////////////////////
    // TALLER THAN OLD HEAD                                                //
    /////////////////////////////////////////////////////////////////////////
    if(height > old_head->getSize()) {
      if(PSL_DEBUG_MODE) {
	clog << "Node " << data << " is taller than old head." << endl;
      }
      new_head = new TimeStampedArray<ListNode<T>*>(new_time,height,*old_head);
      // make the new node the head at all heights exceeding the size of
      // the old head
      while(start >= old_head->getSize()) {
	if(PSL_DEBUG_MODE) {
	  clog << "Creating head pointers for old node at height: "
	       << start << endl;
	}
	new_head->setElement(start,new_ln);
	--start;
      }
    }
    /////////////////////////////////////////////////////////////////////////
    // NOT TALLER THAN OLD HEAD                                            //
    /////////////////////////////////////////////////////////////////////////
    else {
      // copy the old head
      int head_size = old_head->getSize();
      new_head =
	new TimeStampedArray<ListNode<T>*>(new_time,head_size,*old_head);
    }
    TimeStampedArray<ListNode<T>*>* new_node_next =
      new TimeStampedArray<ListNode<T>*>(new_time,height);
    /////////////////////////////////////////////////////////////////////////
    // ADD TO HEAD IF NEEDED                                               //
    /////////////////////////////////////////////////////////////////////////
    ListNode<T>* old_ln = old_head->getElement(start);
    // travel down the heads, adding the new node until we find a head
    // node which precedes the new node
    while(data < old_ln->getData()) {
      if(PSL_DEBUG_MODE) {
	clog << "Linking new to old at height " << start << endl;
      }
      old_ln->addIncomingNode(start,new_ln);
      new_node_next->setElement(start,old_ln);
      new_head->setElement(start,new_ln);
      --start;
      if(start < 0)
	break;
      old_ln = old_head->getElement(start);
    }
    /////////////////////////////////////////////////////////////////////////
    // ADD TO REST OF LIST IF NEEDED                                       //
    /////////////////////////////////////////////////////////////////////////
    if(start >= 0) {
      int search_height = start;
      // guaranteed not NULL
      ListNode<T>* old_ln = old_head->getElement(search_height);
      while(search_height >= 0) {
	// might be NULL
	ListNode<T>* next_ln = old_ln->getNext(last_time,search_height);
	while(next_ln != NULL && new_ln->getData() > next_ln->getData()) {
	  old_ln = next_ln;
	  next_ln = old_ln->getNext(last_time,search_height);
	}
	// add node to preceding node
	TimeStampedArray<ListNode<T>*>* old_ln_next =
	  old_ln->getNext(last_time);  // <- this could be NULL
	int old_ln_height = old_ln->getHeight();
	if(old_ln_next == NULL) {
	  old_ln_next =
	    new TimeStampedArray<ListNode<T>*>(new_time,old_ln_height);
	} else {
	  old_ln_next =
	    new TimeStampedArray<ListNode<T>*>(new_time,
					       old_ln_height,
					       *old_ln_next);
	}
	while(next_ln == NULL || new_ln->getData() < next_ln->getData()) {
	  // point the new node to the old next node
	  if(next_ln != NULL) {
	    next_ln->removeIncomingNode(search_height);
	    next_ln->addIncomingNode(search_height,new_ln);
	    new_node_next->setElement(search_height,next_ln);
	  }
	  // point the old node to the new node
	  new_ln->removeIncomingNode(search_height);
	  new_ln->addIncomingNode(search_height,old_ln);
	  old_ln_next->setElement(search_height,new_ln);
	  // move to the next height
	  --search_height;
	  if(search_height < 0)
	    break;
	  next_ln = old_ln_next->getElement(search_height);
	}
	old_ln_next->lock();
	old_ln->addNext(old_ln_next);
	// move to next search height
	if(search_height < 0)
	  break;
	// get node at new height
	old_ln = next_ln;
      }
    }
    new_ln->addNext(new_node_next);
  }
  ///////////////////////////////////////////////////////////////////////////
  // COMMON TO BOTH FIRST AND REST CASES                                   //
  ///////////////////////////////////////////////////////////////////////////
  // since we created a new head, lock it then add it to the array of heads
  new_head->lock();
  addHead(new_head);
  // prevent duplicates by registering this datum
  data_set.insert(data);
  // success
  return 0;
}

#endif
