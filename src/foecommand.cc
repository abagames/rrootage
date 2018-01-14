/*
 * $Id: foecommand.cc,v 1.2 2003/08/15 07:06:52 kenta Exp $
 *
 * Copyright 2003 Kenta Cho. All rights reserved.
 */

/**
 * Handle bullet commands.
 *
 * @version $Revision: 1.2 $
 */
#include "bulletml/bulletmlparser.h"
#include "bulletml/bulletmlparser-tinyxml.h"
#include "bulletml/bulletmlrunner.h"
#include "foe.h"

extern "C" {
#include "rr.h"
#include "genmcr.h"
#include "degutil.h"
#include "ship.h"
}

FoeCommand::FoeCommand(BulletMLParser *parser, Foe *f)
  : BulletMLRunner(parser) {
  foe = f;
}

FoeCommand::FoeCommand(BulletMLState *state, Foe *f)
  : BulletMLRunner(state) {
  foe = f;
}

FoeCommand::~FoeCommand() {}

double FoeCommand::getBulletDirection() {
  return (double)foe->d*360/DIV;
}

double FoeCommand::getAimDirection() {
  int d = getPlayerDeg(foe->pos.x, foe->pos.y);
  if ( foe->xReverse == -1 ) d = (-d)&1023;
  return ((double)d*360/DIV);
}

double FoeCommand::getBulletSpeed() {
  return ((double)foe->spd)/COMMAND_SCREEN_SPD_RATE;
}

double FoeCommand::getDefaultSpeed() {
  return 1;
}

double FoeCommand::getRank() {
  return foe->rank;
}

void FoeCommand::createSimpleBullet(double direction, double speed) {
  int d = (int)(direction*DIV/360); d &= (DIV-1);
  addFoeNormalBullet(foe, d, (int)(speed*COMMAND_SCREEN_SPD_RATE), foe->color+1);
  foe->fireCnt++;
}

void FoeCommand::createBullet(BulletMLState* state, double direction, double speed) {
  int d = (int)(direction*DIV/360); d &= (DIV-1);
  addFoeActiveBullet(foe, d, (int)(speed*COMMAND_SCREEN_SPD_RATE), foe->color+1, state);
  foe->fireCnt++;
}

int FoeCommand::getTurn() {
  return tick;
}

void FoeCommand::doVanish() {
  removeFoeCommand(foe);
}

void FoeCommand::doChangeDirection(double d) {
  foe->d = (int)(d*DIV/360);
}

void FoeCommand::doChangeSpeed(double s) {
  foe->spd = (int)(s*COMMAND_SCREEN_SPD_RATE);
}

void FoeCommand::doAccelX(double ax) {
  foe->vel.x = (int)(ax*COMMAND_SCREEN_VEL_RATE);
}

void FoeCommand::doAccelY(double ay) {
  foe->vel.y = (int)(ay*COMMAND_SCREEN_VEL_RATE);
}

double FoeCommand::getBulletSpeedX() {
  return ((double)foe->vel.x/COMMAND_SCREEN_VEL_RATE);
}

double FoeCommand::getBulletSpeedY() {
  return ((double)foe->vel.y/COMMAND_SCREEN_VEL_RATE);
}
