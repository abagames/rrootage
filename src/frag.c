/*
 * $Id: frag.c,v 1.2 2003/03/21 02:59:49 kenta Exp $
 *
 * Copyright 2003 Kenta Cho. All rights reserved.
 */

/**
 * Fragments.
 *
 * @version $Revision: 1.2 $
 */
#include "genmcr.h"
#include "screen.h"
#include "vector.h"
#include "degutil.h"
#include "ship.h"
#include "frag.h"

Frag frag[FRAG_MAX];
static int fragIdx;

void initFrags() {
  int i;
  for ( i=0 ; i<FRAG_MAX ; i++ ) {
    frag[i].cnt = 0;
  }
  fragIdx = FRAG_MAX;
}

static int fragColors[][3] = {
  {100, 255, 100}, {240, 240, 240},
  {240, 240, 120}, {255, 255, 255},
  {128, 128, 128}, {255, 255, 255},
  {100, 100, 255}, {220, 220, 200},
};

#define FRAG_LINE 0

static void addLineFrag(int x, int y, int z, int mx, int my, int mz,
			int width, int d1, int cp, int cnt) {
  int i;
  Frag *fr;
  for ( i=0 ; i<FRAG_MAX ; i++ ) {
    fragIdx--; if ( fragIdx < 0 ) fragIdx = FRAG_MAX-1;
    if ( frag[fragIdx].cnt <= 0 ) break;
  }
  if ( i >= FRAG_MAX ) return;
  fr = &(frag[fragIdx]);
  fr->x = x / FIELD_SCREEN_RATIO;
  fr->y = y / FIELD_SCREEN_RATIO;
  fr->z = z / FIELD_SCREEN_RATIO;
  fr->mx = mx / FIELD_SCREEN_RATIO;
  fr->my = my / FIELD_SCREEN_RATIO;
  fr->mz = mz / FIELD_SCREEN_RATIO;
  fr->width = width / FIELD_SCREEN_RATIO;
  fr->d1 = d1; fr->d2 = 0;
  fr->md1 = randNS(32); fr->md2 = randNS(32);
  for ( i=0 ; i<FRAG_COLOR_NUM ; i++ ){
    fr->r[i] = fragColors[cp+i][0];
    fr->g[i] = fragColors[cp+i][1];
    fr->b[i] = fragColors[cp+i][2];
  }
  fr->cnt = cnt; 
}

static void addLineFragFloat(float x, float y, float z, int mx, int my, int mz,
			     float width, int d1, int cp, int cnt) {
  int i;
  Frag *fr;
  for ( i=0 ; i<FRAG_MAX ; i++ ) {
    fragIdx--; if ( fragIdx < 0 ) fragIdx = FRAG_MAX-1;
    if ( frag[fragIdx].cnt <= 0 ) break;
  }
  if ( i >= FRAG_MAX ) return;
  fr = &(frag[fragIdx]);
  fr->x = x;
  fr->y = y;
  fr->z = z;
  fr->mx = mx / FIELD_SCREEN_RATIO;
  fr->my = my / FIELD_SCREEN_RATIO;
  fr->mz = mz / FIELD_SCREEN_RATIO;
  fr->width = width;
  fr->d1 = d1; fr->d2 = 0;
  fr->md1 = randNS(5); fr->md2 = randNS(5);
  for ( i=0 ; i<FRAG_COLOR_NUM ; i++ ){
    fr->r[i] = fragColors[cp+i][0];
    fr->g[i] = fragColors[cp+i][1];
    fr->b[i] = fragColors[cp+i][2];
  }
  fr->cnt = cnt; 
}

void addLaserFrag(int x, int y, int width) {
  int wd = width/4;
  int i;
  int lx = x - width/4*3;
  for ( i=0 ; i<4 ; i++, lx += width/2 ) {
    addLineFrag(lx, y, 0, randNS(512), 512+randN(512), randNS(256), wd, 256, 0, 10+randN(10));
    addLineFrag(lx, y, 0, randNS(512), 512+randN(512), randNS(256), wd,   0, 0, 10+randN(10));
  }
}

void addBossFrag(float x, float y, float z, float width, int d) {
  addLineFragFloat(x, y, z, randNS(1024), randNS(1024), randNS(1024),
		   width, d, 2, 52+randN(20));
}

void addShipFrag(float x, float y) {
  int i, d, s;
  for ( i=0 ; i<80 ; i++ ) {
    d = randN(1024);
    s = randN(128)+128;
    addLineFragFloat(x, y, 0, (sctbl[d]*s)>>6, (sctbl[d+256]*s)>>6, randNS(1024), 
		     0.5f, -d&1023, 0, 24+randN(12));
  }
}

void addGrazeFrag(int x, int y, int mx, int my) {
  int d = getDeg(mx, -my);
  addLineFrag(x, -y, 0, (mx>>2), (-my>>2), randNS(1024), 5000, d, 6, 16);
  addLineFrag(x, -y, 0, (mx>>2), (-my>>2), randNS(1024), 2500, d, 6, 10);
}

void addLineFragOfs(GLfloat x, GLfloat y, GLfloat ox1, GLfloat oy1, GLfloat ox2, GLfloat oy2, 
		    int d,
		    int mx, int my) {
  GLfloat cx, cy, ox, oy, lw;
  int ld;
  ox = (ox1+ox2)/2; oy = (oy1+oy2)/2;
  d = -d; d &= 1023;
  cx = (ox*sctbl[d+256] - oy*sctbl[d]) / 256.0f + x;
  cy = (ox*sctbl[d] + oy*sctbl[d+256]) / 256.0f + y;
  ld = (getDeg((int)((ox1-ox2)*256), (int)((oy1-oy2)*256)) + d) & 1023;
  lw = getDistanceFloat(ox2-ox1, oy2-oy1)/2;
  addLineFragFloat(cx, cy, 0, mx, -my, -randN(1024)-1024, lw, ld, 4, 24+randN(16));
}
 
void addShapeFrag(GLfloat x, GLfloat y, GLfloat size, int d, int cnt, int type, int mx, int my) {
  int sd;
  GLfloat sz, sz2;
  switch ( type ) {
  case -1:
    sz = size/2;
    addLineFragOfs(x, y, 0, -sz, 0, sz, d, mx, my);
    break;
  case 0:
    sz = size/2;
    addLineFragOfs(x, y, -sz, -sz, sz, -sz, d, mx, my);
    addLineFragOfs(x, y, sz, -sz, 0, size, d, mx, my);
    addLineFragOfs(x, y, 0, size, -sz, -sz, d, mx, my);
    break;
  case 1:
    sz = size/2;
    sd = (cnt*23)&1023;
    addLineFragOfs(x, y, 0, -size, sz, 0, sd, mx, my);
    addLineFragOfs(x, y, sz, 0, 0, size, sd, mx, my);
    addLineFragOfs(x, y, 0, size, -sz, 0, sd, mx, my);
    addLineFragOfs(x, y, -sz, 0, 0, -size, sd, mx, my);
    break;
  case 2:
    sz = size/4; sz2 = size/3*2;
    addLineFragOfs(x, y, -sz, -sz2, sz, -sz2, d, mx, my);
    addLineFragOfs(x, y, sz, -sz2, sz, sz2 ,d, mx, my);
    addLineFragOfs(x, y, sz, sz2, -sz, sz2, d, mx, my);
    addLineFragOfs(x, y, -sz, sz2, -sz, -sz2, d, mx, my);
    break;
  case 3:
    sz = size/2;
    sd = (cnt*37)&1023;
    addLineFragOfs(x, y, -sz, -sz, sz, -sz, sd, mx, my);
    addLineFragOfs(x, y, sz, -sz, sz, sz ,sd, mx, my);
    addLineFragOfs(x, y, sz, sz, -sz, sz, sd, mx, my);
    addLineFragOfs(x, y, -sz, sz, -sz, -sz, sd, mx, my);
    break;
  case 4:
    sz = size/2;
    sd = (cnt*53)&1023;
    addLineFragOfs(x, y, -sz/2, -sz, sz/2, -sz, sd, mx, my);
    addLineFragOfs(x, y, sz/2, -sz, sz, -sz/2, sd, mx, my);
    addLineFragOfs(x, y, sz, -sz/2, sz, sz/2, sd, mx, my);
    addLineFragOfs(x, y, sz, sz/2, -sz/2, sz, sd, mx, my);
    addLineFragOfs(x, y, -sz/2, sz, -sz, sz/2, sd, mx, my);
    addLineFragOfs(x, y, -sz, sz/2, -sz, -sz/2, sd, mx, my);
    break;
  case 5:
    sz = size*2/3; sz2 = size/5;
    addLineFragOfs(x, y, -sz, -sz+sz2, sz, -sz+sz2, d, mx, my);
    addLineFragOfs(x, y, sz, -sz+sz2, 0, sz+sz2, d, mx, my);
    addLineFragOfs(x, y, 0, sz+sz2, -sz, -sz+sz2, d, mx, my);
    break;
  case 6:
    sz = size/2;
    sd = (cnt*13)&1023;
    addLineFragOfs(x, y, -sz, -sz, 0, -sz, sd, mx, my);
    addLineFragOfs(x, y, 0, -sz, sz, 0, sd, mx, my);
    addLineFragOfs(x, y, sz, 0, sz, sz, sd, mx, my);
    addLineFragOfs(x, y, sz, sz, 0, sz,  sd, mx, my);
    addLineFragOfs(x, y, 0, sz, -sz, 0, sd, mx, my);
    addLineFragOfs(x, y, -sz, 0, -sz, -sz, sd, mx, my);
    break;
  }
}

void moveFrags() {
  int i;
  Frag *fr;
  for ( i=0 ; i<FRAG_MAX ; i++ ) {
    if ( frag[i].cnt <= 0 ) continue;
    fr = &(frag[i]);
    fr->x += fr->mx;
    fr->y += fr->my;
    fr->z += fr->mz;
    fr->d1 += fr->md1;
    fr->d2 += fr->md2;
    fr->d1 &= 1023; fr->d2 &= 1023;
    fr->cnt--;
  }
}

void drawFrags() {
  int c;
  int i;
  Frag *fr;
  for ( i=0 ; i<FRAG_MAX ; i++ ) {
    if ( frag[i].cnt <= 0 ) continue;
    fr = &(frag[i]);
    c = fr->cnt&(FRAG_COLOR_NUM-1);
    drawRollLine(fr->x, fr->y, fr->z, fr->width, 
		 fr->r[c], fr->g[c], fr->b[c], 255, fr->d1, fr->d2);
  }
}
