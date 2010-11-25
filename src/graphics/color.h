/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#ifndef GRAPHICS_COLOR_H
#define GRAPHICS_COLOR_H

namespace Graphics
{
   class Color
   {
   public:
	  Color(float r, float g, float b, float a);

	  float r, g, b, a;
   };
   
}

#endif // !GRAPHICS_COLOR_H
