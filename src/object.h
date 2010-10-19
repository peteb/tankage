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
   virtual void submitComponents(World & wold) =0;
   
   void addedOwner(ObjectList * owner);
   void kill();
   
private:
   ObjectList * owner;
};


#endif /* end of include guard: OBJECT_H_GV2YM86M */
