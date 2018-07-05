#ifndef VIEWS_H
#define VIEWS_H

#ifdef VIEWS_SETUP 

#include "pet_definitions.h"
#include "view_pet_items.h"
#include "view_watch_due_main.h"
#include "view_pet_info.h"

#define INIT_VIEW(c_view,v_setup,v_func,v_cleanup)	\
	(c_view).setup = (v_setup);						\
	(c_view).view = (v_func);						\
	(c_view).cleanup = (v_cleanup);

static void 
init_views (void) {
	// This must be done at runtime for postion independant code
	INIT_VIEW(view_watch_due_main,	setup_watch_due_main,	view_func_watch_due_main,		NULL);
	INIT_VIEW(view_pet_info,		NULL,					view_func_pet_info,				NULL);
	INIT_VIEW(view_pet_items,		NULL,					view_func_pet_items,			NULL);
}

#endif

#undef VIEWS_SETUP

#endif