/*
 * $Id: vector.h,v 1.1.1.1 2003/03/16 07:03:49 kenta Exp $
 *
 * Copyright 2002 Kenta Cho. All rights reserved.
 */

/**
 * Vector data.
 *
 * @version $Revision: 1.1.1.1 $
 */
#ifndef DEF_VECTOR
#define absN(a) ((a) < 0 ? - (a) : (a))

typedef struct {
  int x, y;
} Vector;
#define DEF_VECTOR
#endif

float vctInnerProduct(Vector *v1, Vector *v2); 
Vector vctGetElement(Vector *v1, Vector *v2);
void vctAdd(Vector *v1, Vector *v2);
void vctSub(Vector *v1, Vector *v2);
void vctMul(Vector *v1, int a);
void vctDiv(Vector *v1, int a);
int vctCheckSide(Vector *checkPos, Vector *pos1, Vector *pos2);
int vctSize(Vector *v);
int vctDist(Vector *v1, Vector *v2);
