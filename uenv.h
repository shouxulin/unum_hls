//
// Created by kiraMac on 9/4/18.
//

#ifndef UNUM_HLS_UENV_H
#define UNUM_HLS_UENV_H

#include "ap_int.h"

const int esizesize = 3;
const int fsizesize = 5;
const int utagsize = esizesize+fsizesize+1;
const int esizemax = 1<<esizesize;
const int fsizemax = 1<<fsizesize;
const int maxubits = 1+esizemax+fsizemax+utagsize;

const int integersize = 3; //1 for hidden bit, 1 for potential hidden bit overflow, one for sign
const int fractionsize = 2 * fsizemax;

void print_env();

#endif //UNUM_HLS_UENV_H