/*
 * $Id: frag.h,v 1.2 2003/03/21 02:59:49 kenta Exp $
 *
 * Copyright 2003 Kenta Cho. All rights reserved.
 */

/**
 * Fragment data.
 *
 * @version $Revision: 1.2 $
 */
#include "vector.h"

#define FRAG_COLOR_NUM 2

typedef struct {
  float x, y, z;
  float mx, my, mz;
  float width, height;
  int d1, d2, md1, md2;
  int r[FRAG_COLOR_NUM], g[FRAG_COLOR_NUM], b[FRAG_COLOR_NUM];
  int cnt;
} Frag;

#define FRAG_MAX 512

void initFrags();
void moveFrags();
void drawFrags();
void addLaserFrag(int x, int y, int width);
void addBossFrag(float x, float y, float z, float width, int d);
void addShipFrag(float x, float y);
void addGrazeFrag(int x, int y, int mx, int my);
void addShapeFrag(GLfloat x, GLfloat y, GLfloat size, int d, int cnt, int type, int mx, int my);
