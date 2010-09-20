/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#ifndef HI_CONTROLLER_H_TRQG6TL3
#define HI_CONTROLLER_H_TRQG6TL3

#include <vector>
#include <boost/weak_ptr.hpp>
#include <boost/shared_ptr.hpp>

namespace Graphics {class Sprite; }

class HiController {
public:
   HiController();
   
   void startDirection(int dir);
   void stopDirection(int dir);
   void update(float dt);
   void setTarget(boost::shared_ptr<Graphics::Sprite> & newTarget);
   
private:
   boost::weak_ptr<Graphics::Sprite> target;
   std::vector<bool> activeDirs;
};


#endif /* end of include guard: HI_CONTROLLER_H_TRQG6TL3 */
