#ifndef NK_PARTICLES_H
#define NK_PARTICLES_H

#include "macro_utils.h"
#define PARTICLE_COUNT 10


static nk_int 
nk_particles(struct nk_context* ctx, nk_int x, nk_int y, struct SpritePtr sprite, nk_int frame, nk_int particle_scale, bool* reset) {
	nk_int p;
	double angle, angle_delta;
	static double r;
	static bool done;

	if (*reset) {
		r = 0;
		done = false;
		*reset = false;
	}

	if (r > vm_graphic_get_screen_width()/2) {
		if (!done) {
			done = true;
			return 1;
		}
		return 0;
	}

	angle = 0;
	angle_delta = 360.0f/PARTICLE_COUNT;
	
	nk_layout_row_static(ctx, 0, 0, PARTICLE_COUNT);
	for (p = 0; p < PARTICLE_COUNT; p++) {
		struct nk_image particle;
		particle = nk_sprite(sprite, particle_scale, frame);
		particle.xo = (short)(x - particle.w/2 + r * cos(angle * DEG_TO_RAD));
		particle.yo = (short)(y - particle.h/2 + r * sin(angle * DEG_TO_RAD));
		nk_image(ctx, particle);
		angle += angle_delta;
	}
	r += 7;
	return 0;
}


#endif
