/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#include "scheduler.h"
#include "updatable.h"

Scheduler::Item::Item(float interval, const Ref<Updatable> & receiver)
   : receiver(receiver)
{
   this->interval = interval;
}
       
bool Scheduler::Item::trigger(float dt) {
   bool retainItem = true;
   
   if (Ref<Updatable>::SharedPtr lReceiver = receiver.lock()) {
      lReceiver->update(dt);
      retainItem = true;
   }
   else {
      retainItem = false;
   }
   
   return retainItem;
}


void Scheduler::subscribe(float interval, const Ref<Updatable> & receiver) {
   scheduledItems.push_back(Item(interval, receiver));
}

void Scheduler::update(float dt) {
   unsigned listSize = scheduledItems.size();
   
   // TODO: make sure no modifications to the scheduledItems list can be done
   //       while this method is running. new items should be added to another list first
   
   for (std::vector<Item>::size_type i = 0; i < listSize; ++i) {
      Item & item = scheduledItems[i];
      if (!item.trigger(dt)) {
         scheduledItems[i] = scheduledItems[std::max(0u, listSize - 1)];
         listSize--;
      }
   }
   
   scheduledItems.erase(scheduledItems.begin() + listSize, scheduledItems.end());
}

