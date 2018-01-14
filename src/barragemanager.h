/*
 * $Id: barragemanager.h,v 1.3 2003/04/04 03:46:20 kenta Exp $
 *
 * Copyright 2002 Kenta Cho. All rights reserved.
 */

/**
 * Barrage data.
 *
 * @version $Revision: 1.3 $
 */
#ifndef BARRAGEMANAGER_H_
#define BARRAGEMANAGER_H_

#include "bulletml/bulletmlparser.h"
#include "bulletml/bulletmlparser-tinyxml.h"
#include "bulletml/bulletmlrunner.h"

typedef struct {
  BulletMLParser *bulletml;
  double maxRank, rank;
} Barrage;

#define BARRAGE_TYPE_NUM 6
#define BARRAGE_PATTERN_MAX 32

#define NORMAL_BARRAGE 0
#define REVERSIBLE_BARRAGE 1
#define MORPH_BARRAGE 2
#define SIMPLE_BARRAGE 3
#define MORPH_HEAVY_BARRAGE 4
#define PSY_MORPH_BARRAGE 5

extern Barrage barragePattern[BARRAGE_TYPE_NUM][BARRAGE_PATTERN_MAX];
extern int barragePatternNum[BARRAGE_TYPE_NUM];

extern "C" {
#include "brgmng_mtd.h"
}
#endif
