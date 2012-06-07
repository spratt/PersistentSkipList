///////////////////////////////////////////////////////////////////////////////
//                       Copyright (c) 2011 - 2012 by                        //
//                                Simon Pratt                                //
//                           (All rights reserved)                           //
///////////////////////////////////////////////////////////////////////////////
// 
// FILE:    ListNode.hpp
// 
// MODULE:  
// 
// PURPOSE: None.
// 
// NOTES:   None.
// 
///////////////////////////////////////////////////////////////////////////////
// Public Variable:                     Description:                         //
// ----------------                     ------------                         //
///////////////////////////////////////////////////////////////////////////////
//                             Public Methods:                               //
///////////////////////////////////////////////////////////////////////////////
#ifndef LISTNODE_HPP
#define LISTNODE_HPP

// Standard libraries
#include <vector>
#include <cstddef>
#include <cstdlib>

// My libraries
#include "TimeStampedArray.hpp"
#include "lib/SmartPointer/SmartPointer.hpp"

namespace persistent_skip_list {

  template <class T>
  class ListNode {
  public:
    // hereafter refered to as TSA
    typedef TimeStampedArray< SmartPointer< ListNode<T> > > TSA;

    ///////////////////////////////////////////////////////////////////////////
    //                                                                       //
    // FUNCTION NAME: ListNode                                               //
    //                                                                       //
    // PURPOSE:       Basic constructor                                      //
    //                                                                       //
    // SECURITY:      public                                                 //
    //                                                                       //
    // PARAMETERS                                                            //
    //   Type/Name:   T/original_data                                        //
    //   Description: The value to store in the data of the ListNode         //
    //                                                                       //
    // NOTES:         None.                                                  //
    //                                                                       //
    ///////////////////////////////////////////////////////////////////////////
    ListNode(const T&, int size=3);

    ///////////////////////////////////////////////////////////////////////////
    //                                                                       //
    // FUNCTION NAME: ListNode                                               //
    //                                                                       //
    // PURPOSE:       Dummy node constructor                                 //
    //                                                                       //
    // SECURITY:      public                                                 //
    //                                                                       //
    // PARAMETERS                                                            //
    //   Type/Name:   int/h                                                  //
    //   Description: The height of this node.                               //
    //                                                                       //
    //   Type/Name:   bool/positive                                          //
    //   Description: True for positive infinity, false for negative         //
    //                infinity.                                              //
    //                                                                       //
    // NOTES:         None.                                                  //
    //                                                                       //
    ///////////////////////////////////////////////////////////////////////////
    ListNode(int h, const bool positive);
    
    ///////////////////////////////////////////////////////////////////////////
    //                                                                       //
    // FUNCTION NAME: ~ListNode                                              //
    //                                                                       //
    // PURPOSE:       Destructor                                             //
    //                                                                       //
    // SECURITY:      public                                                 //
    //                                                                       //
    // PARAMETERS                                                            //
    //   Type/Name:   Void.                                                  //
    //   Description: None.                                                  //
    //                                                                       //
    // RETURN:        Void.                                                  //
    //                                                                       //
    // NOTES:         None.                                                  //
    //                                                                       //
    ///////////////////////////////////////////////////////////////////////////
    ~ListNode();

    ///////////////////////////////////////////////////////////////////////////
    //                                                                       //
    // FUNCTION NAME: setHeight                                              //
    //                                                                       //
    // PURPOSE:       Changes the height of a dummy node.                    //
    //                                                                       //
    // SECURITY:      public                                                 //
    //                                                                       //
    // PARAMETERS                                                            //
    //   Type/Name:   int/h                                                  //
    //   Description: The new height of the dummy node.                      //
    //                                                                       //
    // RETURN:        Void.                                                  //
    //                                                                       //
    // NOTES:         Must be greater than previous height.                  //
    //                                                                       //
    ///////////////////////////////////////////////////////////////////////////
    void setHeight(int h);
    
    ///////////////////////////////////////////////////////////////////////////
    //                                                                       //
    // FUNCTION NAME: getData                                                //
    //                                                                       //
    // PURPOSE:       Returns the stored data at this node.                  //
    //                                                                       //
    // SECURITY:      public                                                 //
    //                                                                       //
    // PARAMETERS                                                            //
    //   Type/Name:   Void.                                                  //
    //   Description: None.                                                  //
    //                                                                       //
    // RETURN:                                                               //
    //   Type/Name:   T                                                      //
    //   Description: The data stored at this node.                          //
    //                                                                       //
    // NOTES:         None.                                                  //
    //                                                                       //
    ///////////////////////////////////////////////////////////////////////////
    T getData();

    ///////////////////////////////////////////////////////////////////////////
    //                                                                       //
    // FUNCTION NAME: getHeight                                              //
    //                                                                       //
    // PURPOSE:       Returns the height of the node.                        //
    //                                                                       //
    // SECURITY:      public                                                 //
    //                                                                       //
    // PARAMETERS                                                            //
    //   Type/Name:   Void.                                                  //
    //   Description: None.                                                  //
    //                                                                       //
    // RETURN:                                                               //
    //   Type/Name:   int                                                    //
    //   Description: The height of the node.                                //
    //                                                                       //
    // NOTES:         None.                                                  //
    //                                                                       //
    ///////////////////////////////////////////////////////////////////////////
    int getHeight();

    ///////////////////////////////////////////////////////////////////////////
    //                                                                       //
    // FUNCTION NAME: getNextChangeIndex                                     //
    //                                                                       //
    // PURPOSE:       Given a time t, gives the change index of the nearest  //
    //                time when the next pointers were changed.              //
    //                                                                       //
    // SECURITY:      public                                                 //
    //                                                                       //
    // PARAMETERS                                                            //
    //   Type/Name:   int/t                                                  //
    //   Description: The time for which to search.                          //
    //                                                                       //
    // RETURN:        The index of the change of next pointers nearest to the//
    //                given time.                                            //
    //                                                                       //
    // NOTES:         None.                                                  //
    //                                                                       //
    ///////////////////////////////////////////////////////////////////////////
    int getNextChangeIndex(int t);

    ///////////////////////////////////////////////////////////////////////////
    //                                                                       //
    // FUNCTION NAME: getNextAtIndex                                         //
    //                                                                       //
    // PURPOSE:       Returns the next pointers at a given change index.     //
    //                                                                       //
    // SECURITY:      public                                                 //
    //                                                                       //
    // PARAMETERS                                                            //
    //   Type/Name:   int/ci                                                 //
    //   Description: The change index at which to retrieve the change       //
    //                structure.                                             //
    //                                                                       //
    // RETURN:                                                               //
    //   Type/Name:   TSA*                                                   //
    //   Description: The                                                    //
    //                                                                       //
    // NOTES:         None.                                                  //
    //                                                                       //
    ///////////////////////////////////////////////////////////////////////////
    TSA* getNextAtIndex(int ci);

    ///////////////////////////////////////////////////////////////////////////
    //                                                                       //
    // FUNCTION NAME: numberOfNextChangeIndices                              //
    //                                                                       //
    // PURPOSE:       Returns the number of change indices, i.e. the number  //
    //                of times this node's next pointer has been changed and //
    //                stored.                                                //
    //                                                                       //
    // SECURITY:      public                                                 //
    //                                                                       //
    // PARAMETERS                                                            //
    //   Type/Name:   Void.                                                  //
    //   Description: None.                                                  //
    //                                                                       //
    // RETURN:                                                               //
    //   Type/Name:   int                                                    //
    //   Description: The number of times the next pointers have been        //
    //                changed.                                               //
    //                                                                       //
    // NOTES:         None.                                                  //
    //                                                                       //
    ///////////////////////////////////////////////////////////////////////////
    int numberOfNextChangeIndices();

    ///////////////////////////////////////////////////////////////////////////
    //                                                                       //
    // FUNCTION NAME: getNext                                                //
    //                                                                       //
    // PURPOSE:       Retrieves the array of next pointers at the time       //
    //                at or immediately preceding the given time.            //
    //                                                                       //
    // SECURITY:      public                                                 //
    //                                                                       //
    // PARAMETERS                                                            //
    //   Type/Name:   int/t                                                  //
    //   Description: The time at which to retrieve the pointer.             //
    //                                                                       //
    // RETURN:                                                               //
    //   Type/Name:   TSA*                                                   //
    //   Description: The array of next pointers at or immediately           //
    //                preceding the given time.                              //
    //                                                                       //
    // NOTES:         None.                                                  //
    //                                                                       //
    ///////////////////////////////////////////////////////////////////////////
    TSA* getNext(int t);

    ///////////////////////////////////////////////////////////////////////////
    //                                                                       //
    // FUNCTION NAME: addNext                                                //
    //                                                                       //
    // PURPOSE:       Set the next pointer                                   //
    //                                                                       //
    // SECURITY:      public                                                 //
    //                                                                       //
    // PARAMETERS                                                            //
    //   Type/Name:   ListNode<T>*/ln                                        //
    //   Description: The pointer to which to assign next.                   //
    //                                                                       //
    // RETURN:        int return code.  0 means success                      //
    //                                                                       //
    // NOTES:         None.                                                  //
    //                                                                       //
    ///////////////////////////////////////////////////////////////////////////
    int addNext(TSA* next);

    ///////////////////////////////////////////////////////////////////////////
    //                                                                       //
    // FUNCTION NAME: operator<                                              //
    //                                                                       //
    // PURPOSE:       Compares the data at this node to the data of          //
    //                another node to determine which should come first.     //
    //                                                                       //
    // SECURITY:      public                                                 //
    //                                                                       //
    // PARAMETERS                                                            //
    //   Type/Name:   ListNode<T>&/other                                     //
    //   Description: The other ListNode to which to compare this one.       //
    //                                                                       //
    // RETURN:        bool -: true if this node precedes the other           //
    //                                                                       //
    // NOTES:         None.                                                  //
    //                                                                       //
    ///////////////////////////////////////////////////////////////////////////
    bool operator<(ListNode<T>& other);
    bool operator>(ListNode<T>& other);
    bool operator<=(ListNode<T>& other);
    bool operator>=(ListNode<T>& other);

    bool operator<(const T& datum);
    bool operator>(const T& datum);
    bool operator<=(const T& datum);
    bool operator>=(const T& datum);

    ///////////////////////////////////////////////////////////////////////////
    //                                                                       //
    // FUNCTION NAME: setIncoming                                            //
    //                                                                       //
    // PURPOSE:       Sets the pointer to the incoming node at height        //
    //                h to ListNode in.                                      //
    //                                                                       //
    // SECURITY:      public                                                 //
    //                                                                       //
    // PARAMETERS                                                            //
    //   Type/Name:   int/h                                                  //
    //   Description: The height at which to set the incoming node.          //
    //                                                                       //
    //   Type/Name:   ListNode<T>*/in                                        //
    //   Description: The incoming node at height h.                         //
    //                                                                       //
    // RETURN:        Void.                                                  //
    //                                                                       //
    // NOTES:         None.                                                  //
    //                                                                       //
    ///////////////////////////////////////////////////////////////////////////
    void setIncoming(int h, ListNode<T>* in);

    ///////////////////////////////////////////////////////////////////////////
    //                                                                       //
    // FUNCTION NAME: getIncoming                                            //
    //                                                                       //
    // PURPOSE:       Gets the incoming node at height h.                    //
    //                                                                       //
    // SECURITY:      public                                                 //
    //                                                                       //
    // PARAMETERS                                                            //
    //   Type/Name:   int/h                                                  //
    //   Description: The height at which to get.                            //
    //                                                                       //
    // RETURN:                                                               //
    //   Type/Name:   ListNode<T>*                                           //
    //   Description: The node which points to this node at height h.        //
    //                                                                       //
    // NOTES:         None.                                                  //
    //                                                                       //
    ///////////////////////////////////////////////////////////////////////////
    ListNode<T>* getIncoming(int h);

    ///////////////////////////////////////////////////////////////////////////
    //                                                                       //
    // FUNCTION NAME: isPositiveInfinity                                     //
    //                                                                       //
    // PURPOSE:       Returns true if this is a dummy node indicating        //
    //                positive infinity.                                     //
    //                                                                       //
    // SECURITY:      public                                                 //
    //                                                                       //
    // PARAMETERS                                                            //
    //   Type/Name:   Void.                                                  //
    //   Description: None.                                                  //
    //                                                                       //
    // RETURN:        bool - true if this is a positive infinity dummy node. //
    //                                                                       //
    // NOTES:         None.                                                  //
    //                                                                       //
    ///////////////////////////////////////////////////////////////////////////
    bool isPositiveInfinity();
    bool isNegativeInfinity();  // same but for negative infinity

    void remove(int);
    
    ///////////////////////////////////////////////////////////////////////////
    // Kindly ignore my private parts                                        //
    ///////////////////////////////////////////////////////////////////////////
  private:
    int height;
    unsigned int size;
    vector<TSA*> next;
    T data;
    static bool _SEEDED; // must be initialized to false
    bool _isPositiveInfinity;
    bool _isNegativeInfinity;

    ListNode<T>** incoming_nodes;

    static void seed();
    void initializeNode();
  };
}

#include "ListNode.cpp"
#endif
