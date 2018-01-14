/*
 * $Id: foecommand.h,v 1.1.1.1 2003/03/16 07:03:49 kenta Exp $
 *
 * Copyright 2003 Kenta Cho. All rights reserved.
 */

/**
 * Foe commands data.
 *
 * @version $Revision: 1.1.1.1 $
 */
#ifndef FOECOMMAND_H_
#define FOECOMMAND_H_

#include "bulletml/bulletmlparser.h"
#include "bulletml/bulletmlparser-tinyxml.h"
#include "bulletml/bulletmlrunner.h"
#include "foe.h"

#define COMMAND_SCREEN_SPD_RATE 512
#define COMMAND_SCREEN_VEL_RATE 512

class FoeCommand : public BulletMLRunner {
 public:
  FoeCommand(BulletMLParser* parser, struct foe* f);
  FoeCommand(BulletMLState* state, struct foe* f);

  virtual ~FoeCommand();

  virtual double getBulletDirection();
  virtual double getAimDirection();
  virtual double getBulletSpeed();
  virtual double getDefaultSpeed();
  virtual double getRank();
  virtual void createSimpleBullet(double direction, double speed);
  virtual void createBullet(BulletMLState* state, double direction, double speed);
  virtual int getTurn();
  virtual void doVanish();
  
  virtual void doChangeDirection(double d);
  virtual void doChangeSpeed(double s);
  /*virtual void FoeCommand::doAccelX(double ax);
  virtual void FoeCommand::doAccelY(double ay);
  virtual double FoeCommand::getBulletSpeedX();
  virtual double FoeCommand::getBulletSpeedY();*/
  virtual void doAccelX(double ax);
  virtual void doAccelY(double ay);
  virtual double getBulletSpeedX();
  virtual double getBulletSpeedY();
  
 private:
  struct foe *foe;
};
#endif


