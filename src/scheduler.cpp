/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#include <boost/shared_ptr.hpp>
#include "scheduler.h"
#include "updatable.h"

Scheduler::Item::Item(float interval, const boost::weak_ptr<Updatable> & receiver)
   : receiver(receiver)
{
   this->interval = interval;
}
       
void Scheduler::Item::trigger(float dt) {
   if (boost::shared_ptr<Updatable> lReceiver = receiver.lock()) {
      lReceiver->update(dt);
   }
}


void Scheduler::subscribe(float interval, const boost::weak_ptr<Updatable> & receiver) {
   scheduledItems.push_back(Item(interval, receiver));
}

void Scheduler::update(float dt) {
   for (int i = 0; i < scheduledItems.size(); ++i) {
      Item & item = scheduledItems[i];
      item.trigger(dt);
   }
}

