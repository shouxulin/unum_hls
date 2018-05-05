//
// Created by kiraMac on 9/4/18.
//

#ifndef UNUM_HLS_CONV_H
#define UNUM_HLS_CONV_H

#include "ulayer.h"
#include "uenv.h"
#include "glayer.h"

void u2f(const unum_s *u,gnum_s* f);

/* Conversion of a unum to a general interval. */
void unum2g(const unum_s *u, gbnd_s *gbnd);

/* Conversion of a ubound to a general interval. */
void ubnd2g(const ubnd_s *ubnd, gbnd_s *gbnd);

/* Conversion of a unum or ubound to a general interval. */
void u2g(const ubnd_s *ubnd, gbnd_s *gbnd);

/* Conversion of a gnum to unum. */
void f2u(const gnum_s *gnum, unum_s* unum);

/* Conversion of a gbnd to ubnd. */
void g2u(const gbnd_s *gbnd, ubnd_s* ubnd);


#endif //UNUM_HLS_CONV_H
