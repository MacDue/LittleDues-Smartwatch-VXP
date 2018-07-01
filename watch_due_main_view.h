#include "vmtimer.h"


static int mood_offset = 0;
static int mood_reset = 1;

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
watch_due_main_view(struct mre* mre) {
	struct nk_context* ctx = &mre->ctx;
	struct nk_panel layout;
	struct nk_image pet_img;
	static struct nk_color item_btn_color = {218, 59, 89, 255};
	static struct nk_color bg_color = {20, 20, 20, 255};
	nk_size cur = 50;
	static nk_size zero = 0;
	static int padding = -1;
	//mre->height = 250;
	//mre->width = 250;
	if (padding == -1) {
		padding = (mre->width - 144)/2;
		vm_create_timer(100, watch_due_main_timer_hdl);
	}
	//SPAM(("%f %f %f - %d %d\n", ratio [0], ratio [1], ratio [2], mre->width, mre->height));
	SPAM(("... %f\n", ctx->style.font->height)); 
	ctx->style.window.spacing = nk_vec2(0,0);
	ctx->style.window.scaler_size = nk_vec2(0,0);
	ctx->style.window.footer_padding = nk_vec2(0,0);
	ctx->style.window.padding = nk_vec2(0,0);
	ctx->style.window.header.label_padding = nk_vec2(0,0);
	ctx->style.window.header.padding = nk_vec2(0,0);
	ctx->style.window.fixed_background.data.color = bg_color;
	ctx->style.progress.normal.data.color = bg_color;
	if (nk_begin(ctx, &layout, "Main", nk_rect(0, 0, mre->width, mre->height), NK_WINDOW_NO_SCROLLBAR))
	{
		nk_layout_row_static(ctx, (mre->height - 160)/2 - 20, mre->width, 1);
		nk_progress(ctx, &zero, 100, 0);
		nk_layout_row_begin(ctx, NK_STATIC, 0, 2);
		pet_img = nk_image_path("FloorLarge.gif");
		pet_img.w = pet_img.h = 36;
		pet_img.xo = (mre->width-116)/2; pet_img.yo = ((mre->height - 160)/2-20) + 90;
		pet_img.scale = 200;
		nk_image(ctx, pet_img);
		nk_layout_row_begin(ctx, NK_STATIC, 0, 2);
		{
			nk_layout_row_push(ctx,(mre->width - 36)/2);
			nk_progress(ctx, &zero, 100, 0);
			nk_layout_row_push(ctx,36);
			pet_img = nk_image_path("MoodHappyLarge.gif");
			pet_img.w = pet_img.h = 36;
			pet_img.xo = 1; pet_img.yo = 10-mood_offset;
			pet_img.scale = 200;
			nk_image(ctx, pet_img);
			nk_layout_row_push(ctx,(mre->width - 36)/2);
			nk_progress(ctx, &zero, 100, 0);
			nk_layout_row_end(ctx);
		}
		nk_layout_row_begin(ctx, NK_STATIC, 160, 2);
		{
			nk_layout_row_push(ctx, padding);
			nk_progress(ctx, &cur, 100, 0);
			nk_layout_row_push(ctx, 144);
			pet_img = nk_image_path("BattoLarge.gif");
			pet_img.w = pet_img.h = 144;
			pet_img.scale = 180;
			nk_image(ctx, pet_img);
			nk_layout_row_push(ctx, padding);
			nk_progress(ctx, &cur, 100, 0);
			nk_layout_row_end(ctx);
		}
		ctx->style.button.normal.data.color = item_btn_color;
		nk_layout_row_static(ctx, (mre->height - 160)/2+20, mre->width, 1);
		nk_button_label(ctx, "ITEMS ");
		//nk_layout_row_static(ctx, 20, mre->width, 1);
		//nk_progress(ctx, &zero, 100, 0);
		nk_end(ctx);
	}

}