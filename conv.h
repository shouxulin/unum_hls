//
// Created by kiraMac on 9/4/18.
//

#ifndef UNUM_HLS_CONV_H
#define UNUM_HLS_CONV_H

#include "ulayer.h"
#include "uenv.h"
#include "glayer.h"

void u2f(unum_s *u,gnum_s* f);

void unum2g(unum_s *u, gbnd_s *gbnd);
void ubnd2g(ubnd_s *ubnd, gbnd_s *gbnd);
void f2u(gnum_s *gnum, unum_s* unum);


#endif //UNUM_HLS_CONV_H
