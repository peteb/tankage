/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#ifndef HI_CONTROLLER_H_TRQG6TL3
#define HI_CONTROLLER_H_TRQG6TL3

#include <vector>
#include "ref.h"

class ReferenceFrame2;
class Triggerable;

class HiController {
public:
   HiController();
   
   void toggle(const std::string & key, int state);
   void startDirection(int dir);
   void stopDirection(int dir);
   void update(float dt);
   void setRefFrameDelegate(const Ref<ReferenceFrame2> & newTarget);
   void setActionDelegate(const Ref<Triggerable> & newTarget);
   
private:
   Ref<ReferenceFrame2> reframeDelegate;
   Ref<Triggerable> actionDelegate;
   std::vector<bool> activeDirs;
};


#endif /* end of include guard: HI_CONTROLLER_H_TRQG6TL3 */
