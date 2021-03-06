#ifndef VIEW_PET_ITEM
#define VIEW_PET_ITEM

extern struct mre_view_p view_pet_items;

#ifdef VIEWS_SETUP

#include "view_watch_due_main.h"
#include "macro_utils.h"

struct mre_view_p view_pet_items;

static void 
view_func_pet_items(struct mre* mre) {
	struct nk_context* ctx = &mre->ctx;
	struct nk_panel layout;
	struct nk_image image;
	VMINT i;

	nk_button_default(ctx);
	if (nk_begin(ctx, &layout, "Items", nk_rect(0, 0, mre->width, mre->height), NK_WINDOW_NO_SCROLLBAR))
	{
		nk_layout_row_static(ctx, mre->height/4 + 30, mre->width/2, 2);
		nk_button_default(ctx);
		for (i = 0; i < PET_ITEMS_MAX; i++) {
			VMINT item_id;
			item_id = current_pet_state.items[i];

			image = nk_sprite(sprite_ptr(&SPRITE_FOOD_PACKED, SPRITE_PACKED), 8, item_id);
			image.xo = (mre->width/2      - ctx->style.button.padding.x*2 - image.w)/2;
			image.yo = (mre->height/4 +30 - ctx->style.button.padding.y*2 - image.h)/2;

			if (nk_button_image(ctx, image) == NK_TAPPED) {
				// Can only feed when hungry & not already being fed
				if (current_pet_state.hunger < PET_STAT_MAX 
						&& current_pet_state.fed_item == 0 && item_id != 0) {
					current_pet_state.fed_item = item_id;
					current_pet_state.items[i] = 0;
					reset_feeding_particles = true;
					nk_mre_set_view(ctx, &view_watch_due_main);
				}
			}
		}

		nk_layout_row_static(ctx, mre->height/2 - 60, mre->width, 1);
		ctx->style.button.normal.data.color = btn_color_main;
		if(nk_button_label(ctx, NK_TXT_PAD("BACK")) == NK_TAPPED) {
			nk_mre_set_view(ctx, &view_watch_due_main);
		}
		nk_end(ctx);
	}
}

#endif

#endif
