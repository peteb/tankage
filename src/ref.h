/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#ifndef REF_H_CKS8593L
#define REF_H_CKS8593L

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <stdexcept>

template<typename T>
class Ref {
public:
   Ref() {owner = false; }
   Ref(const boost::weak_ptr<T> & wp) {
      setWeak(wp);
   }

	template<typename Other>
	Ref(const Ref<Other> & ref) {
	   sp = ref.sp;
      wp = ref.wp;
      owner = ref.owner;
   }

   typedef boost::shared_ptr<T> SharedPtr;

   void setWeak(const boost::weak_ptr<T> & wp) {
      this->wp = wp;
      this->sp.reset();
      owner = false;
   }

	void setOwning(const boost::shared_ptr<T> & sp) {
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
	boost::shared_ptr<T> sp;
	boost::weak_ptr<T> wp;
};

// template<typename T>
// class OwningRef : public Ref<T> {
// public:
//    OwningRef() : Ref<T>() {}
//    
//    OwningRef(const boost::shared_ptr<T> & sp) {
//       setOwning(sp);
//    }
// };
// 
// template<typename T>
// class WeakRef : public Ref<T> {
// public:
//    WeakRef(const boost::weak_ptr<T> & wp) {
//       setWeak(wp);
//    }
//    
//    WeakRef(const OwningRef<T> & ref) {
//       sp = ref.sp;
//       wp = ref.wp;
//       owner = ref.owner;
//    }
//    
// };

template<typename T>
Ref<T> Observing(const boost::shared_ptr<T> & sp) {
   Ref<T> ret;
   ret.setWeak(sp);
   return ret;
}


template<typename T>
Ref<T> Owning(const boost::shared_ptr<T> & sp) {
   Ref<T> ret;
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
