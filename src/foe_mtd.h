/*
 * $Id: foe_mtd.h,v 1.3 2003/04/04 03:46:21 kenta Exp $
 *
 * Copyright 2003 Kenta Cho. All rights reserved.
 */

/**
 * Foe methods.
 *
 * @version $Revision: 1.3 $
 */
#define FOE_ENEMY_POS_RATIO 1024

#define BULLET_COLOR_NUM 4
#define BULLET_TYPE_NUM 3

extern int processSpeedDownBulletsNum;
extern int nowait;

void initFoes();
void closeFoes();
void moveFoes();
void clearFoes();
void clearFoesZako();
void wipeBullets(Vector *pos, int width);
void drawBulletsWake();
void drawFoes();
void drawBullets();
