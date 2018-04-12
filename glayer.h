//
// Created by kiraMac on 9/4/18.
//

#ifndef UNUM_HLS_GLAYER_H
#define UNUM_HLS_GLAYER_H

#include "ap_fixed.h"
#include "uenv.h"

typedef struct {
    ap_int<esizemax+1+1> e = 0; //extra 1 for potential overflow and 1 for sign
    ap_fixed<integersize+fractionsize,integersize> f = 0;
    ap_uint<1> open = 1;
    ap_uint<1> inf = 1;
} gnum_s;

typedef struct {
    gnum_s l;
    gnum_s r;
    ap_uint<1> nan = 1;
} gbnd_s;

void print_gbnd(gbnd_s gbnd);

#endif //UNUM_HLS_GLAYER_H
