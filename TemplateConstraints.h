//-*- mode: c++ -*-////////////////////////////////////////////////////////////
//                       Copyright (c) 2011 - 2012 by                        //
//                                Simon Pratt                                //
//                           (All rights reserved)                           //
///////////////////////////////////////////////////////////////////////////////
//                                                                           //
// FILE:    TemplateConstraints.h                                            //
//                                                                           //
// MODULE:  Template Constraints                                             //
//                                                                           //
// NOTES:   Implements (sort of) constraints for C++'s templates.            //
//                                                                           //
//          Simply instantiate one of these structs in the constructor       //
//          of the class template, and it should give reasonable             //
//          errors when the constraints aren't met.                          //
//                                                                           //
//          These are taken directly from Bjarne Stroustrup at:              //
//          http://www2.research.att.com/~bs/bs_faq2.html#constraints        //
//                                                                           //
//          Example:                                                         //
//                                                                           //
//          MyClass<T>::MyClass() {                                          //
//            Can_compare<T>; // should fail at compile time if T            //
//                            // doesn't implement comparison operators      //
//          }                                                                //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////
#ifndef TEMPLATECONSTRAINTS_H
#define TEMPLATECONSTRAINTS_H

template<class T, class B> struct Derived_from {
  static void constraints(T* p) { B* pb = p; }
  Derived_from() { void(*p)(T*) = constraints; }
};

template<class T1, class T2> struct Can_copy {
  static void constraints(T1 a, T2 b) { T2 c = a; b = a; }
  Can_copy() { void(*p)(T1,T2) = constraints; }
};

template<class T1, class T2 = T1> struct Can_compare {
  static void constraints(T1 a, T2 b) { a==b; a!=b; a<b; }
  Can_compare() { void(*p)(T1,T2) = constraints; }
};

template<class T1, class T2, class T3 = T1> struct Can_multiply {
  static void constraints(T1 a, T2 b, T3 c) { c = a*b; }
  Can_multiply() { void(*p)(T1,T2,T3) = constraints; }
};

#endif
