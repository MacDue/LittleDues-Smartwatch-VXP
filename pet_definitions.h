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

static char const P_TYPE_STRS[8 /* mood count */][10 /* length of "PEACEFUL" + 1*/] = {
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

struct PositionAndScaleAdjustments {
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

struct WatchDuePet {

	VMINT id;

	char species[MRE_STRING_MAX_SIZE];

	struct SpritePtr sprite;
	VMINT acceptable_foods[PET_ITEMS_MAX];

	struct {
		struct PositionAndScaleAdjustments floating_status;
		struct PositionAndScaleAdjustments pet_sprite;
		struct PositionAndScaleAdjustments floor_disc;
	} view_main_adjustments;

	struct {
		struct PositionAndScaleAdjustments pet_sprite;
	} view_info_adjustments;

};

#ifdef PETS_SETUP

#define FLAME_PET 0
#define DUE_PET 1
#define LILAC_DRAGON_PET 2

static struct WatchDuePet PET_DB[PET_ENTRIES] 
	= 
	{ /* id = 0 - flame pet */
		{	
			/* id */ 0,
			/* species */ "FLAME",
			/* sprite */
			{ { NULL }, SPRITE_PACKED }, /* to be loaded */
			{ /* Foods */
				1, 2, 3, 2
			},
			{ /* main view adjustments */
				{ /* floating status */
					/* x offset */	1, /* y offset */	-5,
					/* scale */		4
				},
				{ /* pet sprite */
					/* x offset */	-9, /* y offset */	30,
					/* scale */		7,	/* padding */	0
				},
				{ /* floor disc */
					/* x offset */	0, /* y offset */	95,
					/* scale */		4
				}
			},

			{ /* info view adjustments */ 
				{ /* pet sprite */
					/* x offset */	0, /* y offset */	10,
					/* scale */		5
				}
			}
		},
		{ /* id = 1 - due pet */
			/* id */ 1,
			/* species */ "GOD",
			/* sprite */
			{ { NULL }, SPRITE_PACKED }, /* to be loaded */
			{ /* Foods */
				5, 6, 7, 8
			},
			{ /* main view adjustments */
				{ /* floating status */
					/* x offset */	1, /* y offset */	-5,
					/* scale */		4
				},
				{ /* pet sprite */
					/* x offset */	-5, /* y offset */	25,
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
		},
		{ /* id = 2 - lilac dragon*/
			/* id */ 2,
			/* species */ "DRAGON",
			/* sprite */
			{ { NULL }, SPRITE_PACKED }, /* to be loaded */
			{ /* Foods */
				4, 8, 5, 7
			},
			{ /* main view adjustments */
				{ /* floating status */
					/* x offset */	1, /* y offset */	-5,
					/* scale */		4
				},
				{ /* pet sprite */
					/* x offset */	-5, /* y offset */	15,
					/* scale */		7,	/* padding */	0
				},
				{ /* floor disc */
					/* x offset */	0, /* y offset */	95,
					/* scale */		4
				}
			},

			{ /* info view adjustments */ 
				{ /* pet sprite */
					/* x offset */	0, /* y offset */	5,
					/* scale */		5
				}
			}
		}
	};

struct PetItem {
	VMINT drop_chance;
	VMINT nutrition;
};

static struct PetItem const PET_ITEMS[ITEM_ENTRIES] 
	=
	{
		{ /* id 0 */
			/* No item */
			0, 0
		},
		{ /* id 1 */
			/* Amber */
			/* drop chance */	20,
			/* nutrition*/		5
		},
		{ /* id 2 */
			/* Coal */
			/* drop chance */	5,
			/* nutrition */		12
		},
		{ /* id 3 */
			/* Dead bush */
			/* drop chance */	10,
			/* nutrition */		3
		},
		{ /* id 4 */
			/* Cooked meat */
			/* drop chance */	5,
			/* nutrition */		12
		},
		{ /* id 5 */
			/* Human 1 */
			/* drop chance */	9,
			/* nutrition */		6
		},
		{ /* id 6 */
			/* Human 2 */
			/* drop chance */	10,
			/* nutrition */		11
		},
		{ /* id 7 */
			/* Human 3 */
			/* drop chance */	7,
			/* nutrition */		8
		},
		{ /* id 8 */
			/* Human 4 */
			/* drop chance */	10,
			/* nutrition */		3
		}
	};


static void 
init_pets(void) {
	PET_DB[FLAME_PET].sprite.ptr.packed_sprite = &SPRITE_FLAME_PACKED;
	PET_DB[DUE_PET].sprite.ptr.packed_sprite = &SPRITE_DUE_PACKED;
	PET_DB[LILAC_DRAGON_PET].sprite.ptr.packed_sprite = &SPRITE_LILAC_DRAGON_PACKED;
}

#undef PETS_SETUP
#endif

#endif