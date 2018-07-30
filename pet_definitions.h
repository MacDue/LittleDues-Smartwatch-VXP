#ifndef PET_DEFINITIONS_H
#define PET_DEFINITIONS_H

#include "share.h"
#include "macro_utils.h"
#include "sprites.h"

#define MAX_SPRITE_FRAMES 10

#define P_TYPES(GEN)	\
	GEN(LOGICAL)		\
	GEN(PEACEFUL)		\
	GEN(EVIL)			\
	GEN(QUIET)			\
	GEN(LOUD)			\
	GEN(GLUTTON)		\
	GEN(DOG)			\
	GEN(ODD)			

static const char P_TYPE_STRS[8 /* mood count */][10 /* length of "PEACEFUL" + 1*/] = {
	P_TYPES(GENERATE_STRING)
};

// Very limited set of moods for testing. From bad to good mood.
#define MOODS(GEN)		\
	GEN(SAD)			\
	GEN(HAPPY)			

#define GOOD_BAD_MOOD_TIPPING_POINT 0

static const char MOOD_STRS[2 /* mood count */][6 /* length of "HAPPY" + 1 */] = {
	MOODS(GENERATE_STRING)
};

struct pet_sprite {
	VMUINT8 width, height;
	VMUINT8 frame_count;
	VMUINT16 frame_rate; /* zero means not animated */
	char frames[MAX_SPRITE_FRAMES][MRE_STRING_MAX_SIZE];
};

struct position_and_scale_adjustments {
	VMINT16 x_offset;
	VMINT16 y_offset;
	VMUINT16 scale;
	VMINT16 padding;
};


static enum { P_TYPES(GENERATE_ENUM) } 
get_personality_type(VMUINT8 mood, VMUINT8 hunger) {
	// Completely arbitrary (a bit crap)
	if ((mood) <= 60 && hunger <= 60 
		&& abs(mood - hunger) <= 20) {
		return LOGICAL;
	} else if (mood <= 25 && hunger <= 28) {
		return QUIET;
	} else if (abs(mood - hunger*2) < 50) {
		return PEACEFUL;
	} else if (hunger/(float)mood >= 1.5f) {
		return EVIL;
	} else if (mood > 90 && hunger > 90) {
		return DOG;
	} else if (mood > 10 && hunger > 20) {
		return GLUTTON;
	} else {
		return ODD;
	}
}

static enum { MOODS(GENERATE_ENUM) }
get_current_mood(VMUINT8 mood, VMUINT8 hunger) {
	// Complex emotional state (will be expaneded on).
	return mood < PET_STAT_MAX/2 || hunger < PET_STAT_MAX/3 ? SAD : HAPPY;
}

struct watch_due_pet {

	VMINT id;

	char species[MRE_STRING_MAX_SIZE];

	struct sprite sprite;
	struct sprite dead_sprite;
	VMINT acceptable_foods[PET_ITEMS_MAX];

	struct {
		struct position_and_scale_adjustments floating_status;
		struct position_and_scale_adjustments pet_sprite;
		struct position_and_scale_adjustments floor_disc;
	} view_main_adjustments;

	struct {
		struct position_and_scale_adjustments pet_sprite;
	} view_info_adjustments;

};

#ifdef PETS_SETUP

#define PET_ENTRIES 2

static const struct watch_due_pet PET_DB[PET_ENTRIES] 
	= 
	{ /* id = 0 - flame pet */
		{	
			/* id */ 0,
			/* species */ "FLAME",
			{ /* alive sprite */
				/* x */ 0, /* y */	26, /* width */ 11, /* height*/ 14, /* frames*/ 6
			},
			{ /* dead sprite */
						66,			26,				13,				14,				1
			},
			{ /* Foods */
				1, 2, 3, 2
			},
			{ /* main view adjustments */
				{ /* floating status */
					/* x offset */	1, /* y offset */	-5,
					/* scale */		4
				},
				{ /* pet sprite */
					/* x offset */	-5, /* y offset */	35,
					/* scale */		7,	/* padding */	16
				},
				{ /* floor disc */
					/* x offset */	0, /* y offset */	95,
					/* scale */		4
				}
			},

			{ /* info view adjustments */ 
				{ /* pet sprite */
					/* x offset */	0, /* y offset */	16,
					/* scale */		5
				}
			}
		},
		{ /* id = 1 - due pet */
			/* id */ 1,
			/* species */ "GOD",
			{ /* alive sprite */
				/* x */ 47, /* y */	0, /* width */	16, /* height*/ 16, /* frames*/ 2
			},
			{ /* dead sprite */
						31,			0,				16,				16,				1
			},
			{ /* Foods */
				4, 5, 6, 7
			},
			{ /* main view adjustments */
				{ /* floating status */
					/* x offset */	1, /* y offset */	-5,
					/* scale */		4
				},
				{ /* pet sprite */
					/* x offset */	-5, /* y offset */	35,
					/* scale */		7,	/* padding */	0
				},
				{ /* floor disc */
					/* x offset */	0, /* y offset */	95,
					/* scale */		4
				}
			},

			{ /* info view adjustments */ 
				{ /* pet sprite */
					/* x offset */	0, /* y offset */	16,
					/* scale */		5
				}
			}
		}
	};

struct pet_item {
	struct sprite sprite;
	VMINT drop_chance;
	VMINT nutrition;
};

#define ITEM_ENTRIES 8

static const struct pet_item PET_ITEMS[ITEM_ENTRIES] 
	=
	{
		{ /* id 0 */
			/* No item */
			{0, 18, 8, 8, 1}, 0, 0
		},
		{ /* id 1 */
			/* Amber */
			/* sprite */		{8, 18, 8, 8, 1},
			/* drop chance */	20,
			/* nutrition*/		5
		},
		{ /* id 2 */
			/* Coal */
			/* sprite */		{16, 18, 8, 8, 1},
			/* drop chance */	5,
			/* nutrition */		12
		},
		{ /* id 3 */
			/* Dead bush */
			/* sprite */		{24, 18, 8, 8, 1},
			/* drop chance */	10,
			/* nutrition */		3
		},
		{ /* id 4 */
			/* Cooked meat */
			/* sprite */		{32, 18, 8, 8, 1},
			/* drop chance */	4,
			/* nutrition */		15
		},
		{ /* id 5 */
			/* Raw meat */
			/* sprite */		{40, 18, 8, 8, 1},
			/* drop chance */	16,
			/* nutrition */		9
		},
		{ /* id 6 */
			/* Melon */
			/* sprite */		{48, 18, 8, 8, 1},
			/* drop chance */	20,
			/* nutrition */		2
		},
		{ /* id 7 */
			/* Mushroom */
			/* sprite */		{56, 18, 8, 8, 1},
			/* drop chance */	20,
			/* nutrition */		5
		}
	};



#define GIF_GCT_FLAG(p_byte) (p_byte & (1 << 7))
#define GIF_COLOUR_RES(p_byte) (p_byte & (7 << 5))
#define GIF_GCT_SIZE(p_byte) (p_byte & 7)


void glitch_pet_resource(VMUINT8* gif_res, VMUINT8 glitch_val) {
	VMUINT8 packing_byte;
	VMINT colour_table_len;
	VMINT c;

	if (strncmp("GIF89a", gif_res, 6) != 0) {
		return;
	}
	packing_byte = *(gif_res+10);
	if (!GIF_GCT_FLAG(packing_byte)) {
		return;
	}
	c = GIF_GCT_SIZE(packing_byte) + 1;
	colour_table_len = 1 << (GIF_GCT_SIZE(packing_byte) +1);
	for (c = 0; c < colour_table_len; c++) {
		*(gif_res+13+c) ^= glitch_val;
	}
}


#undef PETS_SETUP
#endif

#endif