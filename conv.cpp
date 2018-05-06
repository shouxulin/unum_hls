//
// Created by kiraMac on 9/4/18.
//

#include "conv.h"
#include "support.h"
#include "glayer.h"

void u2f(const unum_s *u,gnum_s* f){
    utag_s ut;
    utag(&ut,u);

    //exponent
    f->e = u->range(ut.esize+utagsize+ut.fsize-1,utagsize+ut.fsize);
    ap_uint<1> hidden = ((u->range(utagsize+ut.fsize+ut.esize-1,utagsize+ut.fsize)).or_reduce());
    //printf("hideen:%s\n",hidden.to_string().c_str());
    long bias = (1UL<<(ut.esize-1)) - 1;
    f->e = f->e - bias + 1 -hidden;

    //printf("exponent value: %s\n",f->e.to_string(10).c_str());

    //fraction
    //printf("ftest:%s\n:",u->range(utagsize+ut.fsize-1, utagsize).to_string(2).c_str());
    (f->f).range(fractionsize-1,fractionsize-ut.fsize) = u->range(utagsize+ut.fsize-1, utagsize);
    //f->f = f->f + (hidden?1:0);
    f->f.bit(fractionsize) = hidden;

    //sign
    f->f = (f->f) * (u->test(ut.esize+ut.fsize+utagsize)?-1:1);

    // printf("fraction value: %s\n",f->f.to_string(2,true).c_str());
}

void unum2g(const unum_s *u, gbnd_s *gbnd){
    // NaN
    if (nanQ(u)){
        printf("NaN\n");
        gbnd->nan = 1;
        gbnd->l.e = gbnd->l.f = gbnd->r.e = gbnd->r.f = 0;
        gbnd->l.open = gbnd->r.open = 1;
        gbnd->l.inf = gbnd->r.inf = 0;
    } else{
        utag_s ut;
        utag(&ut,u);

        if (inf(u)){ //infinity [inf,inf]
            gbnd->nan = 0;
            gbnd->l.inf = gbnd->r.inf = 1;
            gbnd->l.open = gbnd->r.open = 0;
            if (u->test(utagsize+ut.fsize+ut.esize)){ //negative inf
                printf("negative inf\n");
                gbnd->l.f = gbnd->r.f = 1;
            } else{//positive inf
                gbnd->l.f = gbnd->r.f = -1;
                printf("positive inf\n");
            }
            return;
        }

        if (!u->test(utagsize-1)){//exact
            // printf("exact\n");
            gbnd->nan = 0;
            u2f(u,&(gbnd->l));
            u2f(u,&(gbnd->r));
            gbnd->l.inf = gbnd->r.inf = 0;
            gbnd->l.open = gbnd->r.open = 0;
            return;
        }

        //the interval is open
        gbnd->nan = 0;
        gbnd->l.open = gbnd->r.open = 1;

        unum_s tmp = 0;
        bigu(&tmp,u);

        tmp.set(static_cast<uint32_t>(utagsize - 1));//set the ubit

        if (*u==tmp){ //return (bigu,Inf)
            u2f(u, &(gbnd->l)); gbnd->r.f = 1;
            gbnd->l.inf = 0; gbnd->r.inf = 1;
            printf("(bigu,Inf)\n");
            return;
        }

        tmp.set(ut.esize+ut.fsize+utagsize);
        if (*u==tmp){ //return (-Inf,-bigu)
            u2f(u, &(gbnd->r));gbnd->l.f=-1;
            gbnd->l.inf=1;gbnd->r.inf=0;

            printf("(-Inf,-bigu)\n");
            return;
        }

        //u+ulp
        tmp = *u;
        tmp.clear(static_cast<uint32_t>(utagsize - 1));//clear the ubit
        tmp = tmp + (((unum_s)1)<<utagsize);//u+ulp
        //printf("tmp: %s\n",tmp.to_string(2).c_str());
        gbnd->l.inf=gbnd->r.inf=0;
        if (!u->test(ut.esize+ut.fsize+utagsize)){
            printf("(u.u+ulp)\n");
            u2f(u, &(gbnd->l));u2f(&tmp, &(gbnd->r));
        } else{
            printf("(-(u+ulp).-u)\n");
            u2f(u, &(gbnd->r));u2f(&tmp, &(gbnd->l));
        }

    }
}

void ubnd2g(const ubnd_s *ubnd, gbnd_s *gbnd){
    /** If either of the ubnd is NaN, return NaN */
    if (nanQ(&(ubnd->l)) || nanQ(&(ubnd->r))){
        gbnd->nan = 1;
        return;
    }

    gbnd->nan = 0;

    gbnd_s gtmp;
    init_gbnd(&gtmp);

    /** Left end point */
    unum2g(&(ubnd->l),&gtmp);
    gbnd->l.f = gtmp.l.f;
    gbnd->l.e = gtmp.l.e;
    gbnd->l.inf = gtmp.l.inf;
    gbnd->l.open = gtmp.l.open;

    /** Right end point */
    init_gbnd(&gtmp);
    unum2g(&(ubnd->r),&gtmp);
    gbnd->r.f = gtmp.r.f;
    gbnd->r.e = gtmp.r.e;
    gbnd->r.inf = gtmp.r.inf;
    gbnd->r.open = gtmp.r.open;
}

void u2g(const ubnd_s *ubnd, gbnd_s *gbnd){
    if (ubnd->p){
        ubnd2g(ubnd,gbnd);
    } else{
        unum2g(&(ubnd->l), gbnd);
    }
}

void normalize(gnum_s* gnum){
    //check if mantissa underflow
    while (gnum->f<1 && gnum->f>-1){
        printf("mantissa underflow\n");

        //gnum_e_s smallest_exponent = 1-((1<<(esizemax-1))-1);
        //printf("smallest exponent:%s\n",smallest_exponent.to_string(10).c_str());
        if (gnum->e<=MIN_GNUM_E){
            //printf("the hidden bit must be 0\n");
            break;
        }

        //left shift mantissa
        gnum_f_shift(&(gnum->f),1);

        //decrease exponent
        gnum->e --;

        //printf("%s * 2^%s\n",gnum->f.to_string(2).c_str(),gnum->e.to_string(10).c_str());

        /**
        //check if exponent can be expressed as: 0 + 1 - bias
        gnum_e_s bias = 0;
        bool zero=false;
        for (int es = 1; es <= esizemax; ++es) {
            bias = (1<<(es-1))-1;
            if (gnum->e == 1 - bias){
                zero = true;
                break;
            }
            printf("%d:%s,%s\n",es,bias.to_string(10).c_str(),zero?"yes":"no");
        }
         */
    }
}

void f2u(const gnum_s *gnum, unum_s* unum){
    gnum_s g = *gnum;
    // check if mantissa overflow
    if ((g.f>=2 || g.f<=-2)){
        printf("mantissa overflow\n");

        //check if the truncated bit(i.e. LSB bit), if it is 1 => set the ubit
        unum->bit(utagsize-1) = g.f[0];
        //printf("last bit:%d\n",last_bit>0?1:0);

        //shift right the mantissa
        gnum_f_shift(&(g.f),-1);
        //printf("f: %s\n",gnum->f.to_string(2,true).c_str());

        //increase the exponent
        g.e ++;
        //printf("e: %s\n",gnum->e.to_string(2,true).c_str());
    }

    // normalize
    normalize(&g);

    /* Zero is a special case. The smallest unum for it is just 0: */
    if (g.f==0){
        printf("it is 0!\n");
        *unum = 0;
        return;
    }

    /* Magnitudes too large to represent:
       i.e. exponent is too larget -> return maxreal with ubit set: (maxreal,Inf) */
    if (g.e>MAX_GNUM_E){
        printf("Magnitudes too large to represent\n");
        *unum = maxrealu;
        unum->bit(utagsize-1) = 1;
        if (g.f<0){
            unum->bit(maxubits-1)=1;
        }
        return;
    }

    /* Magnitudes too small to represent become "inexact zero"
	   with the maximum exponent and fraction field sizes: */
    if (g.e<MIN_GNUM_E){
        printf("Magnitudes too small to represent\n");
        *unum = 0;
        unum->bit(utagsize-1)=1;
        if (g.f<0){
            unum->bit(maxubits-1)=1;
        }
        return;
    }

    /* Compute the fraction */
    //printf("gf:%s\n",gnum.f.to_string(2).c_str());
    int fs = 1;
    for (int i = 0; i < fractionsize; ++i) {
        if (g.f[i]==1){
            fs = fractionsize-i;
            break;
        }
    }

    // printf("fs:%d\n",fs);

    if (fs <= fsizemax){
        unum->range(fsizesize-1,0) = fs-1;
        unum->range(utagsize+fs-1,utagsize) = (g.f>0?(g.f.range(fractionsize-1,fractionsize-fs)):((-(g.f)).range(fractionsize-1,fractionsize-fs)));
    }


    /* Compute the exponent */
    gnum_e_s tmp = 0;
    gnum_e_s bias = 0;
    ap_uint<1> hold = 0;
    for (int es = 1; es <= esizemax; ++es){
        // test the min
        tmp = 2-(1 << (es-1));
        //printf("max: %s\n",tmp.to_string(2,true).c_str());
        if (g.e<tmp){
            continue;
        }
        //break;

        //test of max
        tmp = (1<<(es-1));
        //printf("min: %s\n",tmp.to_string(2,true).c_str());
        if (g.e>tmp){
            continue;
        }

        // this es can hold
        //printf("es:%d\n",es);
        hold = 1;
        unum->range(fsizesize+esizesize-1,fsizesize) = es-1;
        bias = (1<<(es-1))-1;
        unum->range(utagsize+fs+es-1, utagsize+fs) = g.e+bias;

        if (g.f<0){
            printf("negative\n");
            unum->bit(utagsize+es+fs) = 1;
        }
        break;
    }

    if (!hold){
        printf("current environment can not hold\n");
    }
}

void g2u(const gbnd_s *gbnd, ubnd_s* ubnd){
    /* Handle NaN cases first. */
    if (   gbnd->nan ||
           (  gbnd->l.inf  && !gbnd->r.inf   && gbnd->l.f > 0) ||
           (! gbnd->l.inf  &&  gbnd->r.inf   && gbnd->r.f < 0) ||
           (!(gbnd->l.inf   ^  gbnd->r.inf)  && gbnd->l.f > gbnd->r.f) ||
           ((gbnd->l.open  ||  gbnd->r.open) && gbnd->l.f == gbnd->r.f)
            )
    {
        printf("this interval is NaN!\n");
        ubnd->p = 0;
        qNaN(&(ubnd->l));
        qNaN(&(ubnd->r));
        return;
    }
    /* Handle Inf cases */
    ubnd->p = 1;
    if (gbnd->l.inf){
        if (gbnd->l.f>0) {
            posInfu(&(ubnd->l))
        } else {
            negInfu(&(ubnd->l));
        }
    } else {
        f2u(&(gbnd->l),&(ubnd->l));
    }
    if (gbnd->r.inf){
        if (gbnd->r.f>0) {
            posInfu(&(ubnd->r))
        } else {
            negInfu(&(ubnd->r));
        }
    } else {
        f2u(&(gbnd->r),&(ubnd->r));
    }

    /* If both unums are identical, and both open or both closed,
	   we have a single unum bound. */
    if (!(gbnd->l.open ^ gbnd->r.open) && (ubnd->l == ubnd->r)){
        ubnd->p = 0;
        return;
    }

    // TODO: See if general interval is expressible as a single unum without loss.
}