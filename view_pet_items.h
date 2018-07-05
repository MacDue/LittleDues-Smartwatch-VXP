#ifndef VIEW_PET_ITEM
#define VIEW_PET_ITEM

extern struct mre_view_p view_pet_items;

#ifdef VIEWS_SETUP

#include "view_watch_due_main.h"

struct mre_view_p view_pet_items;

static void 
view_func_pet_items(struct mre* mre) {
	struct nk_context* ctx = &mre->ctx;
	struct nk_panel layout;

	nk_button_default(ctx);
	if (nk_begin(ctx, &layout, "Items", nk_rect(0, 0, mre->width, mre->height), NK_WINDOW_NO_SCROLLBAR))
	{
		nk_layout_row_static(ctx, mre->height/4 + 30, mre->width/2, 2);
		nk_button_label(ctx,"A"); nk_button_label(ctx,"B");
		nk_button_label(ctx,"C"); nk_button_label(ctx,"D");

		nk_layout_row_static(ctx, mre->height/2 - 60, mre->width, 1);
		ctx->style.button.normal.data.color = btn_color_main;
		if(nk_button_label(ctx, "BACK ") == 1) {
			nk_mre_set_view(ctx, &view_watch_due_main);
		}
		nk_end(ctx);
	}
}

#endif

#endif
