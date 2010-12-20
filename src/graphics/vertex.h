/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#ifndef VERTEX_H_L5CPFJ2H
#define VERTEX_H_L5CPFJ2H

#include "math/vec2.h"
#include "graphics/color.h"

struct Vertex2T2 {
  Vertex2T2(const vec2 &pos, const vec2 &tc, const Graphics::Color &col = Graphics::Color::White)
    : pos(pos), tc(tc), color(col) {}
   
  vec2 pos;
  vec2 tc;
  Graphics::Color color;
};

#endif /* end of include guard: VERTEX_H_L5CPFJ2H */
