/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#ifndef TEXTURE_H_61TB0IDZ
#define TEXTURE_H_61TB0IDZ

#include "math/rect.h"

namespace Graphics {
   class Texture {
   public:
	  virtual ~Texture() {}
   
	  virtual rect getSize() const =0;
	  virtual void bind() const =0;
   };
}

#endif /* end of include guard: TEXTURE_H_61TB0IDZ */
