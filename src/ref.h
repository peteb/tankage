/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#ifndef REF_H_CKS8593L
#define REF_H_CKS8593L

#include <stdexcept>
#include "config.h"

#ifdef HAVE_TR1_MEMORY_H
  #include <tr1/memory>
  #define WEAK_PTR std::tr1::weak_ptr
  #define SHARED_PTR std::tr1::shared_ptr

#elif defined(HAVE_BOOST)
  #include <boost/shared_ptr.hpp>
  #include <boost/weak_ptr.hpp>
  #define WEAK_PTR boost::weak_ptr
  #define SHARED_PTR boost::shared_ptr

#else
  #error "No smart pointer implementation found"
#endif




template<typename T>
class Ref {
public:
   typedef SHARED_PTR<T> SharedPtr;
   typedef WEAK_PTR<T> WeakPtr;

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
   Ref<T> & operator = (const Ref<OtherT> & other) {
	  this->sp = other.sp;
	  this->wp = other.wp;
	  owner = other.owner;
   }
	
   // and just for convenience...
   SharedPtr operator -> () const {
	  SharedPtr ret = lock();
	  if (!ret)
		 throw std::runtime_error("null pointer exception!");
	  
	  return ret;
   }

   template<typename Other>
   friend class Ref<T>;
   
   // friend class WeakRef<T>;
   // friend class OwningRef<T>;
   
protected:
	bool owner;
	SharedPtr sp;
	WeakPtr wp;
};

template<typename T, typename Source>
typename Ref<T>::SharedPtr Cast(const Source & source) {
#ifdef HAVE_TR1_MEMORY_H
   return std::tr1::dynamic_pointer_cast<T>(source);
#elif defined(HAVE_BOOST)
   return boost::dynamic_pointer_cast<T>(source);
#else
#error "Can't cast, no smartptr implementer"
#endif
}


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


#endif /* end of include guard: REF_H_CKS8593L */
