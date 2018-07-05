#ifndef VIEW_WACH_DUE_MAIN_H
#define VIEW_WACH_DUE_MAIN_H

extern struct mre_view_p view_watch_due_main;

#ifdef VIEWS_SETUP

#include "vmtimer.h"
#include "view_pet_info.h"
#include "view_pet_items.h"
#include "pet_definitions.h"

#define IMG_PET_GROUND_DARK		"FloorLarge.gif"
#define IMG_PET_MOOD_HAPPY		"MoodHappyLarge.gif"

#define PET_AREA_WH						160
#define SPRITE_FLOATING_STATUS_WH		18
#define SPRITE_GROUND_DISC_WIDTH		58
#define SPRITE_GROUND_DISC_HEIGHT		18

#define GROUND_DISC_SCAlE				2
#define MULT_SCALE_TO_PERCENT(mult)		(mult)*100 

#define PROGRESS_BAR_MAX 70


static int mood_offset			= 0;
static int mood_reset			= 1;
static int pet_sprite_index		= 0;


static bool main_view_one_time_setup_done = false;

struct mre_view_p view_watch_due_main;

static struct view_adjustments_calculated {
	VMINT8 view_height_padding_top;	

	VMINT8 pet_sprite_draw_w;
	VMINT8 pet_sprite_draw_h;

	VMINT8 floating_status_draw_wh; 
} magic_numbers;

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
update_pet_frame(VMINT tid) {
	pet_sprite_index = ++pet_sprite_index 
							% current_pet_state.def->sprite.frame_count;
}

static void
setup_watch_due_main(struct mre* mre) {
	struct nk_context* ctx = &mre->ctx;
	struct watch_due_pet* pet_def = current_pet_state.def;

	if (!main_view_one_time_setup_done) {
		vm_create_timer(100, watch_due_main_timer_hdl);
		vm_create_timer(200, update_pet_frame);
		main_view_one_time_setup_done = true;
	}

	ctx->style.progress.padding.x = 20;
	ctx->style.progress.padding.y = 15;
	ctx->style.button.image_padding.x = 0;
	ctx->style.button.image_padding.y = 0;

	magic_numbers.pet_sprite_draw_w 
		= pet_def->sprite.width 
			* ((float)pet_def->view_main_adjustments.pet_sprite.scale/100);

	magic_numbers.pet_sprite_draw_h
		= pet_def->sprite.width 
			* ((float)pet_def->view_main_adjustments.pet_sprite.scale/100);

	magic_numbers.floating_status_draw_wh
		= SPRITE_FLOATING_STATUS_WH
			* ((float)pet_def->view_main_adjustments.floating_status.scale/100);

	magic_numbers.view_height_padding_top 
		= (mre->height - PET_AREA_WH)/2 - 20;

}

static void 
view_func_watch_due_main(struct mre* mre) {

	struct watch_due_pet* pet_def = current_pet_state.def;

	static struct nk_color mood_bar_color = {245, 223, 102, 255};
	static struct nk_color food_bar_color = {245, 102, 206, 255};

	struct nk_context* ctx = &mre->ctx;
	struct nk_panel layout;
	struct nk_image image;
	nk_int state;

	nk_size cur = PROGRESS_BAR_MAX;

	if (nk_begin(ctx, &layout, "Main", nk_rect(0, 0, mre->width, mre->height), NK_WINDOW_NO_SCROLLBAR))
	{
		// Top padding
		nk_layout_row_static(ctx, magic_numbers.view_height_padding_top, mre->width, 1);
		nk_progress(ctx, &zero, 100, 0); /* used for spacing size nk_spacing() seems to glitch */

		nk_layout_row_begin(ctx, NK_STATIC, 0, 2); /* invisible row */

		// Ground disc (below pet)
		image = nk_image_path(IMG_PET_GROUND_DARK);
		image.w = SPRITE_GROUND_DISC_WIDTH * GROUND_DISC_SCAlE; 
		image.h = SPRITE_GROUND_DISC_HEIGHT * GROUND_DISC_SCAlE;
		image.xo = (mre->width-image.w)/2;
		image.yo = ((mre->height - 160)/2-20) + 95; // ???
		image.scale = MULT_SCALE_TO_PERCENT(2);
		nk_image(ctx, image);

		// Pet floating status
		nk_layout_row_begin(ctx, NK_STATIC, 0, 2);
		{
			image = nk_image_path(IMG_PET_MOOD_HAPPY);
			image.w = image.h = SPRITE_FLOATING_STATUS_WH 
									* pet_def->view_main_adjustments.floating_status.scale/100;
			nk_layout_row_push(ctx,(mre->width - image.w)/2);
			nk_progress(ctx, &zero, 100, 0);
			nk_layout_row_push(ctx,image.w);
			image.xo = pet_def->view_main_adjustments.floating_status.x_offset; 
			image.yo = pet_def->view_main_adjustments.floating_status.y_offset-mood_offset;
			image.scale = pet_def->view_main_adjustments.floating_status.scale;
			nk_image(ctx, image);
			nk_layout_row_push(ctx,(mre->width - image.w)/2);
			nk_progress(ctx, &zero, 100, 0);
			nk_layout_row_end(ctx);
		}

		// Pet sprite and happy/hunger bars
		nk_layout_row_begin(ctx, NK_STATIC, PET_AREA_WH, 2);
		{
			// Pet mood/happiness bar
			nk_layout_row_push(ctx, (mre->width - magic_numbers.pet_sprite_draw_w)/2);
			ctx->style.progress.cursor_normal.data.color = mood_bar_color;
			nk_progress(ctx, &cur, 100, 0);

			nk_layout_row_push(ctx, magic_numbers.pet_sprite_draw_w);
			image = nk_image_path(pet_def->sprite.frames[pet_sprite_index]);
			image.w = magic_numbers.pet_sprite_draw_w;
			image.h = PET_AREA_WH; 
			image.scale = pet_def->view_main_adjustments.pet_sprite.scale;
			image.xo = pet_def->view_main_adjustments.pet_sprite.x_offset;
			image.yo = pet_def->view_main_adjustments.pet_sprite.y_offset;
			// Double tap on pet to open pet info
			nk_button_transparent(ctx);
			if (nk_button_image(ctx, image) == NK_DOUBLE_TAPPED) {
				nk_mre_set_view(ctx, &view_pet_info);
			}
			
			// Pet hunger bar
			nk_layout_row_push(ctx, (mre->width - magic_numbers.pet_sprite_draw_w)/2);
			ctx->style.progress.cursor_normal.data.color = food_bar_color;
			nk_progress(ctx, &cur, 100, 0);

			nk_layout_row_end(ctx);
		}

		// Open pet items screen button
		nk_button_default(ctx);
		ctx->style.button.normal.data.color = btn_color_main;
		nk_layout_row_static(ctx, (mre->height - PET_AREA_WH)/2 + 20, mre->width, 1);
		if(nk_button_label(ctx, "ITEMS ") == NK_TAPPED) {
			nk_mre_set_view(ctx, &view_pet_items);
		}

		nk_end(ctx);
	}
}

#endif

#endif