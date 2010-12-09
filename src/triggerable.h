/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#ifndef TRIGGERABLE_H_YUJ548TD
#define TRIGGERABLE_H_YUJ548TD

#include <string>

class Triggerable {
public:
   virtual ~Triggerable() {}
   virtual void trigger(const std::string & action, int state) = 0;
};

#endif /* end of include guard: TRIGGERABLE_H_YUJ548TD */
