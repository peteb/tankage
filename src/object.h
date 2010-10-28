/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#ifndef OBJECT_H_GV2YM86M
#define OBJECT_H_GV2YM86M

class World;
class ObjectList;

class Object {
public:
   Object();
   virtual ~Object() {}

   void addedOwner(ObjectList * owner);
   void kill();
	virtual void activate();
	virtual void deactivate();
   
private:
   ObjectList * owner;
};


#endif /* end of include guard: OBJECT_H_GV2YM86M */
