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

// TODO: delegate = stream output, ref<grej>.
// setDelegate(Owning(new TransformForwarder(helmetSprite, vec2(blah blah), mat(blah)))
// two ways of doing this, either with the TransformForwarder, or with an event. TODO: event!
// TODO: maybe, Owning -> owning? Then Cast -> cast, too. so maybe not. maybe do like in llvm?

#endif /* end of include guard: TRIGGERABLE_H_YUJ548TD */
