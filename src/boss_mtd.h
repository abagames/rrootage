/*
 * $Id: boss_mtd.h,v 1.2 2003/03/21 02:59:48 kenta Exp $
 *
 * Copyright 2003 Kenta Cho. All rights reserved.
 */

/**
 * Boss methods.
 *
 * @version $Revision: 1.2 $
 */
#include "vector.h"

void createBoss(int seed, double rank, int round);
void initBoss();
void moveBoss();
void drawBoss();
int checkHitDownside(int x);
int checkHitUpside();
void damageBoss(int dmg);
void damageBossLaser(int cnt);
void weakenBoss();
void drawBossState();
Vector* getBossPos();
