/*
	Little Dues

	A little simple tamagotchi like game for Mediatek smartwatches.
	This is an experiment & mainly just a POC.

	Everything is a static method because that helps the compiler reduce the size of nuklear.
	The game is simple enough that that does not matter much.

	Also no memory allocations (outside of the few nuklear might do).

	Version: Super rushed & jumbled mess to test v0.0.1
*/

#include "vmsys.h"
#include "vmio.h"
#include "vmgraph.h"
#include "vmchset.h"
#include "vmstdlib.h"
#include "ResID.h"
#include "vm4res.h"
#include "vmtimer.h"
#include "macro_utils.h"

#define		DEBUG
#define		SUPPORT_BG

#if 0
  #define SPAM(a) printf a
#else
  #define SPAM(a) (void)0
#endif

VMINT		layer_hdl[1];

typedef enum {true = 1, false = 0} bool;

//////////// PET STATE & LOGIC

#define PET_ITEMS_MAX 4
#define PET_STAT_MAX 70

#define PETS_SETUP
#include "pet_definitions.h"

#define SAVE_FILE_NAME "ld_pet.sav"
VMWCHAR wide_save_file_name[MRE_FILE_NAME_SIZE];

static struct 
current_pet_state {
	VMUINT8 mood_trait, hunger_trait;
	VMUINT8 mood;
	VMUINT8 hunger;
	float age;
	VMINT items[PET_ITEMS_MAX];
	bool dead;
	const struct watch_due_pet* def;
	const struct sprite * sprite;
	VMUINT8 glitch;
	VMINT fed_item;
} current_pet_state;

static int pet_sprite_index	= 0;

static void
update_pet_frame(VMINT tid) {
	pet_sprite_index = ++pet_sprite_index 
							% current_pet_state.sprite->frame_count;
}

static void
save_pet_data(void) {
	VMWCHAR file_name_w[MRE_FILE_NAME_SIZE];
	VMINT file_hdl;
	VMUINT  written;
	VMUINT save_time;
	
	file_hdl = vm_file_open(wide_save_file_name, MODE_CREATE_ALWAYS_WRITE, true);
	if (file_hdl < 0)
		return; // error (not much can be done)
	(void) vm_get_curr_utc(&save_time);
	vm_file_write(file_hdl, (VMINT*)&current_pet_state.def->id,		 sizeof(VMINT),					&written);
	vm_file_write(file_hdl, &current_pet_state.mood,		 sizeof(VMUINT8),				&written);
	vm_file_write(file_hdl, &current_pet_state.mood_trait,	 sizeof(VMUINT8),				&written);
	vm_file_write(file_hdl, &current_pet_state.hunger,		 sizeof(VMUINT8),				&written);
	vm_file_write(file_hdl, &current_pet_state.hunger_trait, sizeof(VMUINT8),				&written);
	vm_file_write(file_hdl, &current_pet_state.age,			 sizeof(float),					&written);
	vm_file_write(file_hdl, &save_time,						 sizeof(VMUINT),				&written);
	vm_file_write(file_hdl, current_pet_state.items,	PET_ITEMS_MAX * sizeof(VMINT),		&written);
	vm_file_write(file_hdl, &current_pet_state.dead,			 sizeof(bool),				&written);
	vm_file_write(file_hdl, &current_pet_state.glitch,		sizeof(VMUINT8),				&written);
	vm_file_close(file_hdl);
}

#define TWENTY_MINS (60 * 20)

static void apply_time_to_game(VMUINT save_time) {
	VMUINT current_time;
	VMUINT time_diff;
	VMUINT time_attrition;
	VMINT mood_loss, hunger_increase, drop_chances, drop;

	(void) vm_get_curr_utc(&current_time); // assume the clock works

	// Pet aging
	time_diff = current_time - save_time + 10000;
	current_pet_state.age += (float)time_diff/60/60; // pet age is in hours
	
	time_attrition = (time_diff/2 + rand() % time_diff)/60;
	
	// Gives about 10 hours until the pet will die
	mood_loss = (VMINT)(time_attrition * (float)current_pet_state.mood_trait/1000);
	hunger_increase = (VMINT)(time_attrition * (float)current_pet_state.hunger_trait/1000);

	if (mood_loss < current_pet_state.mood) 
		current_pet_state.mood -= mood_loss;
	else 
		current_pet_state.mood = 0;

	if (hunger_increase < current_pet_state.hunger) 
		current_pet_state.hunger -= hunger_increase;
	else 
		current_pet_state.hunger = 0;

	if (current_pet_state.hunger < 5 && current_pet_state.mood < 5) {
		current_pet_state.dead = rand() % 100 < 80;	// 80% chance of death
	} else if (current_pet_state.mood < 5) {
		// suicide check
		current_pet_state.dead = rand() % current_pet_state.mood_trait * 3 < 40;
	} else if (current_pet_state.hunger < 5) {
		// stave check
		current_pet_state.dead = rand() % current_pet_state.hunger_trait * 2 < 50;
	}

	// Item drops
	// Chance every 5 mins
	drop_chances = time_diff/TWENTY_MINS;
	for (drop = 0; drop < drop_chances; drop++) {
		VMINT8 slot;
		VMINT dropped_item = 0;
		for (slot = 0; slot < PET_ITEMS_MAX; slot++) {
			VMINT item_id = current_pet_state.def->acceptable_foods[slot];
			if (dropped_item) break;
			dropped_item = item_id * ((rand() % 101) < PET_ITEMS[item_id].drop_chance); 
		}
		if (!dropped_item) {
			continue;
		}
		for (slot = 0; slot < PET_ITEMS_MAX; slot++) {
			if (current_pet_state.items[slot] == 0) {
				current_pet_state.items[slot] = dropped_item;
				break;
			} else if (slot == PET_ITEMS_MAX-1) {
				drop_chances = 0;
			}
		}
	}	
}


static void load_or_create_pet(void) {
	VMINT file_hdl;
	VMUINT read;
	VMUINT save_time;
	VMINT pet_id;

	current_pet_state.fed_item = 0;
	
	file_hdl = vm_file_open(wide_save_file_name, MODE_READ, true);
	if (file_hdl < 0) {
		// No pet data found.
		// Create new pet state
		current_pet_state.def			= &PET_DB[rand() % PET_ENTRIES];
		current_pet_state.age			= 0;
		current_pet_state.mood			= PET_STAT_MAX;
		current_pet_state.hunger		= PET_STAT_MAX;
		current_pet_state.mood_trait	= RAND(100) + 1;
		current_pet_state.hunger_trait	= RAND(100) + 1;
		current_pet_state.dead			= false;
		if (RAND(100) < 5) {
			current_pet_state.glitch = RAND(255) + 1;
		}
		memset(current_pet_state.items, 0, PET_ITEMS_MAX * sizeof(VMINT));
		current_pet_state.sprite = &current_pet_state.def->sprite;
		return;
	}

	vm_file_read(file_hdl, &pet_id,	sizeof(VMINT),				&read);
	vm_file_read(file_hdl, &current_pet_state.mood,			sizeof(VMUINT8),			&read);
	vm_file_read(file_hdl, &current_pet_state.mood_trait,	sizeof(VMUINT8),			&read);
	vm_file_read(file_hdl, &current_pet_state.hunger,		sizeof(VMUINT8),			&read);
	vm_file_read(file_hdl, &current_pet_state.hunger_trait,	sizeof(VMUINT8),			&read);
	vm_file_read(file_hdl, &current_pet_state.age,			sizeof(float),				&read);
	vm_file_read(file_hdl, &save_time,						sizeof(VMUINT),				&read);
	vm_file_read(file_hdl, current_pet_state.items,	PET_ITEMS_MAX * sizeof(VMINT),		&read);
	vm_file_read(file_hdl, &current_pet_state.dead,			sizeof(bool),				&read);
	vm_file_read(file_hdl, &current_pet_state.glitch,		sizeof(VMUINT8),			&read);
	vm_file_close(file_hdl);
	current_pet_state.def = &PET_DB[pet_id];
	current_pet_state.sprite = &current_pet_state.def->sprite;

	SPAM(("LOADED PET!\n"));
	SPAM(("ID: %d\n", pet_id));
	SPAM(("MOOD: %d\n", (int)current_pet_state.mood));
	SPAM(("HUNGER: %d\n", (int)current_pet_state.hunger));
	SPAM(("SAVE TIME: %u\n", save_time));
	SPAM(("ITEMS: [%d, %d, %d, %d]\n",	current_pet_state.items[0], current_pet_state.items[1], 
										current_pet_state.items[2], current_pet_state.items[3]));
	if (!current_pet_state.dead)
		apply_time_to_game(save_time);

	if (current_pet_state.dead) {
		current_pet_state.mood = current_pet_state.hunger = 0;
		current_pet_state.sprite = &current_pet_state.def->dead_sprite;
	}
}

static void 
feed_pet(void) {
	if (!current_pet_state.fed_item) return;
	current_pet_state.hunger += PET_ITEMS[current_pet_state.fed_item].nutrition;
	if (current_pet_state.hunger > PET_STAT_MAX)
		current_pet_state.hunger = PET_STAT_MAX;
	current_pet_state.fed_item = 0;
}

static void 
play_with_pet(void) {
	current_pet_state.mood += (rand() % (current_pet_state.mood_trait/10)+1) +1;
	if (current_pet_state.mood > PET_STAT_MAX) 
		current_pet_state.mood = PET_STAT_MAX;
}

static void
glitch_pet_sprite(VMUINT8* res_data) {
	glitch_pet_resource(res_data, current_pet_state.glitch);
}

//////////// TIMERS

/*
#define MAX_TIMERS 10
enum timer_type {FG_TIMER, BG_TIMER};

static struct timer {
	VM_TIMERPROC_T func;
	VMINT delay;
	VMINT tid;
};

static struct timers {
	struct timer foreground[MAX_TIMERS];
	struct timer background[MAX_TIMERS];
} game_timers;

static struct timer 
game_timer(VM_TIMERPROC_T func, VMINT delay) {
	struct timer timer; 
	timer.func = func;
	timer.delay = delay;
	timer.tid = -1;
	return timer;
}

static bool
create_timer(struct timer timer, enum timer_type type, VMINT slot) {
	struct timer* slots = type == BG_TIMER ? game_timers.background : game_timers.foreground;
	if (slots[slot].func != NULL) 
		return false;
	slots[slot] = timer;
	slots[slot].tid = type == FG_TIMER ? vm_create_timer(timer.delay, timer.func) : -1;
	if (slots[slot].tid < 0) {
		slots[slot].func = NULL;	
		return false;
	}
	return true;
}

static void
pause_timers(enum timer_type type) {
	int t;
	struct timer* slots = type == BG_TIMER ? game_timers.background : game_timers.foreground;
	for (t = 0; t < MAX_TIMERS; t++) {
		if (slots[t].func && slots[t].tid > 0) {
			vm_delete_timer(slots[t].tid);
			slots[t].tid = -1;
		}
	}
}

static void
resume_timers(enum timer_type type) {
	int t;
	struct timer* slots = type == BG_TIMER ? game_timers.background : game_timers.foreground;
	for (t = 0; t < MAX_TIMERS; t++) {
		if (slots[t].func && slots[t].tid == -1) {
			slots[t].tid = vm_create_timer(slots[t].delay, slots[t].func);
		}
	}
}
*/

//////////// NUKLEAR SETUP

static void 
game_sys_event_handler(VMINT message, VMINT param);

/* the (old) compiler seems much better at producing a small binary with static methods */
#define NK_PRIVATE 
#define NK_IMPLEMENTATION
#include "nuklear_mre.h"

static nk_size zero = 0;
static struct nk_user_font watch_font;
static struct nk_color bg_color			= {20, 20, 20, 255};
static struct nk_color btn_color_main	= {218, 59, 89, 255};

static void 
nk_button_transparent(struct nk_context* ctx) {
	ctx->style.button.normal = nk_style_item_color(nk_rgba(0,0,0,0));
	ctx->style.button.hover = nk_style_item_color(nk_rgba(0,0,0,0));
	ctx->style.button.active = nk_style_item_color(nk_rgba(0,0,0,0));
	ctx->style.button.border_color = nk_rgba(0,0,0,0);
}

static void 
nk_button_default(struct nk_context* ctx) {
	ctx->style.button.normal          = nk_style_item_color(nk_default_color_style[NK_COLOR_BUTTON]);
    ctx->style.button.hover           = nk_style_item_color(nk_default_color_style[NK_COLOR_BUTTON_HOVER]);
    ctx->style.button.active          = nk_style_item_color(nk_default_color_style[NK_COLOR_BUTTON_ACTIVE]);
    ctx->style.button.border_color    = nk_default_color_style[NK_COLOR_BORDER];
}

/*	everything is defined within headers since nk must be static 
	or the ancient arm compiler will keep a load of bloat */

#define VIEWS_SETUP
#include "views.h"

float calc_watch_font_width(nk_handle _, float h, const char* str, int len) {
	VMWCHAR wide_str[MRE_STR_SIZE_MAX];
	vm_gb2312_to_ucs2(wide_str, MRE_STR_SIZE_MAX, (VMSTR)str);
	wide_str[len] = '\0';
	return (float) vm_graphic_get_string_width(wide_str);
}

static void
game_sys_event_handler(VMINT message, VMINT param) {
	int t;
	if (message == VM_MSG_QUIT) {
		save_pet_data();
	}
}


void vm_main(void) {
	VMUINT time;
	int i;

	vm_gb2312_to_ucs2(wide_save_file_name, MRE_FILE_NAME_SIZE, (VMSTR)SAVE_FILE_NAME);

	if(!vm_get_curr_utc(&time)) {
		vm_log_fatal("Time is broken");
	}

	srand(time);

	init_views();
	vm_res_init();

	//memset(game_timers, 0, sizeof (struct timers));
	load_or_create_pet();

	//create_timer(game_timer(update_pet_frame, 200), FG_TIMER, 0);
	vm_create_timer(200, update_pet_frame);

	watch_font.height = (float) vm_graphic_get_character_height();
	watch_font.width = calc_watch_font_width;
	nk_mre_init(&watch_font, vm_graphic_get_screen_width(), vm_graphic_get_screen_height());
	
	// All the window padding does not make sense on a tiny watch
	mre.ctx.style.window.spacing				= nk_vec2(0,0);
	mre.ctx.style.window.scaler_size			= nk_vec2(0,0);
	mre.ctx.style.window.footer_padding			= nk_vec2(0,0);
	mre.ctx.style.window.padding				= nk_vec2(0,0);
	mre.ctx.style.window.header.label_padding	= nk_vec2(0,0);
	mre.ctx.style.window.header.padding			= nk_vec2(0,0);

	// The the background colours
	mre.ctx.style.window.fixed_background.data.color = bg_color;
	mre.ctx.style.progress.normal.data.color		 = bg_color;

	// Goto the main view of your pet
	nk_mre_set_view(&mre.ctx,&view_watch_due_main);
}

