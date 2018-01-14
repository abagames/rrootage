/*
 * $Id: attractmanager.c,v 1.5 2003/04/26 03:24:15 kenta Exp $
 *
 * Copyright 2003 Kenta Cho. All rights reserved.
 */

/**
 * Attraction(score/title/gameover) manager.
 *
 * @version $Revision: 1.5 $
 */
#include "SDL.h"
#include <stdlib.h>
#include <stdio.h>

#include "rr.h"
#include "genmcr.h"
#include "screen.h"
#include "letterrender.h"
#include "attractmanager.h"
#include "ship.h"
#include "brgmng_mtd.h"
#include "soundmanager.h"
#include "degutil.h"
#include "boss_mtd.h"

int score;
static int nextExtend, neAdd;
int left, stage, scene;
static float rank;
unsigned long seed;
int bombUsed, shipUsed;
int mode;

static HiScore hiScore;

#define PREF_FILE "rr.prf"
#define DEFAULT_HISCORE 100000

static void initHiScore() {
  int i, j;
  for ( j=0 ; j<MODE_NUM ; j++ ) {
    for ( i=0 ; i<STAGE_NUM ; i++ ) {
      hiScore.score[j][i] = DEFAULT_HISCORE;
      hiScore.cleard[j][i] = 0;
    }
  }
  hiScore.stage = hiScore.mode = 0;
}

static float bgColor[MODE_NUM][3] = {
  {0.0f, 0.0f, 0.0f}, {0.1f, 0.1f, 0.3f}, {0.3f, 0.1f, 0.3f}, {0.1f, 0.3f, 0.1f}, 
};

static void setMode(int m) {
  mode = m;
  glClearColor(bgColor[m][0], bgColor[m][1], bgColor[m][2], 0.0f);
}

// Load preference.
void loadPreference() {
  FILE *fp;
  int i, j;
  int version;
  if ( NULL == (fp = fopen(PREF_FILE,"rb")) ) {
    initHiScore();
    return;
  }
  version = getw(fp);
  if ( version != VERSION_NUM ) {
    initHiScore();
    return;
  }
  for ( j=0 ; j<MODE_NUM ; j++ ) {
    for ( i=0 ; i<STAGE_NUM ; i++ ) {
      hiScore.score[j][i] = getw(fp);
      hiScore.cleard[j][i] = getw(fp);
    }
  }
  hiScore.stage = getw(fp);
  hiScore.mode = getw(fp);
  fclose(fp);
}

// Save preference.
void savePreference() {
  FILE *fp;
  int i, j;
  if ( NULL == (fp = fopen(PREF_FILE,"wb")) ) return;
  putw(VERSION_NUM, fp);
  for ( j=0 ; j<MODE_NUM ; j++ ) {
    for ( i=0 ; i<STAGE_NUM ; i++ ) {
      putw(hiScore.score[j][i], fp);
      putw(hiScore.cleard[j][i], fp);
    }
  }
  putw(hiScore.stage, fp);
  putw(hiScore.mode, fp);
  fclose(fp);
}

static void gotoNextScene() {
  scene++;
  seed = seed*8513L + 179L;
  createBoss(seed, rank, scene);
  bombUsed = shipUsed = 0;
}

#define RANK_UP_BASE 1.25

void initStageState(int stg) {
  float rb;
  int rn, sn;
  int i;
  scene = -1;
  rn = stg/SAME_RANK_STAGE_NUM; sn = stg%SAME_RANK_STAGE_NUM;
  if ( sn == SAME_RANK_STAGE_NUM-1 ) {
    seed = rand();
  } else {
    seed = stg*10357 + 5449 + mode*947;
  }
  rank = 0.1; rb = 0;
  for ( i=0 ; i<rn ; i++ ) {
    rb += RANK_UP_BASE;
    rank += rb;
  }
  if ( stg >= STAGE_NUM || stg < 0 ) rank = 0;
  gotoNextScene();
}

static char stageStr[3] = "1A";

static void makeStageStr(int stg) {
  if ( stg >= 36 ) {
    stageStr[0] = '0';
  } else {
    stageStr[0] = '1' + stg/SAME_RANK_STAGE_NUM;
  }
  if ( stg%SAME_RANK_STAGE_NUM < SAME_RANK_STAGE_NUM-1 ) {
    stageStr[1] = stg%SAME_RANK_STAGE_NUM + 'A';
  } else {
    stageStr[1] = 'R';
  }
}

void initGameStateFirst() {
  score = 0;
  nextExtend = 200000;
  neAdd = 300000;
  left = 2;
  stage = 0;
}

void initGameState(int stg) {
  initGameStateFirst();
  hiScore.stage = stage = stg;
  hiScore.mode = mode;
  makeStageStr(stg);
  initStageState(stg);
}

void addScore(int s) {
  score += s;
  if ( score >= nextExtend ) {
    nextExtend += neAdd;
    neAdd = 500000;
    if ( extendShip() ) {
      playChunk(8);
    }
  }
}

int extendShip() {
  if ( left > 8 ) return 0;
  left++;
  drawRPanel();
  return 1;
}

int decrementShip() {
  left--;
  drawRPanel();
  if ( left < 0 ) return 1;
  return 0;
}

void addLeftBonus() {
  nextExtend = 999999999;
  addScore(left*100000);
}

void setHiScore(int cleard) {
  if ( score > hiScore.score[mode][stage] ) {
    hiScore.score[mode][stage] = score;
  }
  if ( cleard ) {
    hiScore.cleard[mode][stage] = 1;
  }
}

void drawScore() {
  drawNum(score, 118, 24, 28, 200, 200, 222);
  drawNum(bonusScore/10*10, 24, 14, 16, 200, 200, 222);
}

#define SCENE_STAT_X 77
#define SCENE_STAT_SIZE 9

static char *lStr = "LEFT", *bStr = "BOMB", *okStr="OK";

void drawRPanel() {
  int y;
  int ml;
  if ( left >= 0 ) {
    drawString(lStr, 40+480, 280, 18, 1, 200, 200, 222);
    drawLetter(left, 40+480, 420, 18, 1, 230, 180, 150);
    switch ( mode ) {
    case NORMAL_MODE:
      drawString(bStr, 90+480, 280, 18, 1, 200, 200, 222);
      drawLetter(bomb, 90+480, 420, 18, 1, 230, 180, 150);
      break;
    case PSY_MODE:
      ml = ship.grzCnt/40;
      drawBox(550, 460, 50, 8, 120, 120, 120);
      drawBox(500+ml, 460, ml, 8, 210, 210, 240);
      break;
    case GW_MODE:
      ml = (ship.rfMtr-ship.rfMtrDec)/40;
      drawBox(550, 460, 50, 8, 120, 120, 120);
      drawBox(500+ml, 460, ml, 8, 210, 240, 210);
      if ( ml >= 50 ) {
	drawString(okStr, 540, 460, 10, 0, 230, 240, 230);
      }
      break;
    }
  }
  y = 24;
  drawString(stageStr, 124+480, y, 24, 1, 200, 200, 222);
  y += 24*1.7f*2;
  drawLetter(38, 124+480, y, 24, 1, 200, 200, 222);
  y += 24*1.7f;
  drawNumRight(scene+1, 124+480, y, 24, 200, 200, 222);
}

#define STG_BOX_SIZE 21
#define STG_BOX_NUM (MODE_NUM+STAGE_NUM+1)

#define LAYER_HEIGHT 480

static int stageX[STG_BOX_NUM], stageY[STG_BOX_NUM];

void initAttractManager() {
  int i, j, x, y, s;
  //y = 200;
  y = 172;
  s = 0;
  for ( i=0 ; i<12 ; i++, y+=STG_BOX_SIZE*1.2f ) {
    x = 180;
    if ( i < 11 ) {
      for ( j=0 ; j<SAME_RANK_STAGE_NUM ; j++, s++, x+=STG_BOX_SIZE*1.2f ) {
	stageX[s] = x; stageY[s] = y;
      }
    } else {
      stageX[s] = x; stageY[s] = y;
    }
  }
}

static int titleCnt;
static int slcStg;
static int mnp;

int initTitleAtr() {
  stopMusic();
  titleCnt = 0;
  slcStg = hiScore.stage;
  setMode(hiScore.mode);
  mnp = 0;
  return slcStg;
}

#define QUIT_STAGE_NUM 40

void moveTitleMenu() {
  int pad = getPadState();
  int btn = getButtonState();
  int bs = slcStg;
  if ( pad & PAD_DOWN ) {
    if ( mnp ) {
      if ( slcStg < STAGE_NUM-SAME_RANK_STAGE_NUM ) slcStg += SAME_RANK_STAGE_NUM;
      else if ( slcStg == QUIT_STAGE_NUM ) slcStg = -MODE_NUM;
      else slcStg = QUIT_STAGE_NUM;
    }
  } else if ( pad & PAD_UP ) {
    if ( mnp ) {
      if ( slcStg >= 0 ) slcStg -= SAME_RANK_STAGE_NUM;
      else slcStg = QUIT_STAGE_NUM ;
    }
  } else if ( pad & PAD_RIGHT ) {
    if ( mnp ) {
      if ( slcStg >= 0 ) {
	if ( (slcStg%SAME_RANK_STAGE_NUM) < SAME_RANK_STAGE_NUM-1 && slcStg != QUIT_STAGE_NUM ) {
	  slcStg++;
	}
      } else if ( slcStg < -1 ) {
	slcStg++;
      }
    }
  } else if ( pad & PAD_LEFT ) {
    if ( mnp ) {
      if ( slcStg >= 0 ) {
	if ( (slcStg%SAME_RANK_STAGE_NUM) > 0 && slcStg != QUIT_STAGE_NUM ) {
	  slcStg--;
	}
      } else if ( slcStg > -4 ) {
	slcStg--;
      }
    }
  } else if ( btn == 0 ) {
    mnp = 1;
  }
  if ( slcStg != bs ) {
    mnp = 0;
    initTitleStage(slcStg);
    titleCnt = 0;
  }
  if ( mnp && (btn & PAD_BUTTON1) ) {
    if ( slcStg == QUIT_STAGE_NUM  ) {
      quitLast();
    } else if ( slcStg < 0 ) {
      mnp = 0;
      setMode(MODE_NUM+slcStg);
    } else {
      hiScore.stage = slcStg;
      initGame(slcStg);
    }
  }
  if ( mnp && (btn & PAD_BUTTON2) ) {
    mnp = 0;
    setMode((mode+1)%MODE_NUM);
    initTitleStage(slcStg);
    titleCnt = 0;
  }
  titleCnt++;
}

void drawTitle() {
  int i;
  int r, g, b;
  int sx, sy;
  char *stgChr = "STAGE";
  char *quitChr = "QUIT";
  char *mdChr[] = {"NORMAL MODE", "PSY MODE", "IKA MODE", "GW MODE"};
  int mdChrX[] = {270, 330, 330, 350};
  char mdIni[] = {'N', 'P', 'I', 'G'}; 
  drawTitleBoard();
  for ( i=-MODE_NUM ; i<STAGE_NUM+1 ; i++ ) {
    if ( i < 0 ) {
      if ( 4+i == mode ) {
	r = 100; g = 100; b = 240;
      } else {
	r = 150; g = 150; b = 200;
      }
    } else if ( i < QUIT_STAGE_NUM && hiScore.cleard[mode][i] ) {
      r = 240; g = 180; b = 180;
    } else {
      r = 210; g = 210; b = 240;
    }
    sx = stageX[i+MODE_NUM]; sy = stageY[i+MODE_NUM];
    if ( i == slcStg ) {
      int sz = STG_BOX_SIZE*3/2;
      if ( titleCnt < 16 ) sz = sz*titleCnt/16;
      drawBox(sx, sy, sz, sz, r, g, b);
      sz = sz*3/5;
      if ( i < 0 ) {
	int md = MODE_NUM+i;
	drawString(mdChr[md], mdChrX[md], 133, 12, 0, 150, 150, 200);
	drawLetter(mdIni[md]-'A'+10, sx, sy, sz, 0, 150, 150, 240);
      } else if ( i < QUIT_STAGE_NUM  ) {
	makeStageStr(i);
	drawString(stageStr, sx-sz, sy, sz, 0, 210, 210, 240);
	drawString(stgChr, 330, 133, 12, 0, 210, 210, 240);
	drawString(stageStr, 445, 133, 12, 0, 210, 210, 240);
	drawNumCenter(hiScore.score[mode][i], 466, 168, 12, 210, 210, 240);
      } else {
	drawLetter('Q'-'A'+10, sx, sy, sz, 0, 210, 210, 240);
	drawString(quitChr, 410, 133, 12, 0, 210, 210, 240);
      }
    } else {
      drawBox(sx, sy, STG_BOX_SIZE/2, STG_BOX_SIZE/2, r*2/3, g*2/3, b*2/3);
    }
  }
  drawString(mdChr[mode], mdChrX[mode], 455, 12, 0, 150, 150, 200);
}

static int goCnt;

void initGameoverAtr() {
  goCnt = 0;
  mnp = 0;
  fadeMusic();
}

void moveGameover() {
  int btn = getButtonState();
  if ( goCnt > 900 || (goCnt > 128 && mnp && (btn & PAD_BUTTON1)) ) {
    setHiScore(0);
    initTitle();
    return;
  }
  if ( btn == 0 ) {
    mnp = 1;
  }
  goCnt++;
}

void drawGameover() {
  char *goChr = "GAME OVER";
  int y;
  if ( goCnt < 128 ) {
    y = LAYER_HEIGHT/3*goCnt/128;
  } else {
    y = LAYER_HEIGHT/3;
  }
  drawString(goChr, 184, y, 20, 0, 180, 180, 220);
}

static int psCnt = 0;

void movePause() {
  psCnt++;
}

void drawPause() {
  char *psChr = "PAUSE";
  if ( (psCnt&63) < 32 ) {
    drawString(psChr, 252, LAYER_HEIGHT/3, 20, 0, 200, 200, 180);
  }
}

static int bsCnt, bossScore, bsAdd;

void initBossScoreAtr(int bs) {
  bsCnt = 0;
  mnp = 0;
  bossScore = bs;
  bsAdd = bs/(1 + shipUsed + bombUsed)/10*10;
  if ( scene > 3 ) {
    status = STAGE_CLEAR;
    fadeMusic();
  }
}

void moveBossScoreAtr() {
  int btn = getButtonState();
  if ( bsCnt == 128 ) addScore(bsAdd);
  if ( bsCnt > 600 || (bsCnt > 160 && mnp && (btn & PAD_BUTTON1)) ) {
    if ( status != STAGE_CLEAR ) {
      gotoNextScene();
    } else {
      setHiScore(1);
      initTitle();
    }
    return;
  }
  if ( btn == 0 ) {
    mnp = 1;
  }
  bsCnt++;
}

void drawBossScoreAtr() {
  if ( bsCnt < 32 ) return;
  drawNumCenter(bossScore, 450, 240, 16, 200, 200, 220);
  if ( bsCnt < 64 ) return;
  drawBox(320, 272, 150, 4, 200, 200, 220);
  if ( bsCnt < 96 ) return;
  drawNumCenter(1, 230, 306, 16, 200, 200, 220);
  drawLetter(39, 260, 306, 16, 0, 200, 200 ,220);
  drawNumCenter(shipUsed, 340, 306, 16, 200, 200, 220);
  if ( mode == NORMAL_MODE ) {
    drawLetter(39, 380, 306, 16, 0, 200, 200 ,220);
    drawNumCenter(bombUsed, 450, 306, 16, 200, 200, 220);
  }
  if ( bsCnt < 128 ) return;
  drawNumCenter(bsAdd, 450, 380, 19, 200, 220, 200);
  if ( status == STAGE_CLEAR ) {
    char *scChr = "STAGE CLEAR";
    drawString(scChr, 190, 440, 15, 0, 180, 220, 180);
  }
}
