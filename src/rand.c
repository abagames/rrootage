/*
 * $Id: rand.c,v 1.1.1.1 2003/03/16 07:03:49 kenta Exp $
 *
 * Copyright 2003 Kenta Cho. All rights reserved.
 */

/**
 * Make random number(Using Mersenne Twister).
 *
 * @version $Revision: 1.1.1.1 $
 */
#include "rand.h"

extern void init_genrand(unsigned long s);
extern unsigned long genrand_int32();

void setSeed(unsigned long s) {
  init_genrand(s);
}

unsigned long nextRand() {
  return genrand_int32();
}
