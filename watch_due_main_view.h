#ifndef WACH_DUE_MAIN_VIEW_H
#define WACH_DUE_MAIN_VIEW_H

#include "vmtimer.h"

static int mood_offset = 0;
static int mood_reset = 1;
static int pet_sprite_index = 0;
static int pet_frame_count = 6;

#define IMG_PET_GROUND_DARK "FloorLarge.gif"
#define IMG_PET_MOOD_HAPPY "MoodHappyLarge.gif"
#define IMG_BATTO "Flame.gif"

// MRE seems to support getting frames from gifs but the api is undocumented & all my attempts use it resulted in
// the -1 error code (which seems to translate to a shrug).
static char flame_sprites[][12] = {"flame_0.gif", "flame_1.gif","flame_2.gif","flame_3.gif","flame_4.gif","flame_5.gif"};

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
	pet_sprite_index = ++pet_sprite_index % pet_frame_count;
}


static void 
watch_due_main_view(struct mre* mre) {

	static struct nk_color bg_color = {20, 20, 20, 255};
	static struct nk_color mood_bar_color = {245, 223, 102, 255};
	static struct nk_color food_bar_color = {245, 102, 206, 255};

	struct nk_context* ctx = &mre->ctx;
	struct nk_panel layout;
	struct nk_image pet_img;
	nk_int state;

	nk_size cur = 70;
	static int padding = -1;

	if (padding == -1) {
		padding = (mre->width - 120)/2;
		vm_create_timer(100, watch_due_main_timer_hdl);
		vm_create_timer(200, update_pet_frame);
	}

	ctx->style.window.spacing = nk_vec2(0,0);
	ctx->style.window.scaler_size = nk_vec2(0,0);
	ctx->style.window.footer_padding = nk_vec2(0,0);
	ctx->style.window.padding = nk_vec2(0,0);
	ctx->style.window.header.label_padding = nk_vec2(0,0);
	ctx->style.window.header.padding = nk_vec2(0,0);
	ctx->style.window.fixed_background.data.color = bg_color;
	ctx->style.progress.normal.data.color = bg_color;
	ctx->style.progress.padding.x = 20;
	ctx->style.progress.padding.y = 15;
	if (nk_begin(ctx, &layout, "Main", nk_rect(0, 0, mre->width, mre->height), NK_WINDOW_NO_SCROLLBAR))
	{
		nk_layout_row_static(ctx, (mre->height - 160)/2 - 20, mre->width, 1);
		nk_progress(ctx, &zero, 100, 0);
		nk_layout_row_begin(ctx, NK_STATIC, 0, 2);
		pet_img = nk_image_path(IMG_PET_GROUND_DARK);
		pet_img.w = pet_img.h = 36;
		pet_img.xo = (mre->width-116)/2; pet_img.yo = ((mre->height - 160)/2-20) + 95;
		pet_img.scale = 200;
		nk_image(ctx, pet_img);
		nk_layout_row_begin(ctx, NK_STATIC, 0, 2);
		{
			nk_layout_row_push(ctx,(mre->width - 36)/2);
			nk_progress(ctx, &zero, 100, 0);
			nk_layout_row_push(ctx,36);
			pet_img = nk_image_path(IMG_PET_MOOD_HAPPY);
			pet_img.w = pet_img.h = 36;
			pet_img.xo = 1; pet_img.yo = -5-mood_offset;
			pet_img.scale = 200;
			nk_image(ctx, pet_img);
			nk_layout_row_push(ctx,(mre->width - 36)/2);
			nk_progress(ctx, &zero, 100, 0);
			nk_layout_row_end(ctx);
		}
		nk_layout_row_begin(ctx, NK_STATIC, 160, 2);
		{
			nk_layout_row_push(ctx, padding);
			ctx->style.progress.cursor_normal.data.color = mood_bar_color;
			nk_progress(ctx, &cur, 100, 0);
			nk_layout_row_push(ctx, 120);
			ctx->style.button.image_padding.x = 0;
			ctx->style.button.image_padding.y = 0;
			pet_img = nk_image_path(flame_sprites[pet_sprite_index]);
			pet_img.w = pet_img.h = 120;
			pet_img.scale = 150;
			pet_img.yo = 35;
			pet_img.xo = -8;
			nk_button_transparent(ctx);
			if (nk_button_image(ctx, pet_img) == 3) {
				nk_mre_set_view(ctx, pet_item_screen);
			}
			nk_layout_row_push(ctx, padding);
			ctx->style.progress.cursor_normal.data.color = food_bar_color;
			nk_progress(ctx, &cur, 100, 0);
			nk_layout_row_end(ctx);
		}
		nk_button_default(ctx);
		ctx->style.button.normal.data.color = btn_color_main;
		nk_layout_row_static(ctx, mre->height/2 - 60, mre->width, 1);
		if(nk_button_label(ctx, "ITEMS ") == 1) {
			//printf("%d btn\n", state);
			nk_mre_set_view(ctx, pet_item_screen);
		}
		nk_end(ctx);
	}
}

#endif