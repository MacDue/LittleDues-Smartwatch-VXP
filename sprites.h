#ifndef SPRITES_H
#define SPRITES_H

#include "vmsys.h"

#define SPRITE_SHEET "sprites.bmp"
#define SPRITE_SHEET_W 79
#define SPRITE_SHEET_H 40

struct SpriteSimple {
	VMUINT16 x, y, w, h;
};

struct SpritePart {
	VMUINT16 x, y, w, h;
};

struct RenderPart {
	VMUINT8 part_index;
	VMUINT8 r_x, r_y;
};

struct SpritePackedFrame {
	VMUINT8 part_count;
	struct RenderPart const * render_parts;
};

struct SpritePacked {
	VMUINT16 w, h;
	VMUINT8 frame_count;
	struct SpritePart const * parts;
	struct SpritePackedFrame const * frames; 
};

enum SpriteType { SPRITE_SIMPLE, SPRITE_PACKED };
struct SpritePtr {
	union { struct SpritePacked const * packed_sprite; struct SpriteSimple const * simple_sprite; } ptr;
	enum SpriteType type;
};

#ifdef SPRITES_SETUP 

__inline struct SpritePtr 
sprite_ptr(void const * const real_sprite_ptr, enum SpriteType type) {
	struct SpritePtr ptr;
	ptr.ptr.packed_sprite = real_sprite_ptr;
	ptr.type = type;
	return ptr;
}

#define SPRITE_PACKED_SET_FRAME(frame, _render_parts)                      \
	(frame).part_count = (sizeof(_render_parts)/sizeof(_render_parts[0])); \
	(frame).render_parts = (_render_parts);

////////////////////////// FOOD //////////////////////////

struct SpritePacked SPRITE_FOOD_PACKED = {
	/* w */ 8, /* h */ 8,
	/* frame count */ ITEM_ENTRIES
};

static void
init_food_sprites(void) {
	static struct SpritePart const food_sprite_pixels[] = 
	{ 
	/* no item */	{ /* x */ 31, /* y */ 12, /* w */ 8, /* h */ 8 },
	/* amber */	{ /* x */ 69, /* y */ 0, /* w */ 4, /* h */ 6 },
	/* coal */	{ /* x */ 68, /* y */ 6, /* w */ 7, /* h */ 6 },
	/* bush */	{ /* x */ 69, /* y */ 12, /* w */ 6, /* h */ 5 },
	/* ham */	{ /* x */ 73, /* y */ 0, /* w */ 6, /* h */ 6 },
	/* person 0 */	{ /* x */ 57, /* y */ 0, /* w */ 6, /* h */ 8 },
	/* person 1 */	{ /* x */ 63, /* y */ 0, /* w */ 6, /* h */ 8 },
	/* person 2 */  { /* x */ 63, /* y */ 8, /* w */ 6, /* h */ 8 },
	/* person 3 */ { /* x */ 57, /* y */ 8, /* w */ 6, /* h */ 8 },
	};

	static struct RenderPart const no_item[] = {
		{ 0, /* r_x */ 0, /* r_y */ 0 }
	};
	static struct RenderPart const amber[] = {
		{ 1, /* r_x */ 2, /* r_y */ 1 }
	};
	static struct RenderPart const coal[] = {
		{ 2, /* r_x */ 0, /* r_y */ 1 }
	};
	static struct RenderPart const bush[] = {
		{ 3, /* r_x */ 1, /* r_y */ 2 }
	};
	static struct RenderPart const ham[] = {
		{ 4, /* r_x */ 1, /* r_y */ 1 }
	};
	static struct RenderPart const person_0[] = {
		{ 5, /* r_x */ 1, /* r_y */ 0 }
	};
	static struct RenderPart const person_1[] = {
		{ 6, /* r_x */ 1, /* r_y */ 0 }
	};
	static struct RenderPart const person_2[] = {
		{ 7, /* r_x */ 1, /* r_y */ 0 }
	};
	static struct RenderPart const person_3[] = {
		{ 8, /* r_x */ 1, /* r_y */ 0 }
	};

	static struct SpritePackedFrame food_sprites[ITEM_ENTRIES];
	SPRITE_PACKED_SET_FRAME(food_sprites[0], no_item);
	SPRITE_PACKED_SET_FRAME(food_sprites[1], amber);
	SPRITE_PACKED_SET_FRAME(food_sprites[2], coal);
	SPRITE_PACKED_SET_FRAME(food_sprites[3], bush);
	SPRITE_PACKED_SET_FRAME(food_sprites[4], ham);
	SPRITE_PACKED_SET_FRAME(food_sprites[5], person_0);
	SPRITE_PACKED_SET_FRAME(food_sprites[6], person_1);
	SPRITE_PACKED_SET_FRAME(food_sprites[7], person_2);
	SPRITE_PACKED_SET_FRAME(food_sprites[8], person_3);

	SPRITE_FOOD_PACKED.parts = food_sprite_pixels;
	SPRITE_FOOD_PACKED.frames = food_sprites;
}

////////////////////////// FLAME //////////////////////////

struct SpritePacked SPRITE_FLAME_PACKED = {
	/* w */ 16, /* h */ 16,
	/* frame count */ 6
};

static void
init_flame_sprite(void) {
	static struct SpritePart const flame_parts[] = 
	{ 
	/* 0 side l f 0 */ { /* x */ 0, /* y */ 35, /* w */ 3, /* h */ 5 },
	/* 1 side r f 0 */ { /* x */ 3, /* y */ 35, /* w */ 3, /* h */ 5 },
	/* 2 side l f 1 */ { /* x */ 6, /* y */ 35, /* w */ 3, /* h */ 5 },
	/* 3 side r f 1 */ { /* x */ 9, /* y */ 35, /* w */ 3, /* h */ 5 },
	/* 4 side l f 2 */ { /* x */ 12, /* y */ 35, /* w */ 3, /* h */ 5 },
	/* 5 side r f 2 */ { /* x */ 15, /* y */ 35, /* w */ 3, /* h */ 5 },
	/* 6 side l f 3 */ { /* x */ 18, /* y */ 35, /* w */ 3, /* h */ 5 },
	/* 7 side r f 3 */ { /* x */ 21, /* y */ 35, /* w */ 3, /* h */ 5 },
	/* 8 side l f 4 */ { /* x */ 24, /* y */ 35, /* w */ 3, /* h */ 5 },
	/* 9 side r f 4 */ { /* x */ 27, /* y */ 35, /* w */ 3, /* h */ 5 },
	/* 10 side l f 5 */ { /* x */ 30, /* y */ 35, /* w */ 3, /* h */ 5 },
	/* 11 side r f 5 */ { /* x */ 33, /* y */ 35, /* w */ 3, /* h */ 5 },

	/* 12 face */ { /* x */ 36, /* y */ 35, /* w */ 5, /* h */ 5 },
	/* 13 base */ { /* x */ 41, /* y */ 35, /* w */ 8, /* h */ 2 },

	/* 14 flame top 0 */ { /* x */ 5, /* y */ 28, /* w */ 11, /* h */ 7 },
	/* 15 flame top 1 */ { /* x */ 16, /* y */ 28, /* w */ 11, /* h */ 7 },
	/* 16 flame top 2 */ { /* x */ 27, /* y */ 28, /* w */ 11, /* h */ 7 },
	/* 17 flame top 3 */ { /* x */ 38, /* y */ 28, /* w */ 11, /* h */ 7 },
	/* 18 flame top 4 */ { /* x */ 49, /* y */ 28, /* w */ 11, /* h */ 7 },
	/* 19 flame top 5 */ { /* x */ 60, /* y */ 28, /* w */ 11, /* h */ 7 },

	/* 20 flame line 2 */ { /* x */ 0, /* y */ 32, /* w */ 5, /* h */ 1 },
	/* 21 flame line 3 */ { /* x */ 0, /* y */ 33, /* w */ 5, /* h */ 1 },
	/* 22 flame line 4 */ { /* x */ 0, /* y */ 34, /* w */ 5, /* h */ 1 },

	/* 23 dead */ { /* x */ 49, /* y */ 35, /* w */ 13, /* h */ 4 }
	};

	// FRAME 0	
	static struct RenderPart const flame_frame_0[] = {
		{ /* base */ 13, /* r_x */ 4, /* r_y */ 13 },
		{ /* face */ 12, /* r_x */ 6, /* r_y */ 8 },
		{ /* side l f 0 */ 0, /* r_x */ 3, /* r_y */ 8 },
		{ /* side r f 0 */ 1, /* r_x */ 11, /* r_y */ 8  },
		{ /* flame top 0 */ 14, /* r_x */ 3, /* r_y */ 1  },
	};

	// FRAME 1
	static struct RenderPart const flame_frame_1[] = {
		{ /* base */ 13, /* r_x */ 4, /* r_y */ 13 },
		{ /* face */ 12, /* r_x */ 6, /* r_y */ 8 },
		{ /* side l f 1 */ 2, /* r_x */ 3, /* r_y */ 8 },
		{ /* side r f 1 */ 3, /* r_x */ 11, /* r_y */ 8  },
		{ /* flame top 1 */ 15, /* r_x */ 3, /* r_y */ 1  },
	};

	// FRAME 2	
	static struct RenderPart const flame_frame_2[] = {
		{ /* base */ 13, /* r_x */ 4, /* r_y */ 13 },
		{ /* flame line 2 */ 20, /* r_x */ 6, /* r_y */ 8 },
		{ /* face */ 12, /* r_x */ 6, /* r_y */ 9 },
		{ /* side l f 2 */ 4, /* r_x */ 3, /* r_y */ 8 },
		{ /* side r f 2 */ 5, /* r_x */ 11, /* r_y */ 8  },
		{ /* flame top 2 */ 16, /* r_x */ 3, /* r_y */ 1  },
	};

	// FRAME 3	
	static struct RenderPart const flame_frame_3[] = {
		{ /* base */ 13, /* r_x */ 4, /* r_y */ 13 },
		{ /* flame line 3 */ 21, /* r_x */ 6, /* r_y */ 8 },
		{ /* face */ 12, /* r_x */ 6, /* r_y */ 9 },
		{ /* side l f 3 */ 6, /* r_x */ 3, /* r_y */ 8 },
		{ /* side r f 3 */ 7, /* r_x */ 11, /* r_y */ 8  },
		{ /* flame top 3 */ 17, /* r_x */ 3, /* r_y */ 1  },
	};

	// FRAME 4	
	static struct RenderPart const flame_frame_4[] = {
		{ /* base */ 13, /* r_x */ 4, /* r_y */ 13 },
		{ /* flame line 4 */ 22, /* r_x */ 6, /* r_y */ 8 },
		{ /* face */ 12, /* r_x */ 6, /* r_y */ 9 },
		{ /* side l f 4 */ 8, /* r_x */ 3, /* r_y */ 8 },
		{ /* side r f 4 */ 9, /* r_x */ 11, /* r_y */ 8  },
		{ /* flame top 4 */ 18, /* r_x */ 3, /* r_y */ 1  },
	};

	// FRAME 5
	static struct RenderPart const flame_frame_5[] = {
		{ /* base */ 13, /* r_x */ 4, /* r_y */ 13 },
		{ /* face */ 12, /* r_x */ 6, /* r_y */ 8 },
		{ /* side l f 5 */ 10, /* r_x */ 3, /* r_y */ 8 },
		{ /* side r f 5 */ 11, /* r_x */ 11, /* r_y */ 8  },
		{ /* flame top 5 */ 19, /* r_x */ 3, /* r_y */ 1  },
	};

	// FRAME DEAD
	static struct RenderPart const flame_dead[] = {
		{ /* base */ 23, /* r_x */ 1, /* r_y */ 10 },
	};

	static struct SpritePackedFrame flame_frames[7];
	SPRITE_PACKED_SET_FRAME(flame_frames[0], flame_frame_0);
	SPRITE_PACKED_SET_FRAME(flame_frames[1], flame_frame_1);
	SPRITE_PACKED_SET_FRAME(flame_frames[2], flame_frame_2);
	SPRITE_PACKED_SET_FRAME(flame_frames[3], flame_frame_3);
	SPRITE_PACKED_SET_FRAME(flame_frames[4], flame_frame_4);
	SPRITE_PACKED_SET_FRAME(flame_frames[5], flame_frame_5);
	SPRITE_PACKED_SET_FRAME(flame_frames[6], flame_dead);

	SPRITE_FLAME_PACKED.parts = flame_parts;
	SPRITE_FLAME_PACKED.frames = flame_frames;
}

////////////////////////// DUE //////////////////////////

struct SpritePacked SPRITE_DUE_PACKED = {
	/* w */ 16, /* h */ 16,
	/* frame count */ 2
};

static void
init_due_sprite(void) {
	static struct SpritePart const due_parts[] = 
	{ 
	/* body */	{ /* x */ 43, /* y */ 0, /* w */ 14, /* h */ 14 },
	/* wing down l */ { /* x */ 39, /* y */ 14, /* w */ 4, /* h */ 3 },
	/* wing down r */ { /* x */ 43, /* y */ 14, /* w */ 4, /* h */ 3 },
	/* wing up l */	{ /* x */ 47, /* y */ 14, /* w */ 5, /* h */ 3 },
	/* wing up r */	{ /* x */ 52, /* y */ 14, /* w */ 5, /* h */ 3 },
	/* dead */ { /* x */ 29, /* y */ 0, /* w */ 14, /* h */ 12 }
	};

	// FRAME 0	
	static struct RenderPart const due_frame_0[] = {
		{ /* body */ 0, /* r_x */ 1, /* r_y */ 2 },
		{ /* wing down l */ 1, /* r_x */ 1, /* r_y */ 1 },
		{ /* wing down r */ 2, /* r_x */ 11, /* r_y */ 1 },
	};

	// FRAME 1
	static struct RenderPart const due_frame_1[] = {
		{ /* body */ 0, /* r_x */ 1, /* r_y */ 1 },
		{ /* wing down l */ 3, /* r_x */ 0, /* r_y */ 0 },
		{ /* wing down r */ 4, /* r_x */ 11, /* r_y */ 0 },
	};
	
	// FRAME DEAD
	static struct RenderPart const due_dead[] = {
		{ /* dead */ 5, /* r_x */ 1, /* r_y */ 2 }
	};
	
	static struct SpritePackedFrame due_frames[3];
	SPRITE_PACKED_SET_FRAME(due_frames[0], due_frame_0);
	SPRITE_PACKED_SET_FRAME(due_frames[1], due_frame_1);
	SPRITE_PACKED_SET_FRAME(due_frames[2], due_dead);

	SPRITE_DUE_PACKED.parts = due_parts;
	SPRITE_DUE_PACKED.frames = due_frames;
}

////////////////////////// LILAC DRAGON //////////////////////////

struct SpritePacked SPRITE_LILAC_DRAGON_PACKED = {
	/* w */ 16, /* h */ 16,
	/* frame count */ 4
};

static void
init_lilac_dragon(void) {
	static struct SpritePart const lilac_dragon_parts[] = 
	{ 
	/* body */	{ /* x */ 57, /* y */ 17, /* w */ 15, /* h */ 11 },
	/* wing up */ { /* x */ 72, /* y */ 17, /* w */ 7, /* h */ 6 },
	/* wing down */ { /* x */ 72, /* y */ 23, /* w */ 7, /* h */ 5 },
	/* dead */ { /* x */ 42, /* y */ 22, /* w */ 15, /* h */ 6 },
	};

	// FRAME 0
	static struct RenderPart const lilac_dragon_frame_0[] = {
		{ /* body */ 0, /* r_x */ 0, /* r_y */ 4 },
		{ /* wing up */ 1, /* r_x */ 9, /* r_y */ 3 }
	};

	// FRAME 1
	static struct RenderPart const lilac_dragon_frame_1[] = {
		{ /* body */ 0, /* r_x */ 0, /* r_y */ 5 },
		{ /* wing up */ 1, /* r_x */ 9, /* r_y */ 4 }
	};

	// FRAME 2
	static struct RenderPart const lilac_dragon_frame_2[] = {
		{ /* body */ 0, /* r_x */ 0, /* r_y */ 5 },
		{ /* wing down */ 2, /* r_x */ 9, /* r_y */ 5 }
	};

	// FRAME 3
	static struct RenderPart const lilac_dragon_frame_3[] = {
		{ /* body */ 0, /* r_x */ 0, /* r_y */ 4 },
		{ /* wing down */ 2, /* r_x */ 9, /* r_y */ 4 }
	};

	// FRAME DEAD
	static struct RenderPart const lilac_dragon_dead[] = {
		{ /* body */ 3, /* r_x */ 0, /* r_y */ 10 }
	};
	
	static struct SpritePackedFrame lilac_dragon_frames[5];
	SPRITE_PACKED_SET_FRAME(lilac_dragon_frames[0], lilac_dragon_frame_0);
	SPRITE_PACKED_SET_FRAME(lilac_dragon_frames[1], lilac_dragon_frame_1);
	SPRITE_PACKED_SET_FRAME(lilac_dragon_frames[2], lilac_dragon_frame_2);
	SPRITE_PACKED_SET_FRAME(lilac_dragon_frames[3], lilac_dragon_frame_3);
	SPRITE_PACKED_SET_FRAME(lilac_dragon_frames[4], lilac_dragon_dead);

	SPRITE_LILAC_DRAGON_PACKED.parts = lilac_dragon_parts;
	SPRITE_LILAC_DRAGON_PACKED.frames = lilac_dragon_frames;
}

////////////////////////// SETUP //////////////////////////

static void
init_sprites(void) {
	init_food_sprites();
	init_flame_sprite();
	init_due_sprite();
	init_lilac_dragon();
}

#else

extern struct SpritePacked SPRITE_FOOD_PACKED;
extern struct SpritePacked SPRITE_FLAME_PACKED;
extern struct SpritePacked SPRITE_DUE_PACKED;
extern struct SpritePacked SPRITE_LILAC_DRAGON_PACKED;

#endif
#undef SPRITES_SETUP

#endif