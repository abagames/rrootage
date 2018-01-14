/*
 * $Id: letterrender.c,v 1.1.1.1 2003/03/16 07:03:49 kenta Exp $
 *
 * Copyright 2002 Kenta Cho. All rights reserved.
 */

/**
 * Letter render.
 *
 * @version $Revision: 1.1.1.1 $
 */
#include "screen.h"
#include "letterrender.h"
#include "letterdata.h"

void drawLetter(int idx, int lx, int ly, int ltSize, int d,
		int r, int g, int b) {
  int i;
  float x, y, length, size, t;
  int deg;
  for ( i=0 ; ; i++ ) {
    deg = (int)spData[idx][i][4];
    if ( deg > 99990 ) break;
    x = -spData[idx][i][0]; y = -spData[idx][i][1];
    size = spData[idx][i][2]; length = spData[idx][i][3];
    size *= 0.66f; length *= 0.6f;
    switch ( d ) {
    case 0:
      x = -x; y = y;
      break;
    case 1:
      t = x; x = -y; y = -t;
      deg += 90;
      break;
    case 2:
      x = x; y = -y;
      deg += 180;
      break;
    case 3:
      t = x; x = y; y = t;
      deg += 270;
      break;
    }
    deg %= 180;
    if ( deg <= 45 || deg > 135 ) {
      drawBox((int)(x*ltSize)+lx, (int)(y*ltSize)+ly, 
	      (int)(size*ltSize), (int)(length*ltSize), r, g, b);
    } else {
      drawBox((int)(x*ltSize)+lx, (int)(y*ltSize)+ly, 
	      (int)(length*ltSize), (int)(size*ltSize), r, g, b);
    }
  }
}
		       
void drawString(char *str, int lx, int ly, int ltSize, int d,
		int r, int g, int b) {
  int x = lx, y = ly;
  int i, c, idx;
  for ( i=0 ; ; i++ ) {
    if ( str[i] == '\0' ) break;
    c = str[i];
    if ( c != ' ' ) {
      if ( c >= '0' && c <='9' ) {
	idx = c-'0';
      } else if ( c >= 'A' && c <= 'Z' ) {
	idx = c-'A'+10;
      } else if ( c >= 'a' && c <= 'z' ) {
	idx = c-'a'+10;
      } else if ( c == '.' ) {
	idx = 36;
      } else if ( c == '-' ) {
	idx = 38;
      } else if ( c == '+' ) {
	idx = 39;
      } else {
	idx = 37;
      }
      drawLetter(idx, x, y, ltSize, d, r, g, b);
    }
    switch ( d ) {
    case 0:
      x += ltSize*1.7f;
      break;
    case 1:
      y += ltSize*1.7f;
      break;
    case 2:
      x -= ltSize*1.7f;
      break;
    case 3:
      y -= ltSize*1.7f;
      break;
    }
  }
}

