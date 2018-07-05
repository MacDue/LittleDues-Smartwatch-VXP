#ifndef VIEW_PET_INFO_H
#define VIEW_PET_INFO_H

extern struct mre_view_p view_pet_info;

#ifdef VIEWS_SETUP

struct mre_view_p view_pet_info;

static void view_func_pet_info(struct mre* mre) {
	struct nk_context* ctx = &mre->ctx;
	struct nk_panel layout;

	nk_button_default(ctx);
	if (nk_begin(ctx, &layout, "Info", nk_rect(0, 0, mre->width, mre->height), NK_WINDOW_NO_SCROLLBAR))
	{
		
		nk_end(ctx);
	}

}

#endif

#endif