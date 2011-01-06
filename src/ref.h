//===- ref.h - Reference class definition -----------------------*- c++ -*-===//
//
//                                   Snail Wail
// 
// (c) Copyright 2010 Peter Backman. All Rights Reserved. 
//
//===----------------------------------------------------------------------===//
//
// This file declares the Ref class and helper functions.
//
//===----------------------------------------------------------------------===//


#ifndef REF_H_CKS8593L
#define REF_H_CKS8593L

#include <stdexcept>
#include "config.h"

#ifdef HAVE_TR1_SMARTPTRS
#include <tr1/memory>
#define WEAK_PTR std::tr1::weak_ptr
#define SHARED_PTR std::tr1::shared_ptr
#define DYNAMIC_PTR_CAST std::tr1::dynamic_pointer_cast

#elif HAVE_TR1_IN_STD_SMARTPTRS
#include <memory>
#define WEAK_PTR std::tr1::weak_ptr
#define SHARED_PTR std::tr1::shared_ptr
#define DYNAMIC_PTR_CAST std::tr1::dynamic_pointer_cast

#elif HAVE_STD_SMARTPTRS
#include <memory>
#define WEAK_PTR std:::weak_ptr
#define SHARED_PTR std::shared_ptr
#define DYNAMIC_PTR_CAST std::dynamic_pointer_cast

#elif defined(HAVE_BOOST) // TODO: HAVE_BOOST_SMARTPTRS?
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#define WEAK_PTR boost::weak_ptr
#define SHARED_PTR boost::shared_ptr
#define DYNAMIC_PTR_CAST boost::dynamic_pointer_cast

#else
#error "No smart pointer implementation found"
#endif



/// A class that can, at run-time, decide whether it wants to share the object
/// it's pointing at (shared_ptr), or just observe it (weak_ptr.)
template<typename T>
class Ref {
public:
  typedef SHARED_PTR<T> SharedPtr;
  typedef WEAK_PTR<T> WeakPtr;

  template<typename FriendT> friend class Ref;

  Ref() {owner = false; }
  Ref(const WeakPtr & wp) {
    setWeak(wp);
  }

  template<typename Other>
  Ref(const Ref<Other> & ref) {
    sp = ref.sp;
    wp = ref.wp;
    owner = ref.owner;
  }


  void setWeak(const WeakPtr & wp) {
    this->wp = wp;
    this->sp.reset();
    owner = false;
  }

  void setOwning(const SharedPtr & sp) {
    this->sp = sp;
    this->wp.reset();
    owner = true;
  }

  SharedPtr lock() const {
    if (owner)
      return sp;
    else
      return wp.lock();
  }


  template<typename OtherT>
  Ref<T> & operator = (const Ref<OtherT> & other)
  {
    this->sp = other.sp;
    this->wp = other.wp;
    owner = other.owner;
	return *this;
  }

  operator bool() const {
    return lock();
  }
  
  // and just for convenience...
  SharedPtr operator -> () const
  {
    SharedPtr ret = lock();
    if (!ret)
      throw std::runtime_error("null pointer exception!");
	  
    return ret;
  }

   
  SharedPtr operator * () const
  {
    SharedPtr ret = lock();
    if (!ret)
      throw std::runtime_error("null pointer exception!");
	  
    return ret;
  }
   
  static Ref<T> Null() {
    Ref<T> ret;
    ret.setOwning(SharedPtr());
    return ret;
  }
   
protected:
  bool owner;
  SharedPtr sp;
  WeakPtr wp;
};

/// Downcasts a reference, similar to dynamic_cast.
template<typename T, typename Source>
typename Ref<T>::SharedPtr Cast(const Source & source) {
  return DYNAMIC_PTR_CAST<T>(source);
}

/// Creates an observing reference; when the object pointed at is deleted, the
/// reference will be invalidated.
template<typename T>
Ref<typename T::element_type> Observing(const T & sp) {
  Ref<typename T::element_type> ret;
  ret.setWeak(sp);
  return ret;
}

template<typename T>
Ref<T> Observing(const Ref<T> & ref) {
  Ref<T> ret;
  ret.setWeak(ref.lock());
  return ret;
}

/// Creates a reference that shares the object pointed at; it will live as long
/// as the reference lives.
template<typename T>
Ref<typename T::element_type> Owning(const T & sp) {
  Ref<typename T::element_type> ret;
  ret.setOwning(sp);
  return ret;
}

template<typename T>
Ref<T> Owning(T * ptr) {
  Ref<T> ret;
  ret.setOwning(typename Ref<T>::SharedPtr(ptr));
  return ret;
}

#undef SHARED_PTR
#undef WEAK_PTR
#undef DYNAMIC_POINTER_CAST

#endif /* end of include guard: REF_H_CKS8593L */
