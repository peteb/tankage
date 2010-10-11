/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#ifndef UPDATABLE_H_SN7077MK
#define UPDATABLE_H_SN7077MK

class Updatable {
public:
   virtual ~Updatable() {}
   
   virtual void update(float dt) =0;
};


#endif /* end of include guard: UPDATABLE_H_SN7077MK */
