/*
 * $Id: screen.h,v 1.4 2003/04/26 03:24:16 kenta Exp $
 *
 * Copyright 2003 Kenta Cho. All rights reserved.
 */

/**
 * Opengl screen functions header file.
 *
 * @version $Revision: 1.4 $
 */
#include "SDL.h"

#include <GL/gl.h>
#include <GL/glu.h>

#define PAD_UP 1
#define PAD_DOWN 2
#define PAD_LEFT 4
#define PAD_RIGHT 8
#define PAD_BUTTON1 16
#define PAD_BUTTON2 32

#define DEFAULT_BRIGHTNESS 224

extern float eyeX, eyeY, eyeZ;
extern float pitch, roll;
extern float zoom;
extern Uint8 *keys;
extern SDL_Joystick *stick;
extern int buttonReversed;
extern int lowres;
extern int windowMode;
extern int brightness;

int getPadState();
int getButtonState();

void loadModel(char *fileName, GLuint *model);
void loadGLTexture(char*, GLuint*);
void generateTexture(GLuint*);
void deleteTexture(GLuint*);
void initSDL();
void closeSDL();
void resized(int, int);
void drawGLSceneStart();
void drawGLSceneEnd();
void swapGLScene();

void setScreenShake(int type, int cnt);
void moveScreenShake();

void drawBox(GLfloat x, GLfloat y, GLfloat width, GLfloat height, int r, int g, int b);
void drawLine(GLfloat, GLfloat, GLfloat,
	      GLfloat, GLfloat, GLfloat, int, int, int, int);
void drawLinePart(GLfloat x1, GLfloat y1, GLfloat z1,
		  GLfloat x2, GLfloat y2, GLfloat z2, int r, int g, int b, int a, int len);
void drawRollLineAbs(GLfloat x1, GLfloat y1, GLfloat z1,
		     GLfloat x2, GLfloat y2, GLfloat z2, int r, int g, int b, int a, int d1);
void drawRollLine(GLfloat x, GLfloat y, GLfloat z, GLfloat width,
		  int r, int g, int b, int a, int d1, int d2);
void drawSquare(GLfloat x1, GLfloat y1, GLfloat z1, 
		GLfloat x2, GLfloat y2, GLfloat z2, 
		GLfloat x3, GLfloat y3, GLfloat z3, 
		GLfloat x4, GLfloat y4, GLfloat z4, 
		int r, int g, int b);
void drawStar(int f, GLfloat x, GLfloat y, GLfloat z, int r, int g, int b, float size);
void drawLaser(GLfloat x, GLfloat y, GLfloat width, GLfloat height,
	       int cc1, int cc2, int cc3, int cc4, int cnt, int type);
void drawCore(GLfloat x, GLfloat y, int cnt, int r, int g, int b);
void drawShipShape(GLfloat x, GLfloat y, float d, int inv);
void drawBomb(GLfloat x, GLfloat y, GLfloat width, int cnt);
void drawCircle(GLfloat x, GLfloat y, GLfloat width, int cnt, 
		int r1, int g1, int b1, int r2, int b2, int g2);
void drawShape(GLfloat x, GLfloat y, GLfloat size, int d, int cnt, int type,
	       int r, int g, int b);
void drawShapeIka(GLfloat x, GLfloat y, GLfloat size, int d, int cnt, int type, int c);
void drawShot(GLfloat x, GLfloat y, GLfloat d, int c, float width, float height);
void startDrawBoards();
void endDrawBoards();
void drawSideBoards();
void drawTitleBoard();

int drawNum(int n, int x ,int y, int s, int r, int g, int b);
int drawNumRight(int n, int x ,int y, int s, int r, int g, int b);
int drawNumCenter(int n, int x ,int y, int s, int r, int g, int b);
int drawTimeCenter(int n, int x ,int y, int s, int r, int g, int b);
