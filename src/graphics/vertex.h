/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#ifndef VERTEX_H_L5CPFJ2H
#define VERTEX_H_L5CPFJ2H

#include "vec2.h"

struct Vertex2T2 {
   Vertex2T2(const vec2 & pos, const vec2 & tc) : pos(pos), tc(tc) {}
   
   vec2 pos;
   vec2 tc;
};

#endif /* end of include guard: VERTEX_H_L5CPFJ2H */
