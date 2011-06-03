//-*- mode: c++ -*-////////////////////////////////////////////////////////////
//                       Copyright (c) 2011 - 2012 by                        //
//                                Simon Pratt                                //
//                           (All rights reserved)                           //
///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// FILE:    array_utilities.h                                                //
//                                                                           //
// MODULE:  None.                                                            //
//                                                                           //
// PURPOSE: Provides useful functions for working with arrays.               //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
//                             Public Methods:                               //
// T* vectorToArray(vector<T> v)                                             //
// void print(T* points, int nPoints)                                        //
// void print(T* points, int begin, int end)                                 //
// void swap(T* points, int a, int b)                                        //
// void copy(T* a, T* b, int n)                                              //
// void copy(T* a, T* b, int begin, int end)                                 //
// void reverse(T* points, int begin, int end)                               //
// void block_permute(T* points, int beginA, int endA, int endB)             //
///////////////////////////////////////////////////////////////////////////////
#ifndef ARRAY_UTILITIES_H
#define ARRAY_UTILITIES_H
#include <iostream>
#include <vector>

namespace array_utilities {
  // given a vector, returns the address of the inner array
  template <class T>
  T* vectorToArray(std::vector<T>& v) {
    // C++ doesn't really do encapsulation
    return &(v[0]);
  }
  // print the n first points in a given array
  template <class T>
  void print(T* points, int nPoints) {
    printRange(points,0,nPoints-1);
  }
  // prints an array of points in order from begin to end
  // N.B. can't overload a function template
  template <class T>
  void printRange(T* points, int begin, int end) {
    std::cout << "{ " << points[begin];
    for(int i = begin+1; i <= end; i++)
      std::cout << " " << points[i];
    std::cout << " }" << std::endl;
  }
  // given an array, swaps point at index a with point at index b
  template <class T>
  void swap(T* points, int a, int b) {
    T temp = points[a];
    points[a] = points[b];
    points[b] = temp;
  }
  // copies first n points in array a to array b
  template <class T>
  void copy(T* a, T* b, int n) {
    copyRange(a,b,0,n-1);
  }
  // copies points in array a to array b between begin and end (inclusive)
  // N.B. can't overload a function template
  template <class T>
  void copyRange(T* a, T* b, int begin, int end) {
    end++;
    while(--end >= begin) b[end] = a[end];
  }
  // reverses a subarray from begin to end
  template <class T>
  void reverse(T* points, int begin, int end) {
    while(begin < end)
      swap(points,begin++,end--);
  }
  // given an array with 2 abutting blocks, one from beginA to endA,
  // the other from endA+1 to endB, swaps the order of the blocks in
  // O(n) time with O(1) extra space
  template <class T>
  void block_permute(T* points, int beginA, int endA, int endB) {
    if(beginA == endB) return;
    reverse(points,beginA,endA);
    reverse(points,endA +1, endB);
    reverse(points,beginA,endB);
  }
}

#endif
