#include "vmsys.h"
#include "vmio.h"
#include "vmgraph.h"
#include "vmchset.h"
#include "vmstdlib.h"
#include "ResID.h"
#include "vm4res.h"


#define		SUPPORT_BG		

VMINT		layer_hdl[1];

typedef enum {true, false} bool;

#define NK_PRIVATE
#define NK_IMPLEMENTATION
#include "nuklear_mre.h"

static nk_size zero = 0;

#include "pet_item_screen.h"
#include "watch_due_main_view.h"

static struct nk_user_font watch_font;

static bool view_setup = false;

float calc_watch_font_width(nk_handle _, float h, const char* str, int len) {
	VMWCHAR wide_str[MRE_STR_SIZE_MAX];
	vm_gb2312_to_ucs2(wide_str, MRE_STR_SIZE_MAX, (VMSTR)str);
	return (float) vm_graphic_get_string_width(wide_str);
}

void vm_main(void) {
	vm_res_init();
	watch_font.height = (float) vm_graphic_get_character_height();
	watch_font.width = calc_watch_font_width;
	nk_mre_init(&watch_font, vm_graphic_get_screen_width(), vm_graphic_get_screen_height());
	nk_mre_set_view(&mre.ctx,watch_due_main_view);
}

