//
// Created by kiraMac on 13/4/18.
//

#include "gbnd.h"

void plusg(gbnd_s *a, const gbnd_s *x, const gbnd_s *y){
    /* If any value is NaN, the result is also NaN. */
    if (x->nan || y->nan){
        a->nan = 1;
        a->l.inf = a->r.inf = 0;
        a->l.open = a->r.open = 1;
        return;
    }

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
    }
}