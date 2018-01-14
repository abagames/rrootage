/*
 * $Id: laser.c,v 1.3 2003/04/26 03:24:15 kenta Exp $
 *
 * Copyright 2003 Kenta Cho. All rights reserved.
 */

/**
 * Handle players laser.
 *
 * @version $Revision: 1.3 $
 */
#include "SDL.h"

#include "genmcr.h"
#include "screen.h"
#include "vector.h"
#include "degutil.h"
#include "laser.h"
#include "ship.h"
#include "frag.h"
#include "boss_mtd.h"

#define LASER_MAX 64

#define LASER_SPEED 4096
#define LASER_WIDTH 4800
#define LASER_WIDTH_ADD 480

Laser laser[LASER_MAX];
static int laserWidth, laserCnt;

void initLasers() {
  int i;
  for ( i=0 ; i<LASER_MAX ; i++ ) {
    laser[i].cnt = NOT_EXIST;
  }
  laserWidth = laserCnt = 0;
}

#define LASER_COLOR_SPEED 12

static int laserIdx = LASER_MAX;
static int laserColor = 0;
static int laserAdded = 0;

void addLaser() {
  int i;
  for ( i=0 ; i<LASER_MAX ; i++ ) {
    laserIdx--; if ( laserIdx < 0 ) laserIdx = LASER_MAX-1;
    if ( laser[laserIdx].cnt == NOT_EXIST ) break;
  }
  if ( i >= LASER_MAX ) return;
  laser[laserIdx].y = LASER_SPEED;
  laser[laserIdx].color = laserColor;
  laserColor -= LASER_COLOR_SPEED; laserColor &= 255;
  laser[laserIdx].cnt = 0;
  laserWidth += LASER_WIDTH_ADD;
  if ( laserWidth > LASER_WIDTH ) laserWidth = LASER_WIDTH;
  laserAdded = 1;
}

void moveLasers() {
  int i;
  Laser *ls;
  int ry;
  int huy, hdy;
  if ( !laserAdded ) {
    laserWidth -= LASER_WIDTH_ADD;
    if ( laserWidth < 0 ) {
      laserWidth = 0;
      for ( i=0 ; i<LASER_MAX ; i++ ) {
	laser[i].cnt = NOT_EXIST;
      }
      return;
    }
  } else {
    laserAdded = 0;
  }
  hdy = checkHitDownside(ship.pos.x);
  huy = checkHitUpside();
  for ( i=0 ; i<LASER_MAX ; i++ ) {
    if ( laser[i].cnt == NOT_EXIST ) continue;
    else if ( laser[i].cnt == -1 ) { 
      laser[i].cnt = NOT_EXIST;
      continue;
    }
    ls = &(laser[i]);
    ls->y -= LASER_SPEED;
    ry = ship.pos.y + ls->y;
    if ( huy < ry && ry < hdy ) {
      damageBossLaser(ls->cnt);
      if ( (laserCnt&3) == 0 ) {
	addLaserFrag(ship.pos.x, -ship.pos.y-ls->y, LASER_WIDTH);
      }
      ls->cnt = -1;
      continue;
    }
    if ( ry < -FIELD_HEIGHT_8/2 ) {
      ls->cnt = NOT_EXIST;
      continue;
    }
    ls->cnt++;
  }
  laserCnt++;
}

#define LASER_SCREEN_HEIGHT (LASER_SPEED/FIELD_SCREEN_RATIO)

void drawLasers() {
  float x, y;
  int i;
  Laser *ls;
  int t;
  for ( i=0 ; i<LASER_MAX ; i++ ) {
    if ( laser[i].cnt == NOT_EXIST ) continue;
    ls = &(laser[i]);
    x =  (float)ship.pos.x / FIELD_SCREEN_RATIO;
    y = -(float)(ship.pos.y+ls->y) / FIELD_SCREEN_RATIO;
    if ( ls->cnt > 1 )       t = 1;
    else if ( ls->cnt == 1 ) t = 0;
    else                     t = 2;
    drawLaser(x, y, (float)laserWidth/FIELD_SCREEN_RATIO, LASER_SCREEN_HEIGHT,
	      ls->color, 
	      (ls->color+LASER_COLOR_SPEED)&255,
	      (ls->color+LASER_COLOR_SPEED*2)&255,  
	      (ls->color+LASER_COLOR_SPEED*3)&255,
	      laserCnt, t);
  }
}

