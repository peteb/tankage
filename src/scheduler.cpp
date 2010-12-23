/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#include "scheduler.h"
#include "updatable.h"
#include <algorithm>

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
  addedItems.push_back(Item(interval, receiver));
}

void Scheduler::update(float dt) {
  std::copy(addedItems.begin(), addedItems.end(), std::back_inserter(scheduledItems));
  addedItems.clear();

  typedef std::vector<Item>::size_type size_type;
  
  size_type i = 0, e = scheduledItems.size();
  for (; i < e; ++i) {
    Item & item = scheduledItems[i];
    if (!item.trigger(dt)) {
      scheduledItems[i] = scheduledItems[std::max(static_cast<size_type>(1U), e) -
                                         static_cast<size_type>(1U)];
      --e;
    }
  }
  
  scheduledItems.erase(scheduledItems.begin() + e, scheduledItems.end());
}

