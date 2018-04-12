//
// Created by kiraMac on 9/4/18.
//

#include "conv.h"
#include "support.h"

void u2f(gnum_s* f, const unum_s u){
    utag_s ut{};
    utag(&ut,u);

    //exponent
    f->e = u.range(ut.esize+utagsize+ut.fsize-1,utagsize+ut.fsize);
    int hidden = (f->e > 0);
    long bias = (1UL<<(ut.esize-1)) - 1;
    f->e = f->e - bias + 1 -hidden;

    //printf("exponent value: %s\n",f->e.to_string(10).c_str());

    //fraction
    (f->f)(fractionsize-1,fractionsize-ut.fsize) = u(utagsize+ut.fsize-1, utagsize);
    f->f[fractionsize] = hidden;

    //sign
    f->f = (f->f) * (u.test(ut.esize+ut.fsize+utagsize)?-1:1);

    //printf("fraction value: %s\n",f->f.to_string(2).c_str());
}

void unum2g(gbnd_s* a, unum_s u){
    // NaN
    if (nanQ(u)){
        printf("NaN\n");
        a->nan = 1;
        a->l.e = a->l.f = a->r.e = a->r.f = 0;
        a->l.open = a->r.open = 1;
        a->l.inf = a->r.inf = 0;
    } else{
        utag_s ut{};
        utag(&ut,u);

        if (inf(u)){ //infinity [inf,inf]
            a->nan = 0;
            a->l.inf = a->r.inf = 1;
            a->l.open = a->r.open = 0;
            if (u.test(utagsize+ut.fsize+ut.esize)){ //negative inf
                printf("negative inf\n");
                a->l.f = a->r.f = 1;
            } else{//positive inf
                a->l.f = a->r.f = -1;
                printf("positive inf\n");
            }
            return;
        }

        if (!u.test(utagsize-1)){//exact
            printf("exact\n");
            a->nan = 0;
            u2f(&(a->l),u);
            u2f(&(a->r),u);
            a->l.inf = a->r.inf = 0;
            a->l.open = a->r.open = 0;
            return;
        }

        //the interval is open
        a->nan = 0;
        a->l.open = a->r.open = 1;

        unum_s tmp = 0;
        bigu(&tmp,u);

        tmp.set(static_cast<uint32_t>(utagsize - 1));//set the ubit

        if (u==tmp){ //return (bigu,Inf)
            u2f(&(a->l), u); a->r.f = 1;
            a->l.inf = 0; a->r.inf = 1;
            printf("(bigu,Inf)\n");
            return;
        }

        tmp.set(ut.esize+ut.fsize+utagsize);
        if (u==tmp){ //return (-Inf,-bigu)
            u2f(&(a->r),u);a->l.f=-1;
            a->l.inf=1;a->r.inf=0;

            printf("(-Inf,-bigu)\n");
            return;
        }

        //u+ulp
        tmp = u;
        tmp.clear(static_cast<uint32_t>(utagsize - 1));//clear the ubit
        tmp = tmp + (((unum_s)1)<<utagsize);//u+ulp
        //printf("tmp: %s\n",tmp.to_string(2).c_str());
        a->l.inf=a->r.inf=0;
        if (!u.test(ut.esize+ut.fsize+utagsize)){
            printf("(u.u+ulp)\n");
            u2f(&(a->l),u);u2f(&(a->r),tmp);
        } else{
            printf("(-(u+ulp).-u)\n");
            u2f(&(a->r),u);u2f(&(a->l),tmp);
        }

    }
}