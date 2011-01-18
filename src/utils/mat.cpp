/*
 * Snail Wail
 * 
 * (c) Copyright 2010 Peter Backman. All Rights Reserved. 
 */

#include "mat.h"

mat2 mat2::Identity() {
  static mat2 identity(1.0f, 0.0f, 0.0f, 1.0f);
  return identity;
}

mat2 mat2::Zero() {
  static mat2 zero(0.0f, 0.0f, 0.0f, 0.0f);
  return zero;
}

mat2 mat2::operator * (const mat2 &m) const {
  mat2 ret;
  int i, j, k;
  
  for (i = 0; i < 2; i++) {
    for (j = 0; j < 2; j++) {
      for (k = 0, ret.elements[i][j] = 0; k < 2; k++)
        ret.elements[i][j] += elements[i][k] * m.elements[k][j];	
    }
  }
  
  return ret;	
}

