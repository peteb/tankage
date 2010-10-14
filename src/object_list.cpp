/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#include "object_list.h"
#include <iostream>

void ObjectList::add(const boost::shared_ptr<Object> & object) {
   objects.push_back(object);
   std::cout << "Objects: " << objects.size() << std::endl;
}

void ObjectList::erase() {
   
}