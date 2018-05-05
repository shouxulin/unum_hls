//
// Created by kiraMac on 9/4/18.
//

#ifndef UNUM_HLS_GLAYER_H
#define UNUM_HLS_GLAYER_H

#include "ap_fixed.h"
#include "uenv.h"

typedef ap_fixed<integersize+fractionsize,integersize, AP_RND_INF, AP_SAT> gnum_f_s;
typedef ap_int<esizemax+1+1> gnum_e_s;

typedef struct {
    gnum_e_s e = 0; //extra 1 for potential overflow and 1 for sign
    gnum_f_s f = 0;
    ap_uint<1> open = 1;
    ap_uint<1> inf = 1;
} gnum_s;

typedef struct {
    gnum_s l;
    gnum_s r;
    ap_uint<1> nan = 1;
} gbnd_s;

void print_gbnd(gbnd_s *gbnd);

void print_gnum(gnum_s *gnum);

void gnum_e_shift(gnum_e_s* e, int bits);

void gnum_f_shift(gnum_f_s *f, int bit);

gnum_e_s get_max_gnum_e();

const gnum_e_s MAX_GNUM_E = get_max_gnum_e();

gnum_e_s get_min_gnum_e();

const gnum_e_s MIN_GNUM_E = get_min_gnum_e();

gnum_f_s get_min_gnum_f();

const gnum_f_s MIN_GNUM_F = get_min_gnum_f();

void init_gnum(gnum_s *gnum);

void init_gbnd(gbnd_s *gbnd);

#endif //UNUM_HLS_GLAYER_H

