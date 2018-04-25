//
// Created by kiraMac on 13/4/18.
//

#include "gbnd.h"

void plusg(gbnd_s *a, const gbnd_s x, const gbnd_s y){
    /* If any value is NaN, the result is also NaN. */
    if (x.nan || y.nan){
        a->nan = 1;
        a->l.inf = a->r.inf = 0;
        a->l.open = a->r.open = 1;
        return;
    }

    /* Compute left endpoint: */
    /* Cases involving exact infinity or -infinity: */

}