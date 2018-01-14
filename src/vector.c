/*
 * $Id: vector.c,v 1.1.1.1 2003/03/16 07:03:49 kenta Exp $
 *
 * Copyright 2002 Kenta Cho. All rights reserved.
 */

/**
 * Vector functions.
 *
 * @version $Revision: 1.1.1.1 $
 */
#include <math.h>

#include "vector.h"

float vctInnerProduct(Vector *v1, Vector *v2) {
  return (float)v1->x*v2->x + (float)v1->y*v2->y;
}

Vector vctGetElement(Vector *v1, Vector *v2) {
  Vector ans;
  int ll = v2->x*v2->x + v2->y*v2->y;
  if ( ll != 0 ) {
    int mag = vctInnerProduct(v1, v2);
    ans.x = mag*v2->x/ll;
    ans.y = mag*v2->y/ll;
  } else {
    ans.x = ans.y = 0; 
  }
  return ans;
}

void vctAdd(Vector *v1, Vector *v2) {
  v1->x += v2->x;
  v1->y += v2->y;
}

void vctSub(Vector *v1, Vector *v2) {
  v1->x -= v2->x;
  v1->y -= v2->y;
}

void vctMul(Vector *v1, int a) {	
  v1->x *= a;
  v1->y *= a;
}

void vctDiv(Vector *v1, int a) {	
  v1->x /= a;
  v1->y /= a;
}

int vctCheckSide(Vector *checkPos, Vector *pos1, Vector *pos2) {
  int xo = pos2->x-pos1->x, yo = pos2->y-pos1->y;
  if ( xo == 0 ) {
    if ( yo == 0 ) return 0;
    return checkPos->x - pos1->x;
  } else if ( yo == 0 ) {
    return pos1->y - checkPos->y;
  } else {
    if ( xo*yo > 0 ) { 
      return (checkPos->x-pos1->x)/xo - (checkPos->y-pos1->y)/yo;
    } else {
      return -(checkPos->x-pos1->x)/xo + (checkPos->y-pos1->y)/yo;
    }
  }
}

int vctSize(Vector *v) {
  return sqrt(v->x*v->x + v->y*v->y);
}

int vctDist(Vector *v1, Vector *v2) {
  int ax = absN(v1->x - v2->x), ay = absN(v1->y - v2->y);
  if ( ax > ay ) {
    return ax + (ay>>1);
  } else {
    return ay + (ax>>1);
  }
}
