/*
 * $Id: soundmanager.h,v 1.1.1.1 2003/03/16 07:03:49 kenta Exp $
 *
 * Copyright 2003 Kenta Cho. All rights reserved.
 */

/**
 * BGM/SE manager header file.
 *
 * @version $Revision: 1.1.1.1 $
 */
void closeSound();
void initSound();
void playMusic(int idx);
void fadeMusic();
void stopMusic();
void playChunk(int idx);
void haltChunk(int idx);
