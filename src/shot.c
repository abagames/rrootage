/*
 * $Id: shot.c,v 1.2 2003/04/26 03:24:16 kenta Exp $
 *
 * Copyright 2002 Kenta Cho. All rights reserved.
 */

/**
 * Handle players shots(in IKA_MODE and GW_MODE).
 *
 * @version $Revision: 1.2 $
 */
#include "SDL.h"
#include <stdlib.h>
#include <stdio.h>

#include "genmcr.h"
#include "screen.h"
#include "vector.h"
#include "degutil.h"
#include "ship.h"
#include "shot.h"
#include "attractmanager.h"
#include "boss_mtd.h"

Shot shot[SHOT_MAX];

void initShots() {
  int i;
  for ( i=0 ; i<SHOT_MAX ; i++ ) {
    shot[i].cnt = -1;
  }
}

static int shotIdx = SHOT_MAX;

#define SHOT_SPEED 4096
#define SHOT_HEIHGT_SPEED (SHOT_SPEED/FIELD_SCREEN_RATIO)

void addShot(int x, int y, int ox, int oy, int color) {
  int i, d, ds;
  Shot *st;
  for ( i=0 ; i<SHOT_MAX ; i++ ) {
    shotIdx--; if ( shotIdx < 0 ) shotIdx = SHOT_MAX-1;
    if ( shot[shotIdx].cnt < 0 ) break;
  }
  if ( i >= SHOT_MAX ) return;
  st = &(shot[shotIdx]);
  st->x = (float)x/FIELD_SCREEN_RATIO; st->y = -(float)y/FIELD_SCREEN_RATIO;
  d = getDeg(-ox, oy); ds = getDistance(ox, oy);
  st->mx = -(float)sctbl[d]    *SHOT_SPEED/(FIELD_SCREEN_RATIO*256);
  st->my =  (float)sctbl[d+256]*SHOT_SPEED/(FIELD_SCREEN_RATIO*256);
  st->d = (float)d*360/1024;
  st->color = color;
  st->cnt = ds/SHOT_SPEED;
  st->width = 0.07;
  st->height = 0.1;
}

void moveShots() {
  int i;
  Shot *st;
  if ( mode != IKA_MODE && mode != GW_MODE ) return;
  for ( i=0 ; i<SHOT_MAX ; i++ ) {
    if ( shot[i].cnt < 0 ) continue;
    st = &(shot[i]);
    st->x += st->mx/2;
    st->y += st->my/2;
    st->height += SHOT_HEIHGT_SPEED/2;
    st->cnt--;
    if ( st->cnt < 0 ) {
      switch ( mode ) {
      case IKA_MODE:
	damageBoss(64);
	break;
      case GW_MODE:
	damageBoss(30);
	break;
      }
    }
  }
}

void drawShots() {
  int i;
  Shot *st;
  if ( mode != IKA_MODE && mode != GW_MODE ) return;
  for ( i=0 ; i<SHOT_MAX ; i++ ) {
    if ( shot[i].cnt < 0 ) continue;
    st = &(shot[i]);
    drawShot(st->x, st->y, st->d, st->color, st->width, st->height);
  }
}
