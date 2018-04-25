//
// Created by kiraMac on 9/4/18.
//

#ifndef UNUM_HLS_ULAYER_H
#define UNUM_HLS_ULAYER_H

#include "ap_int.h"
#include "uenv.h"

typedef ap_uint<maxubits> unum_s;

typedef struct {
    ap_uint<1> p;
    unum_s *l;
    unum_s *r;
} ubnd_s;

unum_s get_maxrealu();

const unum_s maxrealu = get_maxrealu();

void print_unum(unum_s *u);


#endif //UNUM_HLS_ULAYER_H




