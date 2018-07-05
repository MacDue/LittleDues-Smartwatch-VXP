#include "vmsys.h"
#include "vmio.h"
#include "vmgraph.h"
#include "vmchset.h"
#include "vmstdlib.h"
#include "ResID.h"
#include "vm4res.h"

#define PETS_SETUP
#include "pet_definitions.h"

//#define		DEBUG
#define		SUPPORT_BG		

VMINT		layer_hdl[1];

typedef enum {true = 1, false = 0} bool;

struct current_pet_state {

	VMINT age;
	struct watch_due_pet* def;

} current_pet_state;


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
	return (float) vm_graphic_get_string_width(wide_str);
}

void vm_main(void) {
	init_views();
	vm_res_init();

	current_pet_state.def = &pet_flame_def;

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

