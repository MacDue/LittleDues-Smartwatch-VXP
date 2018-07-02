

static void 
pet_item_screen(struct mre* mre) {
	struct nk_context* ctx = &mre->ctx;
	struct nk_panel layout;

	if (nk_begin(ctx, &layout, "Items", nk_rect(0, 0, mre->width, mre->height), NK_WINDOW_NO_SCROLLBAR))
	{
		nk_layout_row_static(ctx, mre->height/2 + 60, mre->width, 1);
		nk_progress(ctx, &zero, 100, 0);
		nk_layout_row_static(ctx, mre->height/2 - 60, mre->width, 1);
		if(nk_button_label(ctx, "BACK ")) {
		//	nk_mre_set_view(pet_item_screen);
			vm_exit_app();
		}
		nk_end(ctx);
	}

}