#ifndef VIEW_WACH_DUE_MAIN_H
#define VIEW_WACH_DUE_MAIN_H

extern struct mre_view_p view_watch_due_main;

#ifdef VIEWS_SETUP

#include "vmtimer.h"
#include "view_pet_info.h"
#include "view_pet_items.h"
#include "pet_definitions.h"
#include "macro_utils.h"
#include "nk_particles.h"
#include "sprites.h"


const struct sprite SPRITE_PARTICLE_HAPPY	= {18, 0, 9,  9,  1};
const struct sprite SPRITE_FLOOR_DISC		= {0, 9,  29, 9,  1};
const struct sprite SPRITE_MOOD_HAPPY		= {0, 0,  9,  9,  1};
const struct sprite SPRITE_MOOD_SAD			= {9, 0,  9,  9,  1};

#define PARTICLE_HAPPY_SCALE	2
#define PARTICLE_FOOD_SCALE		4
#define PET_AREA_WH		160

static int mood_offset			= 0;
static int mood_reset			= 1;

static bool main_view_one_time_setup_done = false;

struct mre_view_p view_watch_due_main;
static void load_or_create_pet(void);

static struct 
view_main_adjustments_calculated {
	VMUINT8 view_height_padding_top;	

	VMUINT8 pet_sprite_draw_w;
	VMUINT8 pet_sprite_draw_h;

	VMUINT8 floating_status_draw_wh; 

	VMUINT8 floor_disc_w;
	VMUINT8 floor_disc_h;
	
} view_main_magic_numbers;

#define MAGIC_NUMBERS view_main_magic_numbers

static void 
watch_due_main_timer_hdl(VMINT tid) {
	if (mood_offset < 10 && mood_reset) {
		if (++mood_offset == 10)
			mood_reset = 0;
	} else {
		if (--mood_offset == 0)
			mood_reset = 1;
	}
	update_gui();
}

static void
setup_watch_due_main(struct mre* mre) {
	struct nk_context* ctx = &mre->ctx;
	const struct watch_due_pet* pet_def = current_pet_state.def;

	if (!main_view_one_time_setup_done) {
		vm_create_timer(100, watch_due_main_timer_hdl);
		main_view_one_time_setup_done = true;
	}

	ctx->style.progress.padding.x = 20;
	ctx->style.progress.padding.y = 15;
	ctx->style.button.image_padding.x = 0;
	ctx->style.button.image_padding.y = 0;

	MAGIC_NUMBERS.pet_sprite_draw_w 
		= current_pet_state.sprite->w * pet_def->view_main_adjustments.pet_sprite.scale;

	MAGIC_NUMBERS.pet_sprite_draw_h
		= current_pet_state.sprite->h * pet_def->view_main_adjustments.pet_sprite.scale;

	MAGIC_NUMBERS.floating_status_draw_wh
		= SPRITE_MOOD_HAPPY.w * pet_def->view_main_adjustments.floating_status.scale;

	MAGIC_NUMBERS.view_height_padding_top 
		= (mre->height - PET_AREA_WH)/2 - 20;

	MAGIC_NUMBERS.floor_disc_w 
		= SPRITE_FLOOR_DISC.w * pet_def->view_main_adjustments.floor_disc.scale; 

	MAGIC_NUMBERS.floor_disc_h 
		= SPRITE_FLOOR_DISC.h * pet_def->view_main_adjustments.floor_disc.scale;

}


static bool reset_feeding_particles = false;

static void 
view_func_watch_due_main(struct mre* mre) {
	const struct watch_due_pet* pet_def = current_pet_state.def;

	static struct nk_color mood_bar_color = {245, 223, 102, 255};
	static struct nk_color food_bar_color = {245, 102, 206, 255};
	static bool particles_on = false;
	static bool reset_particles = true;

	struct nk_context* ctx = &mre->ctx;
	struct nk_panel layout;
	struct nk_image image;
	nk_size bar_cur;

	if (nk_begin(ctx, &layout, "Main", nk_rect(0, 0, mre->width, mre->height), NK_WINDOW_NO_SCROLLBAR))
	{
		// Top padding
		nk_layout_row_static(ctx, MAGIC_NUMBERS.view_height_padding_top, mre->width, 1);
		nk_progress(ctx, &zero, 100, 0); /* used for spacing size nk_spacing() seems to glitch */

		nk_layout_row_static(ctx, 0, 0, 2); /* invisible row */

		// Ground disc (below pet)
		image = nk_sprite(&SPRITE_FLOOR_DISC, pet_def->view_main_adjustments.floor_disc.scale, 0);
		image.xo = (mre->width-image.w)/2 
					+ pet_def->view_main_adjustments.floor_disc.x_offset;
		image.yo = MAGIC_NUMBERS.view_height_padding_top 
					+ pet_def->view_main_adjustments.floor_disc.y_offset;
		nk_image(ctx, image);

		// Pet floating status
		if (!current_pet_state.dead) {
			nk_layout_row_begin(ctx, NK_STATIC, 0, 2);
			{
				VMINT mood = (VMINT) get_current_mood(current_pet_state.mood, current_pet_state.hunger);
				image = nk_sprite(mood <= GOOD_BAD_MOOD_TIPPING_POINT 
					? &SPRITE_MOOD_SAD : &SPRITE_MOOD_HAPPY, pet_def->view_main_adjustments.floating_status.scale, 0);
				image.w = image.h = MAGIC_NUMBERS.floating_status_draw_wh;
				nk_layout_row_push(ctx,(mre->width - image.w)/2);
				nk_progress(ctx, &zero, 100, 0);
				nk_layout_row_push(ctx,image.w);
				image.xo = pet_def->view_main_adjustments.floating_status.x_offset; 
				image.yo = pet_def->view_main_adjustments.floating_status.y_offset-mood_offset;
				nk_image(ctx, image);
				nk_layout_row_push(ctx,(mre->width - image.w)/2);
				nk_progress(ctx, &zero, 100, 0);
				nk_layout_row_end(ctx);
			}
		}
		// Pet sprite and happy/hunger bars
		nk_layout_row_begin(ctx, NK_STATIC, PET_AREA_WH, 4);
		{
			nk_int pet_tap;
			nk_int pet_padding = pet_def->view_main_adjustments.pet_sprite.padding;
			// Pet mood/happiness bar
			nk_layout_row_push(ctx, (mre->width - MAGIC_NUMBERS.pet_sprite_draw_w)/2 -pet_padding);
			ctx->style.progress.cursor_normal.data.color = mood_bar_color;
			bar_cur = (nk_size)current_pet_state.mood;
			nk_progress(ctx, &bar_cur, 100, 0);

			// Using an empty progress bar as padding again
			nk_layout_row_push(ctx, pet_padding);
			nk_progress(ctx, &zero, 100, 0);

			nk_layout_row_push(ctx, MAGIC_NUMBERS.pet_sprite_draw_w);
			image = nk_sprite(current_pet_state.sprite, pet_def->view_main_adjustments.pet_sprite.scale, pet_sprite_index);
			image.h = PET_AREA_WH; 
			image.xo = pet_def->view_main_adjustments.pet_sprite.x_offset;
			image.yo = pet_def->view_main_adjustments.pet_sprite.y_offset;
			
			if (current_pet_state.glitch) {
				image.pre_draw = glitch_pet_sprite;
			}

			// Double tap on pet to open pet info
			nk_button_transparent(ctx);
			pet_tap = nk_button_image(ctx, image);
			if (!current_pet_state.dead) {
				if (pet_tap == NK_DOUBLE_TAPPED) {
					nk_mre_set_view(ctx, &view_pet_info);
				} else if (pet_tap == NK_TAPPED) {
					reset_particles = true;
					particles_on = true;
				}
			}
	
			// More progress bar padding
			nk_layout_row_push(ctx, pet_padding);
			nk_progress(ctx, &zero, 100, 0);

			// Pet hunger bar
			nk_layout_row_push(ctx, (mre->width - MAGIC_NUMBERS.pet_sprite_draw_w)/2 -pet_padding);
			ctx->style.progress.cursor_normal.data.color = food_bar_color;
			bar_cur = (nk_size)current_pet_state.hunger;
			nk_progress(ctx, &bar_cur, 100, 0);

			nk_layout_row_end(ctx);
		}

		// Open pet items screen button
		nk_button_default(ctx);
		ctx->style.button.normal.data.color = btn_color_main;
		nk_layout_row_static(ctx, (mre->height - PET_AREA_WH)/2 + 20, mre->width, 1);
		if (!current_pet_state.dead) {
			if(nk_button_label(ctx, NK_TXT_PAD("ITEMS")) == NK_TAPPED) {
				nk_mre_set_view(ctx, &view_pet_items);
			}
		} else {
			// Dead pet. Cry to respawn.
			static VMINT crying = 0;
			if(nk_button_label(ctx, NK_TXT_PAD("CRY"))) {
				if (crying++ == 10) {
					vm_file_delete(wide_save_file_name);
					load_or_create_pet();
					vm_exit_app();
				}
			}
		}

		// Feeding + playing particles
		if (current_pet_state.fed_item) {
			if (nk_particles
				(
					ctx, mre->width/2, -mre->height/2, 
					&PET_ITEMS[current_pet_state.fed_item].sprite, 
					PARTICLE_FOOD_SCALE, &reset_feeding_particles)
				) { // on particle end
					feed_pet();
				}	
		} else if (particles_on && !current_pet_state.dead) {
			if (nk_particles
				(
					ctx, mre->width/2, -mre->height/2, 
					&SPRITE_PARTICLE_HAPPY, PARTICLE_HAPPY_SCALE, &reset_particles)
				) { // on particle end
					play_with_pet();
				}
		}
		nk_end(ctx);
	}
}

#undef MAGIC_NUMBERS

#endif

#endif