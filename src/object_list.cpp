/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#include "object_list.h"
#include <algorithm>

void ObjectList::add(const boost::shared_ptr<Object> & object) {
   objects.push_back(object);
}



template<typename T>
class ExistsInPred {
public:
   ExistsInPred(const T & list) : list(list) {}
   
   bool operator() (const boost::shared_ptr<Object> & obj) {
      return std::find(list.begin(), list.end(), obj.get()) != list.end();
   }
   
private:
   const T & list;
};

template<typename T> 
ExistsInPred<T> ExistsIn(const T & list) {return ExistsInPred<T>(list); }

void ObjectList::enqueueRemoval(Object * obj) {
   enqueuedItems.push_back(obj);
}

void ObjectList::eraseEnqueuedRemovals() {
   if (!enqueuedItems.empty()) {
      objects.erase(
         std::remove_if(objects.begin(), objects.end(), ExistsIn(enqueuedItems)),
         objects.end()
      );
   }
   
   enqueuedItems.clear();
}
