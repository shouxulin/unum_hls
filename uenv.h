//
// Created by kiraMac on 9/4/18.
//

#ifndef UNUM_HLS_UENV_H
#define UNUM_HLS_UENV_H

#include "ap_int.h"

const unsigned int esizesize = 4;
const unsigned int fsizesize = 6;
const unsigned int utagsize = esizesize+fsizesize+1;
const unsigned int esizemax = 1<<esizesize;
const unsigned int fsizemax = 1<<fsizesize;
const unsigned int maxubits = 1+esizemax+fsizemax+utagsize;

const unsigned int integersize = 3; //1 for hidden bit, 1 for potential hidden bit overflow, one for sign
const unsigned int fractionsize = fsizemax;

void print_env();

#endif //UNUM_HLS_UENV_H