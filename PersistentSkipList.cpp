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
