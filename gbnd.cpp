//
// Created by kiraMac on 13/4/18.
//

#include "gbnd.h"
#include "glayer.h"


ap_uint<1> plus_gnum(gnum_s *a, const gnum_s *x, const gnum_s *y){
    gnum_f_s xf = x->f;
    gnum_f_s yf = y->f;
    //compare the exponent
    gnum_e_s e_diff = x->e - y->e;
    //printf("the exponent diff:%s\n",e_diff.to_string(10).c_str());
    ap_uint<1> set_ubit = 0;
    if (e_diff>0){
        //the exponent of x > the exponent of y -> shift right the mantissa of y
        if (e_diff> INT_MAX){
            printf("the exponent difference exceeds the max int\n");
        }
        for (unsigned int i = 0; i < e_diff; ++i) {
            if (i!=integersize+fractionsize-1 && y->f.bit(i)!=0){
                set_ubit = 1;
                break;
            }
        }
        gnum_f_shift(&yf, (-e_diff));
        a->e = x->e;
    } else if (e_diff<0){
        //the exponent of y > the exponent of x -> shift right the mantissa of x
        if (e_diff> INT_MAX){
            printf("the exponent difference exceeds the max int\n");
        }
        for (unsigned int i = 0; i < -e_diff; ++i) {
            if (i!=integersize+fractionsize-1 && x->f.bit(i)!=0){
                set_ubit = 1;
                break;
            }
        }
        gnum_f_shift(&xf, e_diff);
        a->e = y->e;
    } else{
        a->e = x->e;
    }
    a->f = xf + yf;
    //TODO: handle the ubit case
    return set_ubit;
}

ap_uint<2> plusg(gbnd_s *a, const gbnd_s *x, const gbnd_s *y){
    ap_uint<2> result = 0;
    ap_uint<1> l = 0;
    ap_uint<1> r = 0;
    /* If any value is NaN, the result is also NaN. */
    if (x->nan || y->nan){
        a->nan = 1;
        //a->nan.set();
        a->l.inf = a->r.inf = 0;
        a->l.open = a->r.open = 1;
        return result;
    }
    a->nan = 0;

    /* Compute left endpoint: */
    /* Cases involving exact infinity or -infinity: */
    if (x->l.inf && x->l.f < 0 && !x->l.open) {
        if (y->l.inf && y->l.f > 0 && !y->l.open) {a->nan = 1;} //[-Inf,  + [Inf,  => NaN
        else {a->l.f = -1; a->l.inf = 1; a->l.open = 0;}        //[-Inf,  + [,  => [Inf, )
    } else if (y->l.inf && y->l.f < 0 && !y->l.open) {
        if (x->l.inf && x->l.f > 0 && !x->l.open) {a->nan = 1;} //[Inf, + [-Inf => NaN
        else {a->l.f = -1; a->l.inf = 1; a->l.open = 0;}        //[-,  + [,Inf  => [Inf,
    } else if (
            (x->l.inf && x->l.f > 0 && !x->l.open) ||
            (y->l.inf && y->l.f > 0 && !y->l.open)
            ) {
        a->l.f = 1; a->l.inf = 1; a->l.open = 0;
    } else if (x->l.inf && x->l.f < 0) {
        if (y->l.inf && y->l.f > 0 && !y->l.open){
            a->l.f = 1; a->l.inf = 1; a->l.open = 0;
        } else
        {
            a->l.f = -1; a->l.inf = 1; a->l.open = 1;
        }
    } else if (y->l.inf && y->l.f < 0) {
        if (x->l.inf && x->l.f > 0 && !x->l.open)
        {a->l.f = 1; a->l.inf = 1; a->l.open = 0;}
        else
        {a->l.f = -1; a->l.inf = 1; a->l.open = 1;}
    } else{
        //printf("this is the normal case\n");
        l = plus_gnum(&(a->l),&(x->l),&(y->l));
        a->l.inf = 0;
        a->l.open = x->l.open || y->l.open;
    }

    /* Compute left endpoint: */
    /* Cases involving exact infinity or -infinity: */
    if (x->r.inf && x->r.f < 0 && !x->r.open) {
        if (y->r.inf && y->r.f > 0 && !y->r.open) {a->nan = 1;} //[-Inf,  + [Inf,  => NaN
        else {a->r.f = -1; a->r.inf = 1; a->r.open = 0;}        //[-Inf,  + [,  => [Inf, )
    } else if (y->r.inf && y->r.f < 0 && !y->r.open) {
        if (x->r.inf && x->r.f > 0 && !x->r.open) {a->nan = 1;} //[Inf, + [-Inf => NaN
        else {a->r.f = -1; a->r.inf = 1; a->r.open = 0;}        //[-,  + [,Inf  => [Inf,
    } else if (
            (x->r.inf && x->r.f > 0 && !x->r.open) ||
            (y->r.inf && y->r.f > 0 && !y->r.open)
            ) {
        a->r.f = 1; a->r.inf = 1; a->r.open = 0;
    } else if (x->r.inf && x->r.f < 0) {
        if (y->r.inf && y->r.f > 0 && !y->r.open){
            a->r.f = 1; a->r.inf = 1; a->r.open = 0;
        } else
        {
            a->r.f = -1; a->r.inf = 1; a->r.open = 1;
        }
    } else if (y->r.inf && y->r.f < 0) {
        if (x->r.inf && x->r.f > 0 && !x->r.open)
        {a->r.f = 1; a->r.inf = 1; a->r.open = 0;}
        else
        {a->r.f = -1; a->r.inf = 1; a->r.open = 1;}
    } else{
        //printf("this is the normal case\n");
        r = plus_gnum(&(a->r),&(x->r),&(y->r));
        a->r.inf = 0;
        a->r.open = x->r.open || y->r.open;
    }

    result.bit(0) = r.to_int();
    result.bit(1) = l.to_int();
    return result;
}