//
// Created by kiraMac on 9/4/18.
//
#include "iostream"
#include "glayer.h"

void print_gbnd(gbnd_s *gbnd){
    if (gbnd->nan){
        printf("(NaN,Nan)\n");
        return;
    }

    printf("%s",(gbnd->l.open)?"(":"[");
    if (gbnd->l.inf){
        printf("%sInf",(gbnd->l.f>0)?"":"-");
    } else{
        printf("%s*2^%s",gbnd->l.f.to_string(10,true).c_str(),gbnd->l.e.to_string(10).c_str());
    }

    printf(" , ");

    if (gbnd->r.inf){
        printf("%sInf",(gbnd->r.f>0)?"":"-");
    } else{
        printf("%s*2^%s",gbnd->r.f.to_string(10,true).c_str(),gbnd->r.e.to_string(10).c_str());
    }
    printf("%s\n",(gbnd->r.open)?")":"]");

}

void print_gnum(gnum_s *gnum){
    printf("%s",(gnum->open)?"(":"[");
    if (gnum->inf){
        printf("%sInf",(gnum->f>0)?"":"-");
    } else{
        printf("%s*2^%s",gnum->f.to_string(10,true).c_str(),gnum->e.to_string(10).c_str());
    }
    printf("\n");
}

void gnum_e_shift(gnum_e_s* e, int bits){
    // bits > 0: left; bits <0; right
    if (bits>0){
        //shift left
        for (int i = 1; i <= bits; ++i) {
            *e = *e *2;
        }
    } else{
        bits = -bits;
        for (int i = 1; i <= bits; ++i) {
            *e = *e /2;
        }
    }
}

void gnum_f_shift(gnum_f_s *f, int bit){
    if (bit>0){
        //left shift
        for (int i = 0; i < bit; ++i) {
            *f = *f *2;
        }
    } else{
        //right shift
        bit = -bit;
        for (int i = 0; i < bit; ++i) {
            *f = *f /2;
        }
    }
}

gnum_e_s get_max_gnum_e(){
    gnum_e_s max_e;
    max_e = 1<<(esizemax-1);
    return max_e;
}

gnum_e_s get_min_gnum_e(){
    gnum_e_s min_e;
    min_e = 2-(1 << (esizemax-1));
    return min_e;
}


gnum_f_s get_min_gnum_f(){
    gnum_f_s min_f=0;
    min_f[fractionsize-fsizemax]=1;

    return min_f;
}

void init_gnum(gnum_s *gnum){
    gnum->e = 0; //extra 1 for potential overflow and 1 for sign
    gnum->f = 0;
    gnum->open = 1;
    gnum->inf = 1;
}

void init_gbnd(gbnd_s *gbnd){
    gbnd->nan = 1;
    init_gnum(&(gbnd->l));
    init_gnum(&(gbnd->r));
}