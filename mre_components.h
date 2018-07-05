#ifndef MRE_COMPONENTS_H
#define MRE_COMPONENTS_H

/*

	MRE Components : Header File
	The MRE Components file declares the component object that makes the view to draw using nuklear. 

	Author: Varuna Singh, UstadMobile
*/ 

/*
	MRE-Nuklear Components
	Components used to draw
*/
struct mre_nk_c {
	int id;
	char *type;	//type of component
	char *title;
	int len;
	//Placeholder. Not using style at the moment.
	//struct nk_style style; 
	int hovering;
	int is_hoverable;
	/* For Media assets */ 
	char *url;
	char *thumbnail;
};

/* Create the mre display object */
struct mre {
	//some sort of window object - most similar in mre is layers?
	int window;
	//int memory_mre;
	unsigned int width;
	unsigned int height;
	struct nk_context ctx;
	
	//Focusable Object Areas:
	struct nk_rect fObjs[20]; 
};

/* 
	MRE View Properties. The list of components and hoverable counter. 
	The view controller will process through this object.
*/
typedef void (*nk_mre_view_setup)(struct mre*);
typedef void (*nk_mre_view_func)(struct mre*);
typedef void (*nk_mre_view_cleanup)(struct mre*);
struct mre_view_p{
	nk_mre_view_setup setup;
	nk_mre_view_func view;
	nk_mre_view_cleanup cleanup;
};

#endif