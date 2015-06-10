#include <stdlib.h>
#include "soundpipe.h"

int sp_dust_create(sp_dust **p) {
    *p = malloc(sizeof(sp_dust));
    return SP_OK;
}

int sp_dust_destroy(sp_dust **p) {
    free(*p);
    return SP_OK;
}

int sp_dust_init(sp_data *sp, sp_dust *p, SPFLOAT amp, SPFLOAT density) {
    p->density = density;
    p->amp = amp;
    p->density0 = 0.0;
    p->thresh = 0.0;
    p->scale = 0.0;
    p->rand = rand();
    p->onedsr = 1.0 / sp->sr;
    p->bipolar = 0;
    return SP_OK;
}

int sp_dust_compute(sp_data *sp, sp_dust *p, SPFLOAT *in, SPFLOAT *out) {
    SPFLOAT density, thresh, scale;
    const SPFLOAT dv2_31 = 4.656612873077392578125e-10;
    density = p->density;
    if (density != p->density0) {
        thresh = p->thresh = density * p->onedsr;
        if(p->bipolar) {
            scale  = p->scale  = (thresh > 0.0 ? 2.0 / thresh : 0.0);
        } else {
            scale  = p->scale  = (thresh > 0.0 ? 1.0 / thresh : 0.0);
        }
        p->density0 = density;
    } else {
        thresh = p->thresh;
        scale  = p->scale;
    }
    *out = 0;
    SPFLOAT r;
    p->rand = rand();
    r = (SPFLOAT)p->rand * dv2_31;

    if(p->bipolar) {
        *out = p->amp * (r < thresh ? r*scale - 1.0 : 0.0);
    } else {
        *out = p->amp * (r < thresh ? r*scale : 0.0);
    }

    return SP_OK;
}
