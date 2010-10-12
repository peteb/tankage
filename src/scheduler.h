/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#ifndef SCHEDULER_H_KJ99TBMW
#define SCHEDULER_H_KJ99TBMW

#include <boost/weak_ptr.hpp>
#include <vector>

class Updatable;
 
class Scheduler {
public:
   class Item {
   private:
      Item(float interval, const boost::weak_ptr<Updatable> & receiver);
      void trigger(float dt);
      
      float interval;
      boost::weak_ptr<Updatable> receiver;
      
      friend class Scheduler;
   };
   
   void subscribe(float interval, const boost::weak_ptr<Updatable> & receiver);
   void update(float dt);
   
private:
   std::vector<Item> scheduledItems;
};


#endif /* end of include guard: SCHEDULER_H_KJ99TBMW */
