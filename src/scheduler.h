/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#ifndef SCHEDULER_H_KJ99TBMW
#define SCHEDULER_H_KJ99TBMW

#include <boost/weak_ptr.hpp>
#include <vector>
#include "ref.h"

class Updatable;
 
class Scheduler {
public:
   class Item {
   private:
      Item(float interval, const Ref<Updatable> & receiver);
      bool trigger(float dt);
      
      float interval;
      Ref<Updatable> receiver;
      
      friend class Scheduler;
   };
   
   void subscribe(float interval, const Ref<Updatable> & receiver);
   void update(float dt);
   
private:
   std::vector<Item> scheduledItems;
};


#endif /* end of include guard: SCHEDULER_H_KJ99TBMW */
