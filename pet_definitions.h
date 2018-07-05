#ifndef PET_DEFINITIONS_H
#define PET_DEFINITIONS_H

#include "share.h"

#define MAX_SPRITE_FRAMES 10

struct pet_sprite {
	VMINT8 width, height;
	VMINT8 frame_count;
	VMINT16 frame_rate; /* zero means not animated */
	char frames[MAX_SPRITE_FRAMES][MRE_STRING_MAX_SIZE];
};

struct position_and_scale_adjustments {
	VMINT8 x_offset;
	VMINT8 y_offset;
	VMINT16 scale;
};

struct watch_due_pet {
	struct pet_sprite sprite;

	struct {
		struct position_and_scale_adjustments floating_status;
		struct position_and_scale_adjustments pet_sprite;
	} view_main_adjustments;
};

#ifdef PETS_SETUP

static struct watch_due_pet pet_flame_def
	= 
	{	{ /* sprite info* /
			/* width */			80, /* height */	70, 
			/* frame count */	6,	/* frame rate*/ 200, 

			/* frame resources */
			{	
				"flame_0.gif", "flame_1.gif", "flame_2.gif",
				"flame_3.gif", "flame_4.gif", "flame_5.gif" 
			}
		},

		{ /* main view adjustments */
			{ /* floating status */
				/* x offset */	1, /* y offset */	-5,
				/* scale */		200
			},
			{ /* pet sprite */
				/* x offset */	-8, /* y offset */	35,
				/* scale */		150
			}
		}
	};

#undef PETS_SETUP
#endif

#endif