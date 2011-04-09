/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#ifndef OPENGL_OPENGL_H
#define OPENGL_OPENGL_H

#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include "gl/glfw.h"
#elif __linux__
#include <GL/gl.h>
#else
#include <windows.h>
#include <GL/GL.h>
#endif

#endif // !OPENGL_OPENGL_H
