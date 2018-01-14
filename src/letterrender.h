/*
 * $Id: letterrender.h,v 1.1.1.1 2003/03/16 07:03:49 kenta Exp $
 *
 * Copyright 2002 Kenta Cho. All rights reserved.
 */

/**
 * Letter render header file.
 *
 * @version $Revision: 1.1.1.1 $
 */

void drawLetter(int idx, int lx, int ly, int ltSize, int d,
		int r, int g, int b);
void drawString(char *str, int lx, int ly, int ltSize, int d, 
		int r, int g, int b);
