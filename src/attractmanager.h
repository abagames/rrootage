/*
 * $Id: attractmanager.h,v 1.2 2003/03/21 02:59:48 kenta Exp $
 *
 * Copyright 2003 Kenta Cho. All rights reserved.
 */

/**
 * Attraction manager header file.
 *
 * @version $Revision: 1.2 $
 */
#define STAGE_NUM 40
#define SAME_RANK_STAGE_NUM 4
#define MODE_NUM 4

typedef struct {
  int score[MODE_NUM][STAGE_NUM];
  int cleard[MODE_NUM][STAGE_NUM];
  int stage, mode;
} HiScore;

extern int score, left, stage;
extern unsigned long seed;
extern int bombUsed, shipUsed;

#define NORMAL_MODE 0
#define PSY_MODE 1
#define IKA_MODE 2
#define GW_MODE 3

extern int mode;

void loadPreference();
void savePreference();
void initStageState(int stg);
void initGameStateFirst();
void initGameState(int stg);
void addScore(int s);
int extendShip();
int decrementShip();
void addLeftBonus();
void setClearScore();
void setHiScore();
void showScore();
void drawScore();
void drawRPanel();
void initAttractManager();
int initTitleAtr();
void drawTitle();
void drawTitleMenu();
void initGameoverAtr();
void moveGameover();
void drawGameover();
void moveTitleMenu();
void movePause();
void drawPause();
void drawTitle();
void initBossScoreAtr(int bs);
void moveBossScoreAtr();
void drawBossScoreAtr();
