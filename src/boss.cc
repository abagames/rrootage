/*
 * $Id: boss.cc,v 1.5 2003/08/10 03:21:28 kenta Exp $
 *
 * Copyright 2003 Kenta Cho. All rights reserved.
 */

/**
 * Boss.
 *
 * @version $Revision: 1.5 $
 */
#include <math.h>

extern "C" {
#include "genmcr.h"
#include "degutil.h"
#include "ship.h"
#include "screen.h"
#include "frag.h"
#include "soundmanager.h"
#include "letterrender.h"
#include "attractmanager.h"
#include "rr.h"
}
#include "boss.h"
#include "foe.h"
#include "barragemanager.h"

static Boss boss;
static BossShape bossShape;
int bossTimer;

#define BOSS_INITIAL_X 0
#define BOSS_INITIAL_Y (-FIELD_HEIGHT_8/3)

void initBoss() {
  boss.x = BOSS_INITIAL_X; boss.y = BOSS_INITIAL_Y; 
  boss.d = 0;
  boss.cnt = 0;
}

static Vector bossPos;

Vector* getBossPos() {
  bossPos.x = boss.x; bossPos.y = boss.y; 
  return &bossPos;
}

static void setBatteryGroupPos(BatteryGroup *right, BatteryGroup *left, int cx, int cy) {
  int r, i, x, y, bn;
  int d, ox, oy;
  bn = right->batteryNum;
  if ( randN(4) == 0 ) {
    // Set in the circle shape.
    r = randN(5000) + 5000;
    d = randN(1024);
    for ( i=0 ; i<bn ; i++, d+=1024/bn ) {
      d &= 1023;
      x = cx + ((sctbl[d]    *r)>>8);
      y = cy + ((sctbl[d+256]*r)>>8);
      right->battery[i].x = x; right->battery[i].y = y;
      left->battery[i].x = -x; left->battery[i].y  = y;
    }
  } else {
    // Set in the line shape.
    r = randN(4000) + 4000;
    d = randN(1024);
    ox = ((sctbl[d]*r)>>8); oy = ((sctbl[d+256]*r)>>8);
    x = cx - (bn-1)*ox/2;
    y = cy - (bn-1)*oy/2;
    for ( i=0 ; i<bn ; i++ ) {
      x += ox; y += oy;
      right->battery[i].x = x; right->battery[i].y = y;
      left->battery[i].x = -x; left->battery[i].y  = y;
    }
  }
}

static float baseSize[] = {0.4f, 0.36f, 0.3f};
static int shapePtn[3][8] = {{3, 2,4,6}, {5, 0,2,3,4,5}, {4, 0,1,4,5}};

static void setBatteryShape(BatteryShape *shape) {
  int i;
  if ( mode == IKA_MODE ) {
    shape->color = randN(2);
  } else {
    shape->color = randN(BULLET_COLOR_NUM);
  }
  for ( i=0 ; i<BULLET_TYPE_NUM ; i++ ) {
    if ( mode == IKA_MODE ) {
      shape->bulletShape[i] = randN(2);
      shape->bulletSize[i] = baseSize[i] * ((float)(256+randN(72))/256.0f);
    } else {
      shape->bulletShape[i] = shapePtn[i][randN(shapePtn[i][0]+1)];
      shape->bulletSize[i] = baseSize[i] * ((float)(224+randN(96))/256.0f);
    }
  }
}

static void setAttackIndex(Attack *at, int center) {
  int i;
  if ( center ) {
    switch ( randN(3) ) {
    case 0:
    case 1:
      at->barrageType = NORMAL_BARRAGE;
      break;
    case 2:
      at->barrageType = SIMPLE_BARRAGE;
      break;
    }
  } else {
    switch ( randN(2) ) {
    case 0:
      at->barrageType = NORMAL_BARRAGE;
      break;
    case 1:
      at->barrageType = REVERSIBLE_BARRAGE;
      break;
    }
  }
  at->xReverse = randN(2)*2-1;
  if ( at->barrageType == REVERSIBLE_BARRAGE && randN(4) == 0 ) {
    at->xrAlter = 1;
  } else {
    at->xrAlter = 0;
  }
  at->barrageIdx = randN(barragePatternNum[at->barrageType]);
  for ( i=0 ; i<MORPH_PATTERN_MAX ; i++ ) {
    at->morphIdx[i] = randN(barragePatternNum[MORPH_BARRAGE]);
  }
  if ( at->barrageType != SIMPLE_BARRAGE && randN(4) == 0 ) at->morphHalf = 1;
  else                                                      at->morphHalf = 0;
  if ( mode == IKA_MODE ) {
    int ir = randN(8), it;
    if ( center ) {
      if ( ir < 4 )      it = IKA_ALTERNATE;
      else if ( ir < 7 ) it = IKA_ALTERNATE_SHOT;
      else               it = IKA_HALF;
    } else {
      if ( ir < 3 )      it = IKA_FIX;
      else if ( ir < 6 ) it = IKA_ALTERNATE;
      else               it = IKA_ALTERNATE_SHOT;
    }
    at->ikaType = it;
  }
}

static void setAttackRank(Attack *at, double rank) {
  if ( rank <= 0.3 ) {
    at->rank = rank;
    at->morphCnt = 0;
    at->speedRank = 1;
  } else {
    at->rank = rank*(90+randN(38))/256.0;
    if ( at->rank > 0.8 ) {
      at->rank = 0.2*(randN(8)+1)/8.0 + 0.8;
    }
    rank /= (at->rank+2);
    if ( mode == IKA_MODE ) {
      at->speedRank = sqrt(rank)*(randN(80)+256)/256;
    } else if ( mode == GW_MODE ) {
      at->speedRank = sqrt(rank)*(randN(92)+236)/256;
    } else {
      at->speedRank = sqrt(rank)*(randN(128)+192)/256;
    }
    if ( at->speedRank < 0.8 ) at->speedRank = 0.8;
    at->morphRank = rank / at->speedRank;
    at->morphCnt = 0;
    while ( at->morphRank > 1 ) {
      at->morphCnt++;
      at->morphRank /= 3;
    }
  }
  at->morphType = -1; 
  if ( at->barrageType == SIMPLE_BARRAGE ) {
    if ( at->morphCnt == 0 ) at->morphCnt++;
    at->morphType = MORPH_HEAVY_BARRAGE;
    at->morphIdx[(at->morphCnt-1)&(MORPH_PATTERN_MAX-1)] = 
      randN(barragePatternNum[MORPH_HEAVY_BARRAGE]);
  }
  if ( at->morphCnt == 0 ) {
    switch ( mode ) {
    case PSY_MODE:
      at->morphCnt = 1;
      at->morphType = PSY_MORPH_BARRAGE;
      at->morphIdx[0] = randN(barragePatternNum[PSY_MORPH_BARRAGE]);
      break;
    }
    at->morphRank = 0.5+randN(6)*0.1;
  }
  switch ( mode ) {
  case NORMAL_MODE:
    //at->speedRank *= 0.7f;
    at->morphRank *= 0.7f;
    at->speedRank *= 0.8f;
    break;
  case PSY_MODE:
    at->speedRank *= 0.72f;
    break;
  case IKA_MODE:
    //at->speedRank *= 0.74f;
    at->morphRank *= 0.8f;
    at->speedRank *= 0.77f;
    break;
  case GW_MODE:
    at->speedRank *= 0.8f;
    break;
  }
}

static void setAttack(Attack *at, double rank, int center) {
  setAttackIndex(at, center);
  setAttackRank(at, rank);
}

static void setFoeBattery(Boss *bs, Battery *bt, Attack *at, 
			  BatteryShape *sp, Limiter *lt, int idx) {
  BulletMLParser *mrp[MORPH_PATTERN_MAX];
  int i, mi;
  int xr;
  if ( at->barrageType == NOT_EXIST ) {
    bt->foe = NULL;
    return;
  }
  for ( i=0 ; i<MORPH_PATTERN_MAX ; i++ ) {
    mrp[i] = barragePattern[MORPH_BARRAGE][at->morphIdx[i]].bulletml;
  }
  if ( at->morphType >= 0 ) {
    switch ( at->morphType ) {
    case PSY_MORPH_BARRAGE:
      mrp[0] = barragePattern[at->morphType][at->morphIdx[0]].bulletml;
      break;
    default:
      mi = (at->morphCnt-1)&(MORPH_PATTERN_MAX-1);
      mrp[mi] = barragePattern[at->morphType][at->morphIdx[mi]].bulletml;
      break;
    }
  }
  xr = at->xReverse;
  if ( at->xrAlter && (idx&1) == 1 ) xr = -xr;
  bt->foe = addFoeBattery(bs->x+bt->x, bs->y+bt->y, at->rank, 512, 0, xr,
			  mrp,
			  at->morphCnt, at->morphHalf, at->morphRank, at->speedRank,
			  sp->color, sp->bulletShape, sp->bulletSize,
			  lt,
			  at->ikaType,
			  barragePattern[at->barrageType][at->barrageIdx].bulletml);
}

static void setBossWing(BossWing *lw, BossWing *rw, int size, int num) {
  int i, j;
  lw->wingNum = rw->wingNum = num;
  lw->size = rw->size = 0;
  for ( i=0 ; i<num ; i++ ) {
    for ( j=0 ; j<2 ; j++ ) {
      lw->x[i][j] = ((float)(randN(size/2)+size/2) / FIELD_SCREEN_RATIO) * (randN(2)*2-1);
      lw->y[i][j] = ((float)(randN(size/2)+size/2) / FIELD_SCREEN_RATIO) * (randN(2)*2-1);
      lw->z[i][j] = ((float)(randN(size/2)+size/2) / FIELD_SCREEN_RATIO) * (randN(2)*2-1);
      rw->x[i][j] = -lw->x[i][j];
      rw->y[i][j] =  lw->y[i][j];
      rw->z[i][j] =  lw->z[i][j];
    }
  }
}

static void setBossTree(BatteryGroup *bg, BossTree *left, BossTree *right) {
  int cx, cy, tn, bn, x, y;
  int i;
  left->diffuse = right->diffuse = 0;
  bn = bg->batteryNum;
  left->epNum = right->epNum = bn;
  cx = cy = 0;
  for ( i=0 ; i<bn ; i++ ) {
    cx += bg->battery[i].x; cy += bg->battery[i].y;
    left->ex[i] = (float)bg->battery[i].x / FIELD_SCREEN_RATIO;
    left->ey[i] = (float)bg->battery[i].y / FIELD_SCREEN_RATIO;
    left->ez[i] = 0;
    right->ex[i] = -(float)bg->battery[i].x / FIELD_SCREEN_RATIO;
    right->ey[i] =  (float)bg->battery[i].y / FIELD_SCREEN_RATIO;
    right->ez[i] = 0;
    setBossWing(&(left->eWing[i]), &(right->eWing[i]), 5000, 1);
  }
  cx /= bn; cy /= bn; 
  cx /= 2; cy /= 2;
  tn = 2 + randN(TREE_MAX_LENGTH-2);
  left->posNum = right->posNum = tn+1;
  x = y = 0; cx /= tn; cy /= tn;
  for ( i=0 ; i<=tn ; i++ ) {
    if ( i == 0 ) {
      left->x[i] = left->y[i] = left->z[i] = 0;
    } else {
      left->x[i] = (float)(x+randNS(2000)) / FIELD_SCREEN_RATIO;
      left->y[i] = (float)(y+randNS(2000)) / FIELD_SCREEN_RATIO;
      left->z[i] = (float)randNS(3000) / FIELD_SCREEN_RATIO;
    }
    right->x[i] = -left->x[i];
    right->y[i] =  left->y[i];
    right->z[i] =  left->z[i];
    x += cx; y += cy;
    setBossWing(&(left->wing[i]), &(right->wing[i]), 10000, 2);
  }
}

#define BOSS_PATTERN_CHANGE_CNT 100
#define BOSS_SHIELD 30000
#define BOSS_SHIELD_MAX 70000

#define BOSS_TIMER_COUNT_UP 17
#define BOSS_TIME_UP (1000*90)
static int bossDstBaseTime;

void createBoss(int seed, double rank, int round) {
  int bn, bgn = 0, lbn, bgni = 0;
  int i, j;
  int wx, wy, cx, cy;
  double tr, sr, sra;
  int bx, by;
  int idx1, idx2;
  int maxX = -999999, maxXY;
  int vbgn, vbg[BATTERY_GROUP_MAX/2], vn;
  cx = maxXY = vbgn = 0;

  setSeed(seed);

  // Boss size.
  wx = 18000 + randN(round*2000+5000);
  wy = 12000 + randN(round*1500+3000);

  // Set the number of batterys.
  bn = round/2+4 + randN(round/2+2);
  for ( ; bn>0 && bgn<BATTERY_GROUP_MAX/2 ; bgni += 2) {
    bgn++;
    lbn = randN(3)+1;
    if ( lbn >= bn ) {
      boss.batteryGroup[bgni].batteryNum = bn;
      boss.batteryGroup[bgni+1].batteryNum = bn;
      break;
    }
    bn -= lbn;
    boss.batteryGroup[bgni].batteryNum = lbn;
    boss.batteryGroup[bgni+1].batteryNum = lbn;
  }
  boss.batteryGroupNum = bgn*2;

  // Set the positions of batterys groups.
  cy = -wy;
  for ( i=0 ; i<bgn ; i++ ) {
    cx = randN(wx*2/3)+wx/3;
    cy += wy*3/(bgn+1);
    setBatteryGroupPos(&(boss.batteryGroup[i*2]), &(boss.batteryGroup[i*2+1]), cx, cy);
    if ( cx > maxX ) {
      maxX = cx; maxXY = cy;
    }
    if ( i == 0 ) {
      boss.collisionYUp = cy;
    }
  }
  boss.collisionX[0] = -maxX*4/3; boss.collisionY[0] = maxXY;
  boss.collisionX[4] =  maxX*4/3; boss.collisionY[4] = maxXY;
  boss.collisionX[1] = -cx*4/3; boss.collisionY[1] = cy;
  boss.collisionX[3] =  cx*4/3; boss.collisionY[3] = cy;
  boss.collisionX[2] = 0; boss.collisionY[2] = cy/2;
  if ( boss.collisionYUp > maxXY - 10000 ) {
    boss.collisionYUp = maxXY - 10000;
  }

  boss.shield = BOSS_SHIELD + round*(BOSS_SHIELD_MAX-BOSS_SHIELD)/4;
  boss.patternChangeShield = boss.shield*(70+randN(8))/256;
  boss.damaged = boss.damageCnt = 0;
  bossDstBaseTime = (boss.shield / 40) * BOSS_TIMER_COUNT_UP;

  // Set the shapes of batterys bullets.
  setBatteryShape(&(boss.shape));
  for ( i=0 ; i<bgn ; i++ ) {
    setBatteryShape(&(boss.batteryGroup[i*2].shape));
    boss.batteryGroup[i*2+1].shape = boss.batteryGroup[i*2].shape;
    if ( mode == IKA_MODE ) {
      boss.batteryGroup[i*2+1].shape.color ^= 1;
    }
  }

  // Set the attack patterns.
  boss.patternNum = 4 + randN(3);
  for ( j=0 ; j<boss.patternNum ; j++ ) {
    for ( i=0 ; i<bgn ; i++ ) {
      setAttackIndex(&(boss.batteryGroup[i*2].attack[j]), 0);
    }
  }
  for ( j=0 ; j<boss.patternNum ; j++ ) {
    tr = rank;
    if ( j == 0 ) {
      tr *= 1.2;
      vbgn = 1+bgn/2;
    } else {
      switch ( randN(6) ) {
      case 0:
	vbgn = 0;
	break;
      case 1:
      case 2:
      case 3:
      case 4:
	vbgn = 1;
	break;
      case 5:
	vbgn = 2;
	break;
      }
    }
    if ( vbgn > bgn ) vbgn = bgn;
    for ( i=0 ; i<bgn ; i++ ) vbg[i] = 0;
    for ( i=0 ; i<vbgn ; i++ ) {
      vn = randN(bgn);
      while ( vbg[vn] ) {
	vn--; if ( vn < 0 ) vn += bgn;
      }
      vbg[vn] = 1;
    }
    sra = tr/vbgn;
    for ( i=0 ; i<bgn ; i++ ) {
      if ( tr > 0 && vbg[i] ) {
	sr = ((double)randN(((int)(sra*256+1))))/256 + sra/2;
	if ( sr > tr ) sr = tr;
	bn = boss.batteryGroup[i*2].batteryNum;
	tr -= sr;
	sr /= (bn*bn);
	setAttackRank(&(boss.batteryGroup[i*2].attack[j]), sr/bn);
	boss.batteryGroup[i*2+1].attack[j] = boss.batteryGroup[i*2].attack[j];
	/*if ( boss.batteryGroup[i*2+1].attack[j].barrageType == REVERSIBLE_BARRAGE ) {
	  boss.batteryGroup[i*2+1].attack[j].xReverse *= -1;
	  }*/
	boss.batteryGroup[i*2+1].attack[j].xReverse *= -1;
	boss.batteryGroup[i*2].limiter.max = 
	  boss.batteryGroup[i*2+1].limiter.max = (int)(rank*5)+1;
      } else {
	boss.batteryGroup[i*2].attack[j].barrageType   = NOT_EXIST;
	boss.batteryGroup[i*2+1].attack[j].barrageType = NOT_EXIST;
      }
    }
    if ( tr > 0 ) {
      setAttack(&(boss.topAttack[j]), tr, 1);
      boss.topLimiter.max = (int)(rank*12)+2;
    } else {
      boss.topAttack[j].barrageType = NOT_EXIST;
    }
  }
  boss.patternLgt = 420+randN(90);
  boss.patternIdx = 1;
  boss.patternCnt = BOSS_PATTERN_CHANGE_CNT;
  boss.state = CREATING;
  boss.stateCnt = BOSS_PATTERN_CHANGE_CNT;

  // Set the moving patterns.
  // Boss move size.
  wx = 10000 - round*2000 + randN(4001-round*1000);
  wy = 4500  - round*800  + randN(2001-round*500);

  boss.mpNum = randN(3)+2;
  for ( i=0 ; i<boss.mpNum/2 ; i++ ) {
    boss.mpx[i*2]   =  randN(wx/2) + wx/2;
    boss.mpx[i*2+1] = -boss.mpx[i*2];
    boss.mpy[i*2]   =  boss.mpy[i*2+1] = randN(wy*2)-wy + boss.y;
  }
  if ( boss.mpNum == 3 ) {
    boss.mpx[2] = 0;
    boss.mpy[2] = randN(wy*2)-wy + boss.y;
  }
  for ( i=0 ; i<8 ; i++ ) {
    idx1 = randN(boss.mpNum); idx2 = randN(boss.mpNum); 
    if ( idx1 == idx2 ) {
      idx2++; if ( idx2 >= boss.mpNum ) idx2 = 0;
    }
    bx = boss.mpx[idx1]; by = boss.mpy[idx2];
    boss.mpx[idx1] = boss.mpx[idx2];
    boss.mpy[idx1] = boss.mpy[idx2];
    boss.mpx[idx2] = bx; boss.mpy[idx2] = by;
  }
  boss.speed = randN(48) + 64 - 8*round;
  boss.md = 2+randN(3);
  boss.mpIdx = 0; boss.onRoute = 0;

  // Set the boss shape.
  bossShape.r = 240; bossShape.g = 240; bossShape.b = 120;
  for ( i=0 ; i<boss.batteryGroupNum ; i+=2 ) {
    BatteryGroup *bg;
    bg = &(boss.batteryGroup[i]);
    setBossTree(bg, &(bossShape.tree[i]), &(bossShape.tree[i+1]));
  }
  bossTimer = 0;
}

// Add fragments.

static void addBossTreeFrag(BossTree *bt) {
  int i, dst, deg;
  int ox, oy;
  float x, y;
  int bpn;
  x =  (float)boss.x / FIELD_SCREEN_RATIO;
  y = -(float)boss.y / FIELD_SCREEN_RATIO;
  bpn = bt->posNum-1;
  for ( i=0 ; i<bpn ; i++ ) {
    ox =  (int)((bt->x[i+1]-bt->x[i])*256);
    oy = -(int)((bt->y[i+1]-bt->y[i])*256);
    dst = getDistance(ox, oy);
    deg = getDeg(ox, oy);
    addBossFrag((bt->x[i+1]+bt->x[i])/2 + x,
		-(bt->y[i+1]+bt->y[i])/2 + y,
		(bt->z[i+1]+bt->z[i])/2, 
		(float)dst/512, deg);
  }
  for ( i=0 ; i<bt->epNum ; i++ ) {
    ox =  (int)((bt->ex[i]-bt->x[bpn])*256);
    oy = -(int)((bt->ey[i]-bt->y[bpn])*256);
    dst = getDistance(ox, oy);
    deg = getDeg(ox, oy);
    addBossFrag((bt->ex[i]+bt->x[bpn])/2 + x,
		-(bt->ey[i]+bt->y[bpn])/2 + y,
		(bt->ez[i]+bt->z[bpn])/2, 
		(float)dst/512, deg);
  }
}

static void addBossTreeFragPart() {
  addBossTreeFrag(&(bossShape.tree[randN(boss.batteryGroupNum)]));
}

static void destroyBoss() {
  boss.state = DESTROIED;
  boss.patternCnt = 999999;
  boss.stateCnt = BOSS_PATTERN_CHANGE_CNT*2;
  clearFoes();
  playChunk(5);
  setScreenShake(0, BOSS_PATTERN_CHANGE_CNT*2);
  ship.absEng = 0;
}

// Boss movements.

static int handleLimiter(Limiter *lt, int allLmtOn) {
  if ( !lt->on ) {
    if ( lt->cnt > lt->max ) {
      lt->on = 1;
      lt->cnt /= 3; lt->cnt += 32;
    }
  }
  if ( (boss.cnt&3) == 0 && lt->cnt > 0 ) {
    lt->cnt--;
    if ( allLmtOn ) lt->cnt -= 2;
    if ( lt->on && lt->cnt <= 0 ) {
      lt->on = 0; lt->cnt = 0;
    }
  }
  return lt->on;
}

static int allLmtOn = 0;

void moveBoss() {
  int bpi;
  int i, j;
  int ax, ay, d, od, aod, emd;
  int dfsChg;
  int lmtOn;

  // Change the barrages pattern.
  boss.patternCnt--;
  if ( boss.patternCnt < 0 ) {
    if ( boss.state == LAST_ATTACK ) {
      boss.patternCnt = 999999;
    } else {
      boss.patternCnt = boss.patternLgt;
    }

    bpi = boss.patternIdx;
    boss.topLimiter.cnt = 0; boss.topLimiter.on = 0; 
    setFoeBattery(&boss, &(boss.topBattery), &(boss.topAttack[bpi]), &(boss.shape), 
		  &(boss.topLimiter), 0);
    for ( i=0 ; i<boss.batteryGroupNum ; i++ ) {
      BatteryGroup *bg = &(boss.batteryGroup[i]);
      bg->limiter.cnt = 0; bg->limiter.on = 0;
      for ( j=0 ; j<bg->batteryNum ; j++ ) {
	setFoeBattery(&boss, &(bg->battery[j]), &(bg->attack[bpi]), 
		      &(bg->shape), &(bg->limiter), j);
      }
    }
  } else if ( boss.patternCnt == BOSS_PATTERN_CHANGE_CNT ) {
    if ( boss.topBattery.foe ) removeFoeForced(boss.topBattery.foe);
    for ( i=0 ; i<boss.batteryGroupNum ; i++ ) {
      BatteryGroup *bg = &(boss.batteryGroup[i]);
      for ( j=0 ; j<bg->batteryNum ; j++ ) {
	if ( bg->battery[j].foe ) removeFoeForced(bg->battery[j].foe);
      }
    }
  } else if ( boss.patternCnt < BOSS_PATTERN_CHANGE_CNT ) {
    if ( boss.patternCnt <= BOSS_PATTERN_CHANGE_CNT/2 ) {
      if ( boss.patternCnt == BOSS_PATTERN_CHANGE_CNT/2 && boss.state == ATTACKING ) {
	boss.patternIdx++;
	if ( boss.patternIdx >= boss.patternNum ) boss.patternIdx = 1;
      }
      dfsChg = 6;
    } else {
      dfsChg = -6;
    }
    if ( boss.topAttack[boss.patternIdx].barrageType != NOT_EXIST ) {
      bossShape.diffuse += dfsChg;
      if ( bossShape.diffuse < 0 ) bossShape.diffuse = 0;
      else if ( bossShape.diffuse > 255 ) bossShape.diffuse = 255;
    }
    for ( i=0 ; i<boss.batteryGroupNum ; i++ ) {
      BatteryGroup *bg = &(boss.batteryGroup[i]);
      if ( bg->attack[boss.patternIdx].barrageType != NOT_EXIST ) {
	bossShape.tree[i].diffuse += dfsChg;
	if ( bossShape.tree[i].diffuse < 0 ) bossShape.tree[i].diffuse = 0;
	else if ( bossShape.tree[i].diffuse > 255 ) bossShape.tree[i].diffuse = 255;
      }
    }
  }

  // Movement.
  if ( boss.state >= DESTROIED_END ) {
    boss.d = 0;
    boss.x += (BOSS_INITIAL_X - boss.x)>>6;
    boss.y += (BOSS_INITIAL_Y - boss.y)>>6;
  } else {
    ax = boss.mpx[boss.mpIdx];
    ay = boss.mpy[boss.mpIdx];
    d = getDeg(ax - boss.x, ay - boss.y);
    od = d - boss.d;
    if ( od > 512 ) od -= 1024;
    if ( od < -512 ) od += 1024;
    aod = absN(od);
    if ( !boss.onRoute ) {
      if ( aod < 256 ) {
	boss.onRoute = 1;
      }
    } else {
      if ( aod > 256 ) {
	boss.onRoute = 0;
	boss.mpIdx++; if ( boss.mpIdx >= boss.mpNum ) boss.mpIdx = 0;
      }
    }
    emd = boss.md;
    if ( aod < emd ) {
      boss.d = d;
    } else if ( od > 0 ) {
      boss.d += emd;
    } else {
      boss.d -= emd;
    }
    boss.d &= 1023;
    boss.x += (sctbl[boss.d]    *boss.speed)>>8;
    boss.y -= (sctbl[boss.d+256]*boss.speed)>>8;
  }
  if ( boss.y < -FIELD_HEIGHT_8/2 ) {
    boss.y = -FIELD_HEIGHT_8/2;
  }

  lmtOn = 1;
  if ( boss.topBattery.foe ) {
    boss.topBattery.foe->pos.x = boss.x;
    boss.topBattery.foe->pos.y = boss.y;
  }
  if ( boss.topAttack[boss.patternIdx].barrageType != NOT_EXIST ) {
    lmtOn &= handleLimiter(&(boss.topLimiter), allLmtOn);
  }
  for ( i=0 ; i<boss.batteryGroupNum ; i++ ) {
    BatteryGroup *bg = &(boss.batteryGroup[i]);
    if ( bg->attack[boss.patternIdx].barrageType != NOT_EXIST ) {
      lmtOn &= handleLimiter(&(bg->limiter), allLmtOn);
    }
    for ( j=0 ; j<bg->batteryNum ; j++ ) {
      Battery *bt = &(bg->battery[j]);
      if ( bt->foe ) {
	bt->foe->pos.x = boss.x + bt->x;
	bt->foe->pos.y = boss.y + bt->y;
      }
    }
  }
  if ( lmtOn ) { 
    allLmtOn = 1;
  } else {
    allLmtOn = 0;
  }

  boss.r = bossShape.r; boss.g = bossShape.g; boss.b = bossShape.b;
  if ( boss.damaged ) {
    if ( (boss.damageCnt&1) == 0 ) {
      boss.r = bossShape.r/2; boss.g = 255; boss.b = bossShape.b/2;
      if ( (boss.damageCnt&31) == 0 ) {
	playChunk(2);
      }
    }
    boss.damageCnt++;
    switch ( mode ) {
    case NORMAL_MODE:
      if ( (boss.cnt&7) == 0 ) {
	addScore(bonusScore/10*10);
      }
      break;
    case PSY_MODE:
    case IKA_MODE:
    case GW_MODE:
      if ( (boss.cnt&15) == 0 ) {
	addScore(bonusScore/10*10);
      }
      break;
    }
  } else {
    if ( boss.damageCnt > 0 ) {
      boss.damageCnt = 0;
      haltChunk(2);
    }
    bonusScore -= 10;
    if ( bonusScore < 10 ) bonusScore = 10;
  }
  boss.damaged = 0;
  boss.cnt++;

  boss.stateCnt--;
  switch ( boss.state ) {
  case CREATING:
    if ( boss.stateCnt <= 0 ) {
      boss.state = ATTACKING;
    }
    break;
  case CHANGE:
    if ( boss.stateCnt <= 0 ) {
      boss.state = LAST_ATTACK;
    }
    break;
  case DESTROIED:
    if ( randN(7) == 0 ) addBossTreeFragPart();
    if ( randN(15) == 0 ) playChunk(6);
    for ( i=0 ; i<boss.batteryGroupNum ; i++ ) {
      BossTree *bt = &(bossShape.tree[i]);
      for ( j=0 ; j<bt->posNum ; j++ ) {
	bt->wing[j].size *= 0.99;
      }
      for ( j=0 ; j<boss.batteryGroup[i].batteryNum ; j++ ) {
	bt->eWing[j].size *= 0.985;
      }
    }
    if ( boss.stateCnt <= 0 ) {
      int bs;
      for ( i=0 ; i<boss.batteryGroupNum ; i++ ) {
	BossTree *bt = &(bossShape.tree[i]);
	for ( j=0 ; j<16 ; j++ ) addBossTreeFrag(bt);
      }
      boss.stateCnt = 999999;
      boss.state = DESTROIED_END;
      playChunk(4);
      setScreenShake(1, BOSS_PATTERN_CHANGE_CNT);
      switch ( mode ) {
      case NORMAL_MODE:
	bs = 7000;
	break;
      case PSY_MODE:
      case IKA_MODE:
      case GW_MODE:
	bs = 5000;
	break;
      }
      bs = (long)(bossDstBaseTime*3 - bossTimer)*5000/(bossDstBaseTime*2);
      if ( bs < 0 || bossTimer >= BOSS_TIME_UP ) bs = 0;
      bs *= 100;
      initBossScoreAtr(bs);
    }
    break;
  case DESTROIED_END:
    moveBossScoreAtr();
    break;
  }
  if ( boss.state == ATTACKING || boss.state == LAST_ATTACK ) {
    bossTimer += BOSS_TIMER_COUNT_UP;
    if ( bossTimer >= BOSS_TIME_UP && status == IN_GAME ) {
      bossTimer = BOSS_TIME_UP;
      destroyBoss();
    }
  }
}

void damageBoss(int dmg) {
  int tn, pn, bn;
  int i, j;
  BossTree *bt;
  tn = randN(TREE_MAX_LENGTH);
  if ( tn < boss.batteryGroupNum ) {
    bt = &(bossShape.tree[tn]);
    pn = randN(bt->posNum);
    bt->wing[pn].size *= 0.996;
    bn = randN(boss.batteryGroup[tn].batteryNum);
    bt->eWing[bn].size *= 0.996;
  }
  boss.shield -= dmg;
  boss.damaged = 1;
  switch ( boss.state ) {
  case ATTACKING:
    if ( boss.shield <= boss.patternChangeShield ) {
      boss.shield = boss.patternChangeShield;
      for ( i=0 ; i<boss.batteryGroupNum ; i++ ) {
	BossTree *bt = &(bossShape.tree[i]);
	for ( j=0 ; j<4 ; j++ ) addBossTreeFrag(bt);
	for ( j=0 ; j<bt->posNum ; j++ ) {
	  bt->wing[j].size = 0;
	}
	for ( j=0 ; j<boss.batteryGroup[i].batteryNum ; j++ ) {
	  bt->eWing[j].size = 0;
	}
      }
      boss.state = CHANGE; 
      boss.patternCnt = BOSS_PATTERN_CHANGE_CNT+1;
      boss.stateCnt = BOSS_PATTERN_CHANGE_CNT;
      boss.patternIdx = 0;
      clearFoes();
      playChunk(5);
    }
    break;
  case LAST_ATTACK:
    if ( boss.shield <= 0 ) {
      for ( i=0 ; i<boss.batteryGroupNum ; i++ ) {
	BossTree *bt = &(bossShape.tree[i]);
	for ( j=0 ; j<2 ; j++ )	addBossTreeFrag(bt);
      }
      destroyBoss();
    }
    break;
  }
}

void damageBossLaser(int cnt) {
  if ( mode == NORMAL_MODE || mode == PSY_MODE ) {
    damageBoss(40-cnt);
  } else {
    damageBoss((40-cnt)*2/3);
  }
  bonusScore += (40-cnt)/8;
  if ( bonusScore > 1000 ) bonusScore = 1000;
  ship.grzCnt++; // Laser hitting also increases the graze meter.
}

// Return a collision of the boss.
int checkHitDownside(int x) {
  int i;
  int x1, x2, y1, y2;
  if ( boss.state != ATTACKING && boss.state != LAST_ATTACK ) return -999999;
  for ( i=0 ; i<4 ; i++ ) {
    x1 = boss.collisionX[i]   + boss.x; 
    x2 = boss.collisionX[i+1] + boss.x; 
    if ( x1 <= x && x < x2 ) {
      y1 = boss.collisionY[i]   + boss.y; 
      y2 = boss.collisionY[i+1] + boss.y; 
      return (y2-y1)*(x-x1)/(x2-x1)+y1;
    }
  }
  return -999999;
}

int checkHitUpside() {
  return boss.y + boss.collisionYUp;
}


static void drawBossWing(float x1, float y1, float z1, float x2, float y2, float z2,
			 BossWing *wg) {
  int i;
  float sz = wg->size;
  for ( i=0 ; i<wg->wingNum ; i++ ) {
    drawSquare(x2, y2, z2, x1, y1, z1,
	       x1+wg->x[i][0]*sz, y1+wg->y[i][0]*sz, z1+wg->z[i][0]*sz, 
	       x2+wg->x[i][1]*sz, y2+wg->y[i][1]*sz, z2+wg->z[i][1]*sz,
	       boss.r, boss.g, boss.b);
  }
}

void drawBoss() {
  float x, y;
  float x1, y1, z1, x2, y2, z2;
  int i, j;
  int df;
  int crBpn, crBpl;
  int bpn;
  crBpn = crBpl = 0;
  x =  (float)boss.x / FIELD_SCREEN_RATIO;
  y = -(float)boss.y / FIELD_SCREEN_RATIO;
  if ( bossShape.diffuse > 0  && boss.state < DESTROIED ) {
    df = bossShape.diffuse;
    drawStar(1, x, y, 0, df, df, df, (float)(df+256)/500.0f);
    drawStar(1, x, y, 0, df, df, df, (float)(df+randN(256))/500.0f);
  }
  for ( i=0 ; i<boss.batteryGroupNum ; i++ ) {
    BossTree *bt = &(bossShape.tree[i]);
    bpn = bt->posNum-1;
    x1 = x; y1 = y; z1 = 0;
    switch ( boss.state ) {
    case CREATING:
    case CHANGE:
      crBpn = (bpn+1)*(BOSS_PATTERN_CHANGE_CNT-boss.stateCnt-1)/BOSS_PATTERN_CHANGE_CNT;
      crBpl = 255 - 
	(boss.stateCnt%(BOSS_PATTERN_CHANGE_CNT/(bpn+1))*256)/(BOSS_PATTERN_CHANGE_CNT/(bpn+1));
      break;
    }
    for ( j=0 ; j<bpn ; j++ ) {
      x2 =  x + bt->x[j+1];
      y2 =  y - bt->y[j+1];
      z2 =  bt->z[j+1];
      switch ( boss.state ) {
      case ATTACKING:
      case LAST_ATTACK:
      case DESTROIED:
	drawLine(x1, y1, z1, x2, y2, z2, bossShape.r, bossShape.g, bossShape.b, 240);
	drawBossWing(x1, y1, z1, x2, y2, z2, &(bt->wing[j]));
	break;
      case CREATING:
	if ( j == crBpn ) {
	  drawLinePart(x1, y1, z1, x2, y2, z2, bossShape.r, bossShape.g, bossShape.b, 240, crBpl);
	} else if ( j < crBpn ) {
	  drawLine(x1, y1, z1, x2, y2, z2, bossShape.r, bossShape.g, bossShape.b, 240);
	}
	if ( crBpn == bpn ) {
	  bt->wing[j].size = (float)crBpl/255;
	  drawBossWing(x1, y1, z1, x2, y2, z2, &(bt->wing[j]));
	}
	break;
      case CHANGE:
	drawLine(x1, y1, z1, x2, y2, z2, bossShape.r, bossShape.g, bossShape.b, 240);
	if ( crBpn == bpn ) {
	  bt->wing[j].size = (float)crBpl/128;
	  drawBossWing(x1, y1, z1, x2, y2, z2, &(bt->wing[j]));
	}
	break;
      }
      if ( bt->diffuse > 0 && boss.state != CHANGE && boss.state < DESTROIED ) {
	df = bt->diffuse;
	drawStar(0, x2, y2, z2, df, df, df, (float)(df+256)/900.0f);
	drawStar(0, x2, y2, z2, df, df, df, (float)(df+randN(256))/900.0f);
      }
      x1 = x2; y1 = y2; z1 = z2;
    }
    x1 = x + bt->x[bpn];
    y1 = y - bt->y[bpn];
    z1 = bt->z[bpn];
    for ( j=0 ; j<bt->epNum ; j++ ) {
      x2 = x + bt->ex[j];
      y2 = y - bt->ey[j];
      z2 = bt->ez[j];
      switch ( boss.state ) {
      case ATTACKING:
      case LAST_ATTACK:
      case DESTROIED:
	drawLine(x1, y1, z1, x2, y2, z2, bossShape.r, bossShape.g, bossShape.b, 220);
	drawBossWing(x1, y1, z1, x2, y2, z2, &(bt->eWing[j]));
	break;
      case CREATING:
	if ( crBpn == bpn ) {
	  drawLinePart(x1, y1, z1, x2, y2, z2, bossShape.r, bossShape.g, bossShape.b, 220, crBpl);
	  bt->eWing[j].size = (float)crBpl/255;
	  drawBossWing(x1, y1, z1, x2, y2, z2, &(bt->eWing[j]));
	}
	break;
      case CHANGE:
	drawLine(x1, y1, z1, x2, y2, z2, bossShape.r, bossShape.g, bossShape.b, 220);
	if ( crBpn == bpn ) {
	  bt->eWing[j].size = (float)crBpl/128;
	  drawBossWing(x1, y1, z1, x2, y2, z2, &(bt->eWing[j]));
	}
	break;
      }
      if ( bt->diffuse > 0 && boss.state != CHANGE && boss.state < DESTROIED ) {
	df = bt->diffuse;
	drawStar(1, x2, y2, z2, df, df, df, (float)(df+256)/640.0f);
	drawStar(1, x2, y2, z2, df, df, df, (float)(df+randN(256))/640.0f);
      }
    }
  }
  drawCore(x, y, boss.cnt, boss.r, boss.g, boss.b);
}

void drawBossState() {
  int wd, cwd;
  if ( boss.state >= ATTACKING ) {
    if ( boss.state < DESTROIED || (boss.cnt&31) < 16 ) {
      drawTimeCenter(bossTimer, 470, 44, 10, 210, 240, 210);
    }
    if ( boss.state == DESTROIED_END ) {
      drawBossScoreAtr();
    }
  }
  if ( boss.state >= DESTROIED ) return;
  if ( boss.state == CREATING ) {
    wd = boss.shield*300/BOSS_SHIELD_MAX *
      (BOSS_PATTERN_CHANGE_CNT-boss.stateCnt)/BOSS_PATTERN_CHANGE_CNT;
  } else {
    wd = boss.shield*300/BOSS_SHIELD_MAX;
  }
  drawBox(180+wd/2, 24, wd/2, 6, 240, 240, 210);
  drawNumCenter(boss.shield, 176+wd, 10, 6, 210, 210, 240);
  cwd = boss.patternChangeShield*300/BOSS_SHIELD_MAX;
  if ( wd > cwd ) {
    drawNumCenter(boss.patternChangeShield, 176+cwd, 10, 6, 240, 210, 210);
  }
}
