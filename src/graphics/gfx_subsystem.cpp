/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#include "graphics/gfx_subsystem.h"
#include <OpenGL/OpenGL.h>

using Graphics::Subsystem;
 
Subsystem::Subsystem() {
   
}

void Subsystem::render(float dt) {
   glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
   glClear(GL_COLOR_BUFFER_BIT);
}