//
// Created by kiraMac on 9/4/18.
//

#ifndef UNUM_HLS_ULAYER_H
#define UNUM_HLS_ULAYER_H

#include "ap_int.h"
#include "uenv.h"
#include "glayer.h"

typedef ap_uint<maxubits> unum_s;

typedef struct {
    ap_uint<1> p = 1;
    unum_s l;
    unum_s r;
} ubnd_s;

unum_s get_maxrealu();

const unum_s maxrealu = get_maxrealu();

void print_unum(const unum_s *u);

void print_ubnd(const ubnd_s *ubnd);

void init_ubnd(ubnd_s *ubnd);

#define qNaN(unum)\
    for (int i = 0; i <maxubits-1; i++){\
        (unum)->bit(i)=1;\
    }\
    (unum)->bit(maxubits-1) = 0;

#define posInfu(unum)\
    qNaN(unum);\
    (unum)->bit(utagsize-1) = 0;

#define negInfu(unum)\
    posInfu(unum);\
    (unum)->bit(maxubits-1) = 1;


#endif //UNUM_HLS_ULAYER_H




