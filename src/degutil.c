/*
 * $Id: degutil.c,v 1.1.1.1 2003/03/16 07:03:49 kenta Exp $
 *
 * Copyright 2003 Kenta Cho. All rights reserved.
 */

/**
 * Changing the cordinate into the angle.
 *
 * @version $Revision: 1.1.1.1 $
 */
#include <math.h>
#include "degutil.h"

static int tantbl[TAN_TABLE_SIZE+2];
int sctbl[SC_TABLE_SIZE+SC_TABLE_SIZE/4];

void initDegutil() {
  int i, d = 0;
  double od = 6.28/DIV;
  for ( i=0 ; i<TAN_TABLE_SIZE ; i++ ) {
    while ( (int)(sin(d*od)/cos(d*od)*TAN_TABLE_SIZE)<i ) d++;
    tantbl[i] = d;
  }
  tantbl[TAN_TABLE_SIZE] = tantbl[TAN_TABLE_SIZE+1] = 128;

  for ( i=0 ; i<SC_TABLE_SIZE+SC_TABLE_SIZE/4 ; i++ ) {
    sctbl[i] = (int)(sin(i*(6.28/SC_TABLE_SIZE))*256);
  }
}

int getDeg(int x, int y) {
  int tx, ty;
  int f, od, tn;

  if ( x==0 && y==0 ) {
    return(512);
  }

  if ( x < 0 ) {
    tx = -x;
    if ( y < 0 ) {
      ty = -y;
      if ( tx > ty ) {
	f = 1;
	od = DIV*3/4; tn = ty*TAN_TABLE_SIZE/tx;
      } else {
	f = -1;
	od = DIV; tn = tx*TAN_TABLE_SIZE/ty;
      }
    } else {
      ty = y;
      if ( tx > ty ) {
	f = -1;
	od = DIV*3/4; tn=ty*TAN_TABLE_SIZE/tx;
      } else {
	f=1;
	od = DIV/2; tn=tx*TAN_TABLE_SIZE/ty;
      }
    }
  } else {
    tx = x;
    if ( y < 0 ) {
      ty = -y;
      if ( tx > ty ) {
	f = -1;
	od = DIV/4; tn = ty*TAN_TABLE_SIZE/tx;
      } else {
	f = 1;
	od = 0; tn = tx*TAN_TABLE_SIZE/ty;
      }
    } else {
      ty = y;
      if ( tx > ty ) {
	f = 1;
	od = DIV/4; tn = ty*TAN_TABLE_SIZE/tx;
      } else {
	f = -1;
	od = DIV/2; tn = tx*TAN_TABLE_SIZE/ty;
      }
    }
  }
  return((od+tantbl[tn]*f)&(DIV-1));
}

int getDistance(int x, int y) {
  if ( x < 0 ) x = -x;
  if ( y < 0 ) y = -y;
  if ( x > y ) {
    return x + (y>>1);
  } else {
    return y + (x>>1);
  }
}

float getDistanceFloat(float x, float y) {
  if ( x < 0 ) x = -x;
  if ( y < 0 ) y = -y;
  if ( x > y ) {
    return x + (y/2);
  } else {
    return y + (x/2);
  }
}

