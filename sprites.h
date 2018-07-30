#ifndef SPRITES_H
#define SPRITES_H

#include "vmsys.h"

#define SPRITE_SHEET_W 79
#define SPRITE_SHEET_H 40

struct sprite {
	VMUINT16 x,y, w, h;
	VMUINT8 frame_count;
};

#endif