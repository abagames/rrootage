/*
 * $Id: foe.h,v 1.5 2003/08/15 07:06:52 kenta Exp $
 *
 * Copyright 2003 Kenta Cho. All rights reserved.
 */

/**
 * Battery/Bullet data.
 *
 * @version $Revision: 1.5 $
 */
#ifndef FOE_H_
#define FOE_H_

extern "C" {
#include "vector.h"
#include "foe_mtd.h"
}

#include "bulletml/bulletmlparser.h"
#include "bulletml/bulletmlparser-tinyxml.h"
#include "bulletml/bulletmlrunner.h"
#include "foecommand.h"
#include "barragemanager.h"
#include "boss.h"

#define BATTERY 0
#define ACTIVE_BULLET 1
#define BULLET 2

#define NOT_EXIST_TMP (NOT_EXIST-1)

struct foe {
  Vector pos, vel, ppos, spos, mv;
  int d, spd;
  FoeCommand *cmd;
  double rank;
  int spc;
  int cnt, cntTotal;
  int xReverse;
  int fireCnt;
  int slowMvCnt;
  BulletMLParser *parser;

  BulletMLParser *morphParser[MORPH_PATTERN_MAX];
  int morphCnt;
  int morphHalf;
  double morphRank;
  double speedRank;

  int color;
  int shapeType;
  int bulletShape[3];
  float bulletSize[3];

  struct limiter *limiter;

  int ikaType;

  int grzRng;
};

typedef struct foe Foe;

Foe* addFoeBattery(int x, int y, double rank, int d, int spd, int xReverse, 
		   BulletMLParser *morphParser[], int morphCnt, int morphHalf, double morphRank,
		   double speedRank,
		   int color, int bulletShape[], float bulletSize[],
		   struct limiter *limiter,
		   int ikaType,
		   BulletMLParser *parser);
void addFoeActiveBullet(Foe *foe, int d, int spd, int color, BulletMLState *state);
void addFoeNormalBullet(Foe *foe, int d, int spd, int color);
void removeFoeCommand(Foe *fe);
void removeFoe(Foe *fe);
void removeFoeForced(Foe *fe);
void wipeBullets(Vector *pos, int width);
#endif
