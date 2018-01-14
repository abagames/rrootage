/*
 * $Id: ship.c,v 1.4 2003/04/26 03:24:16 kenta Exp $
 *
 * Copyright 2003 Kenta Cho. All rights reserved.
 */

/**
 * Players ship.
 *
 * @version $Revision: 1.4 $
 */
#include "SDL.h"
#include <stdlib.h>
#include <stdio.h>

#include "rr.h"
#include "genmcr.h"
#include "screen.h"
#include "vector.h"
#include "ship.h"
#include "degutil.h"
#include "laser.h"
#include "shot.h"
#include "soundmanager.h"
#include "foe_mtd.h"
#include "frag.h"
#include "attractmanager.h"
#include "boss_mtd.h"

Ship ship;
int bonusScore;
int bomb;

#define SHIP_SPEED 1000
#define SHIP_SLOW_SPEED 500
#define SHIP_SLOW_DOWN 50
#define SHIP_ROLLING_SPEED 300
#define SHIP_IKAGW_SPEED 800

#define SHIP_INVINCIBLE_CNT_BASE 240

#define GRZ_WIDTH 5000
#define FLD_WIDTH 8000
#define RF_WIDTH 10000

#define RF_METER_MAX 2000
#define RF_METER_DEC 50
#define RF_METER_INC 5

static void resetPlayer() {
  bonusScore = 10;
  bomb = 3;
  ship.grzInvCnt = ship.rollingCnt = ship.grzf = 0;
  ship.grzWdt = GRZ_WIDTH;
  ship.absEng = 0;
  ship.rfCnt = ship.rfMtrDec = 0;
  ship.rfMtr = RF_METER_MAX;
  ship.reflects = 0;
}

static int btn2f;

void initShip() {
  ship.pos.x = 0; ship.pos.y = FIELD_HEIGHT_8/5*2;
  ship.cnt = 0; ship.laserCnt = 0;
  ship.invCnt = SHIP_INVINCIBLE_CNT_BASE;
  ship.bombCnt = 0;
  ship.d = 0;
  ship.grzCnt = 0;
  ship.color = ship.colorChgCnt = 0; ship.fldWdt = FLD_WIDTH;
  btn2f = 0;
  switch ( mode ) {
  case NORMAL_MODE:
    ship.speed = SHIP_SPEED;
    break;
  case PSY_MODE:
    ship.speed = SHIP_SLOW_SPEED;
    break;
  case IKA_MODE:
  case GW_MODE:
    ship.speed = SHIP_IKAGW_SPEED;
    break;
  }
  resetPlayer();
}

#define SHOT_INTERVAL 3

#define SHIP_FIELD_WIDTH 1024
#define SHIP_SCREEN_EDGE_WIDTH 3

#define BOMB_SPEED 1280
#define BOMB_UP_SPEED 128
#define BOMB_CNT 160
#define BOMB_NEXT_READY_CNT 90

#define ROLLING_CNT_BASE 45
#define INVINCIBLE_GRZ_NUM 2000
#define INVINCIBLE_GRZ_CNT 120

#define COLOR_CHG_CNT 20
#define ABSENG_CNT 1

static int shipMv[8][2] = {
  {0, -256}, {181, -181}, {256, 0}, {181, 181}, {0, 256}, {-181, 181}, {-256, 0}, {-181, -181},  
};

void moveShip() {
  int pad = getPadState();
  int btn = getButtonState();
  int sd = -1;
  Vector *bossPos;
  if ( pad & PAD_RIGHT ) {
    sd = 2;
  }
  if ( pad & PAD_LEFT ) {
    sd = 6;
  }
  if ( pad & PAD_DOWN ) {
    switch ( sd ) {
    case 2:
      sd = 3;
      break;
    case 6:
      sd = 5;
      break;
    default:
      sd = 4;
      break;
    }
  }
  if ( pad & PAD_UP ) {
    switch ( sd ) {
    case 2:
      sd = 1;
      break;
    case 6:
      sd = 7;
      break;
    default:
      sd = 0;
      break;
    }
  }
  if ( btn & PAD_BUTTON1 ) {
    if ( status == IN_GAME ) {
      addLaser();
      if ( ship.laserCnt == 0 ) {
	playChunk(0);
      } else {
	if ( (ship.laserCnt&15) == 1 ) playChunk(1);
      }
      ship.laserCnt++;
    }
    switch ( mode ) {
    case NORMAL_MODE:
      if ( ship.speed > SHIP_SLOW_SPEED ) {
	ship.speed -= SHIP_SLOW_DOWN;
      }
      break;
    }
  } else {
    switch ( mode ) {
    case NORMAL_MODE:
      if ( ship.speed < SHIP_SPEED ) {
	ship.speed += SHIP_SLOW_DOWN;
      }
      break;
    }
    if ( ship.laserCnt > 0 ) {
      haltChunk(1);
      ship.laserCnt = 0;
    }
  }
  if ( (btn & PAD_BUTTON2) && status == IN_GAME ) {
    switch ( mode ) {
    case NORMAL_MODE:
      if ( bomb > 0 && (ship.bombCnt <= 0 || ship.bombCnt > BOMB_NEXT_READY_CNT ) && 
	   ship.invCnt < SHIP_INVINCIBLE_CNT_BASE-30 ) {
	bomb--; bombUsed++;
	ship.bombCnt = 1; ship.bombWdt = 0; ship.bombPos = ship.pos;
	ship.invCnt = SHIP_INVINCIBLE_CNT_BASE/2;
	playChunk(3);
      }
      break;
    case PSY_MODE:
      ship.rollingCnt = ROLLING_CNT_BASE;
      if ( ship.speed > SHIP_ROLLING_SPEED ) {
	ship.speed -= SHIP_SLOW_DOWN;
      }
      break;
    case IKA_MODE:
      if ( btn2f && ship.colorChgCnt <= 0 ) {
	ship.colorChgCnt = COLOR_CHG_CNT;
	btn2f = 0;
	playChunk(12);
      }
      break;
    case GW_MODE:
      if ( ship.rfMtr >= RF_METER_MAX ) {
	ship.rfMtrDec += RF_METER_DEC;
	if ( ship.rfMtrDec >= RF_METER_MAX ) {
	  ship.rfMtrDec = ship.rfMtr = 0;
	  ship.rfCnt = 120;
	  ship.invCnt = 150;
	  playChunk(13);
	}
      }
    }
  } else {
    switch ( mode ) {
    case PSY_MODE:
      if ( ship.rollingCnt > 0 ) {
	ship.rollingCnt--;
	if ( ship.speed < SHIP_SPEED ) {
	  ship.speed += SHIP_SLOW_DOWN;
	}
      } else {
	if ( ship.speed < SHIP_SLOW_SPEED ) {
	  ship.speed += SHIP_SLOW_DOWN;
	} else if ( ship.speed > SHIP_SLOW_SPEED ) {
	  ship.speed -= SHIP_SLOW_DOWN;
	}
      }
      break;
    case IKA_MODE:
      btn2f = 1;
      break;
    case GW_MODE:
      ship.rfMtrDec = 0;
      break;
    }
  }
  switch ( mode ) {
  case NORMAL_MODE:
    if ( ship.bombCnt > 0 ) {
      ship.bombCnt++;
      ship.bombPos.y -= BOMB_UP_SPEED;
      if ( ship.bombCnt < 20 ) {
	ship.bombWdt += BOMB_SPEED;
      } else if ( ship.bombCnt > BOMB_CNT-20 ) {
	ship.bombWdt -= BOMB_SPEED;
      }
      wipeBullets(&(ship.bombPos), ship.bombWdt);
      if ( ship.bombCnt > BOMB_CNT ) ship.bombCnt = 0;
    }
    ship.d += 2.8;
    break;
  case PSY_MODE:
    if ( ship.rollingCnt > 0 && ship.grzWdt == GRZ_WIDTH ) {
      ship.grzWdt = GRZ_WIDTH*2;
    } else {
      ship.grzWdt = GRZ_WIDTH;
    }
    if ( ship.grzCnt >= INVINCIBLE_GRZ_NUM ) {
      int i;
      ship.grzCnt -= INVINCIBLE_GRZ_NUM;
      ship.grzInvCnt = INVINCIBLE_GRZ_CNT;
      for ( i=0 ; i<32 ; i++ ) {
	addGrazeFrag(ship.pos.x, ship.pos.y, randNS(30000), randNS(30000));
      }
      ship.invCnt = 0;
      playChunk(10);
    }
    if ( ship.grzInvCnt > 0 ) {
      ship.grzInvCnt--;
    }
    ship.d += ship.rollingCnt*0.3;
    if ( ship.grzf ) {
      playChunk(9);
      ship.grzf = 0;
    }
    break;
  case IKA_MODE:
    if ( ship.colorChgCnt > 0 ) {
      ship.colorChgCnt--;
      if ( ship.colorChgCnt == COLOR_CHG_CNT/2 ) {
	ship.color ^= 1;
      }
      if ( ship.colorChgCnt < COLOR_CHG_CNT/2 ) {
	ship.fldWdt = (COLOR_CHG_CNT/2-ship.colorChgCnt)*FLD_WIDTH/(COLOR_CHG_CNT/2);
      } else {
	ship.fldWdt = (ship.colorChgCnt-COLOR_CHG_CNT/2)*FLD_WIDTH/(COLOR_CHG_CNT/2);
      }
    }
    if ( ship.absEng >= ABSENG_CNT ) {
      ship.absEng -= ABSENG_CNT;
      bossPos = getBossPos();
      addShot(ship.pos.x, ship.pos.y, bossPos->x-ship.pos.x, bossPos->y-ship.pos.y, ship.color);
      playChunk(11);
    }
    ship.d += 1.5;
    break;
  case GW_MODE:
    if ( ship.rfCnt > 0 ) {
      if ( ship.rfCnt > 20 ) {
	ship.rfWdt = RF_WIDTH*(ship.rfCnt-20)/200 + RF_WIDTH/2;
      } else {
	ship.rfWdt = RF_WIDTH*(20-ship.rfCnt)/8 + RF_WIDTH/2;
	if ( ship.rfCnt == 20 ) playChunk(14);
      }
      ship.rfCnt--;
    } else if ( ship.rfMtr < RF_METER_MAX ) {
      ship.rfMtr += RF_METER_INC;
      if ( ship.rfMtr >= RF_METER_MAX ) {
	ship.rfMtr = RF_METER_MAX;
	playChunk(15);
      }
    }
    if ( ship.reflects ) {
      ship.reflects = 0;
      playChunk(11);
    }
    ship.d += 3.2;
    break;
  }
  if ( sd >= 0 ) {
    ship.pos.x += (ship.speed*shipMv[sd][0])>>8;
    ship.pos.y += (ship.speed*shipMv[sd][1])>>8;
    if ( ship.pos.x < -FIELD_WIDTH_8/2+SHIP_FIELD_WIDTH*SHIP_SCREEN_EDGE_WIDTH ) {
      ship.pos.x = -FIELD_WIDTH_8/2+SHIP_FIELD_WIDTH*SHIP_SCREEN_EDGE_WIDTH;
    } else if ( ship.pos.x > FIELD_WIDTH_8/2-SHIP_FIELD_WIDTH*SHIP_SCREEN_EDGE_WIDTH ) {
      ship.pos.x = FIELD_WIDTH_8/2-SHIP_FIELD_WIDTH*SHIP_SCREEN_EDGE_WIDTH;
    }
    if ( ship.pos.y < -FIELD_HEIGHT_8/2+SHIP_FIELD_WIDTH*SHIP_SCREEN_EDGE_WIDTH ) {
      ship.pos.y = -FIELD_HEIGHT_8/2+SHIP_FIELD_WIDTH*SHIP_SCREEN_EDGE_WIDTH;
    } else if ( ship.pos.y > FIELD_HEIGHT_8/2-SHIP_FIELD_WIDTH*SHIP_SCREEN_EDGE_WIDTH ) {
      ship.pos.y = FIELD_HEIGHT_8/2-SHIP_FIELD_WIDTH*SHIP_SCREEN_EDGE_WIDTH;
    }
  }

  ship.cnt++;
  if ( ship.invCnt > 0 ) ship.invCnt--;
}

#define SHIP_DRAW_WIDTH 6
#define SHIP_DRUM_WIDTH 15
#define SHIP_DRUM_SIZE 4

void drawShip() {
  float x, y, bx, by;
  int inv = 0, ic;
  x =  (float)ship.pos.x / FIELD_SCREEN_RATIO;
  y = -(float)ship.pos.y / FIELD_SCREEN_RATIO;
  switch ( mode ) {
  case NORMAL_MODE:
    if ( ship.bombCnt > 0 ) {
      bx =  (float)ship.bombPos.x / FIELD_SCREEN_RATIO;
      by = -(float)ship.bombPos.y / FIELD_SCREEN_RATIO;
      drawBomb(bx, by, (float)ship.bombWdt / FIELD_SCREEN_RATIO, ship.bombCnt);
    }
    break;
  case PSY_MODE:
    if ( ship.grzInvCnt > 0 ) {
      drawCircle(x, y, 0.01f * ship.grzInvCnt, ship.grzInvCnt, 150, 180, 240, 220, 220, 230);
    }
    break;
  case IKA_MODE:
    if ( ship.color == 0 ) {
      drawCircle(x, y, (float)ship.fldWdt/FIELD_SCREEN_RATIO, ship.cnt, 
		 120, 120, 150, 255, 255, 255);
    } else {
      drawCircle(x, y, (float)ship.fldWdt/FIELD_SCREEN_RATIO, ship.cnt, 
		 200, 0, 0, 100, 0, 0);
    }
    break;
  case GW_MODE:
    if ( ship.rfCnt > 0 ) {
      drawCircle(x, y, (float)ship.rfWdt/FIELD_SCREEN_RATIO, ship.cnt, 
		 200, 250, 200, 100, 200, 100);
    }
    break;
  }
  ic = ship.invCnt&31;
  if ( ic > 0 && ic < 16 ) inv = 1;
  drawShipShape(x, y, ship.d, inv);
}

void destroyShip() {
  float x, y;
  if ( status != IN_GAME || ship.invCnt > 0 || ship.grzInvCnt > 0 ) return;
  x =  (float)ship.pos.x / FIELD_SCREEN_RATIO;
  y = -(float)ship.pos.y / FIELD_SCREEN_RATIO;
  addShipFrag(x, y);
  playChunk(7);
  shipUsed++;
  resetPlayer();
  if ( decrementShip() ) {
    initGameover();
  } else {
    ship.invCnt = SHIP_INVINCIBLE_CNT_BASE;
    clearFoesZako();
  }
}

int getPlayerDeg(int x, int y) {
  return getDeg(ship.pos.x - x, ship.pos.y - y);
}
