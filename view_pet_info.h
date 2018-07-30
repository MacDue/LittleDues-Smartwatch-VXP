#ifndef VIEW_PET_INFO_H
#define VIEW_PET_INFO_H

extern struct mre_view_p view_pet_info;

#ifdef VIEWS_SETUP

#include "macro_utils.h"

struct mre_view_p view_pet_info;

static struct 
view_pet_info_adjustments_calculated {

	VMUINT8 pet_sprite_draw_w;

} view_pet_info_magic_numbers;

#define MAGIC_NUMBERS view_pet_info_magic_numbers

static void
setup_pet_info(struct mre* mre) {
	const struct watch_due_pet* pet_def = current_pet_state.def;
	
	MAGIC_NUMBERS.pet_sprite_draw_w 
		= (VMUINT8)(pet_def->sprite.w 
			* PERCENT_TO_MULT_SCALE(pet_def->view_info_adjustments.pet_sprite.scale));

}


static void 
view_func_pet_info(struct mre* mre) {
	const struct watch_due_pet* pet_def = current_pet_state.def;
	struct nk_context* ctx = &mre->ctx;
	struct nk_panel layout;
	struct nk_image image;

	nk_button_default(ctx);
	if (nk_begin(ctx, &layout, "Info", nk_rect(0, 0, mre->width, mre->height), NK_WINDOW_NO_SCROLLBAR))
	{
		nk_layout_row_begin(ctx, NK_STATIC, mre->height/2 + 60 - vm_graphic_get_character_height()*5, 3);
		{
			// Some left padding
			nk_layout_row_push(ctx, (mre->width-MAGIC_NUMBERS.pet_sprite_draw_w)/2);
			nk_progress(ctx, &zero, 100, 0);

			// Little pet sprite
			nk_layout_row_push(ctx, MAGIC_NUMBERS.pet_sprite_draw_w);
			//image = nk_image_path(current_pet_state.sprite->frames[pet_sprite_index]);
			//image.scale = pet_def->view_info_adjustments.pet_sprite.scale;
			//image.xo = pet_def->view_info_adjustments.pet_sprite.x_offset;
			//image.yo = pet_def->view_info_adjustments.pet_sprite.y_offset;
			//if (current_pet_state.glitch) {
			//	image.pre_draw = glitch_pet_sprite;
		//	}
			//nk_image(ctx,image);

			nk_layout_row_end(ctx);
		}

		nk_layout_row_static(ctx, vm_graphic_get_character_height(), mre->width/3, 3);
		{
			char age_str[SMALL_STR_SIZE];
			const char* p_type 
				= P_TYPE_STRS[
					get_personality_type(current_pet_state.mood_trait, 
									     current_pet_state.hunger_trait)
				  ];
			const char* mood
				= MOOD_STRS[
					get_current_mood(current_pet_state.mood, current_pet_state.hunger)
				  ];

			sprintf(age_str, "%.2f PY", current_pet_state.age);

			// General pet stats
			nk_text(ctx, NK_TXT("SPECIES"),			NK_TEXT_ALIGN_RIGHT);
			nk_text(ctx, NK_TXT("~"),				NK_TEXT_ALIGN_CENTERED);
			nk_text(ctx, NK_TXT(pet_def->species),	NK_TEXT_ALIGN_RIGHT);

			nk_text(ctx, NK_TXT("P-TYPE"),			NK_TEXT_ALIGN_RIGHT);
			nk_text(ctx, NK_TXT("~"),				NK_TEXT_ALIGN_CENTERED);
			nk_text(ctx, NK_TXT(p_type),			NK_TEXT_ALIGN_RIGHT);

			nk_text(ctx, NK_TXT("AGE"),				NK_TEXT_ALIGN_RIGHT);
			nk_text(ctx, NK_TXT("~"),				NK_TEXT_ALIGN_CENTERED);
			nk_text(ctx, NK_TXT(age_str),			NK_TEXT_ALIGN_RIGHT);

			nk_text(ctx, NK_TXT("MOOD"),			NK_TEXT_ALIGN_RIGHT);
			nk_text(ctx, NK_TXT("~"),				NK_TEXT_ALIGN_CENTERED);
			nk_text(ctx, NK_TXT(mood),				NK_TEXT_ALIGN_RIGHT);

			// a little padding
			nk_text(ctx, "", 0, NK_TEXT_ALIGN_CENTERED);
			nk_layout_row_end(ctx);
		}

		// Button back to main view
		nk_layout_row_static(ctx, mre->height/2 - 60, mre->width, 1);
		ctx->style.button.normal.data.color = btn_color_main;
		if(nk_button_label(ctx, NK_TXT_PAD("BACK")) == NK_TAPPED) {
			nk_mre_set_view(ctx, &view_watch_due_main);
		}
	
		nk_end(ctx);
	}
}

#undef MAGIC_NUMBERS

#endif

#endif