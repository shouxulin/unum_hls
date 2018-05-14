//
// Created by kiraMac on 9/4/18.
//
#include "iostream"
#include "uenv.h"
#include "support.h"
#include "glayer.h"
#include "conv.h"
#include "ulayer.h"
#include "gbnd.h"

bool check_fraction(unum_s *u, double_str d){
    utag_s ut;
    utag(&ut,u);
    for (int i = 0; i < ut.fsize; ++i) {
        if (u->bit(utagsize+ut.fsize-1-i) != d.bit(52-1-i)){
            return false;
        }
    }
    for (int j = 0; j < 52-ut.fsize; ++j) {
        if (d.test(j)){
            return false;
        }
    }
    return true;
}

bool check(ubnd_s *ub, double_str expected){
    ap_uint<1> sign = expected.bit(63);
    ap_uint<11> e = expected.range(62,52);
    ap_uint<52> f = expected.range(51,0);

    int e_value = e - 1023;
    printf("expected e: %d\n",e_value);

    gbnd_s gb;
    ubnd2g(ub,&gb);
    print_gbnd(&gb);

    // check Inf of NaN first
    if (e.and_reduce()==1){
        if (!f.or_reduce()){
            printf("the double is Inf\n");
            return gb.l.inf && gb.r.inf;
        } else{
            printf("the double is NaN\n");
            return gb.nan == 1;
        }
    }

    if (gb.nan==1) return false;

    if (gb.l.inf==1 && gb.l.f >0) return false;

    if (gb.r.inf==1 && gb.r.f <0) return false;

    /*--- check exponent value ---*/
    // compare the left end point
    if (gb.l.inf==0){
        if (gb.l.e > e_value){
            return false;
        }
    }
    // compare the right end point
    if (gb.r.inf==0){
        if (gb.r.e < e_value){
            return false;
        }
    }

    /*--- check fraction ---*/
    // compare the left end point
    if (gb.l.inf==0){

    }
    // compare the right end point

    /*
    if (!ub->p){
        utag_s utl;
        utag(&utl,&(ub->l));
        int el = ub->l.range(utagsize+utl.fsize+utl.esize-1,utagsize+utl.fsize) - ((1<<(utl.esize-1))-1);
        printf("el: %d\n",el);
        if (el!=expected_e) return false;
        return check_fraction(&(ub->l),expected);
    } else{
        utag_s utl;
        utag(&utl,&(ub->l));
        int el = ub->l.range(utagsize+utl.fsize+utl.esize-1,utagsize+utl.fsize) - ((1<<(utl.esize-1))-1);
        utag_s utr;
        utag(&utr,&(ub->r));
        int er = ub->r.range(utagsize+utr.fsize+utr.esize-1,utagsize+utr.fsize) - ((1<<(utr.esize-1))-1);
        if (!(el <= expected_e &&  er >= expected_e)) return false;


    }
     */



    //bool b = check_fraction(&(ub->l),expected);
    //printf("%s\n",b?"yes":"no");
    return true;

}

int main(){
    print_env();
    /**
    unum_s u = 0b0100000001101100101110111000000000111010110;
    utag_s ut{};
    utag(&ut,u);
    printutag(ut);
     */

    //gnum_s f;
    //u2f(&f,u);

    //test the lenght of signed value string
    //ap_fixed<3,2> a = 1;
    //printf("%s,length:%d\n",a.to_string(2).c_str(),a.length());

    /**
    // test of inf and nan
    unum_s v = 0b01111111101111;
    bool i = inf(v);
    printf("%sinf\n",i?"":"not ");

    bool j = nanQ(v);
    printf("%sNaN\n",j?"":"not ");

    gbnd_s gbnd={};
    unum2g(&gbnd,v);
     */

    //test of bigu
    /**
    unum_s a = 0b01011011001;
    utag(&ut,a);
    printutag(ut);
    unum_s b;
    bigu(&b,a);
    printf("bigu: %s\n",b.to_string(2).c_str());
     */

    //test of (bigu,Inf)
    /**
    unum_s b = 0b11110111001;
    unum_s c;
    bigu(&c,b);
    //printf("bigu: %s\n",c.to_string(2).c_str());
    gbnd_s gb;
    unum2g(&gb,b);
    print_gbnd(gb);
     */

    //test of fixed type
    /**
    ap_fixed<5,3> f = 1.25;
    printf("f: %s\n",f.to_string(2).c_str());
    ap_fixed<5,3> g =0;
    g(4,2) = f(2,0);
    printf("g: %s\n",g.to_string(2).c_str());
     */

    //test of single precison
    /*
    unum_s x = 0b01000100011000001101000001101111111110110;
    utag_s ut;
    utag(&ut,&x);
    printutag(&ut);

    gbnd_s y;
    unum2g(&x,&y);
    print_gbnd(&y);
    */


    //test of fixed point type when shifting right and left
    /**
    ap_fixed<6,3, AP_RND_INF, AP_SAT> a = 0;
    printf("a: %s\n",a.to_string(2).c_str());
    ap_fixed<7,3, AP_RND_INF, AP_SAT> b = -(0.0625);
    printf("b: %s\n",b.to_string(2).c_str());
    ap_fixed<5,3, AP_RND_INF, AP_SAT> c = a+b;
    printf("c: %s\n",c.to_string(2).c_str());

    ap_fixed<4,2, AP_RND_INF, AP_SAT> d = 0.5+0.25;
    ap_fixed<3,2, AP_RND_INF, AP_SAT> e = d;
    printf("d: %s\n",d.to_string(2).c_str());
    printf("e: %s\n",e.to_string(2).c_str());


    ap_fixed<7,3, AP_RND_INF, AP_SAT> f = -(0.0625);
    printf("f: %s\n",f.to_string(2).c_str());
    //fraction_shift_right(&f,1);
    //ap_fixed<6,2> g = f << 3;
    printf("f after: %s\n",f.to_string(2).c_str());
    //printf("g: %s\n",g.to_string(2).c_str());

    for (int i = 6; i >=0 ; --i) {
        printf("%d",(f[i])?1:0);
    }
     */

    // test of setting the value of some bits in ap_uint
    /**
    ap_uint<3> a = 0;
    int aa = 3;
    a(2,1) = aa;
    printf("a: %s\n",a.to_string(2).c_str());
     */


    // test of shifting a ap_int(gnum exponent)
    /**
    ap_int<3> e = -3;
    printf("e      : %s\n",e.to_string(2,true).c_str());
    e_shift(&e,-2);
    //e = e << 1;
    printf("e after: %s\n",e.to_string(2,true).c_str());
     */

    // test of find_exponent
    /**
    ap_int<esizemax+1+1> e = 127;
    unum_s u = 0;
    find_exponent(e,&u);
    printf("u: %s\n",u.to_string(2).c_str());
    */

    // test of normalize
    /**
    gnum_s gnum;
    gnum.f = -(0+0.25);
    gnum.e = -125;
    printf("o f: %s\n",gnum.f.to_string(2).c_str());
    normalize(&gnum);
     */

    // test of 0 unum
    /**
    unum_s u = 00000000;
    gbnd_s gbnd;
    unum2g(&gbnd,u);
    print_gbnd(gbnd);
     */

    // test of f2u
    /**
    gnum_s g;
    g.f=-(1+0.328125);
    g.e = 10;
    g.inf=0;
    g.open=0;
    unum_s u;
    f2u(g,&u);
    printf("u:%s\n",u.to_string(2).c_str());

    print_unum(u);
     */
    /**
    unum_s u = 0b01000100110010101010001111000000011110110;
    print_unum(&u);
    gnum_s g;
    u2f(&u, &g);
    unum_s v;
    f2u(&g,&v);
    print_unum(&v);
     */

    // test of ubnd2g
    /*
    unum_s u = 0b00111111100100000000000000000000011110110;
    unum_s v = 0b01000000110100101101100100010111111110110;
    ubnd_s ubnd;
    ubnd.p=1;
    ubnd.l=&u;
    ubnd.r=&v;
    gbnd_s gbnd;
    ubnd2g(&ubnd,&gbnd);
    print_gbnd(&gbnd);

    unum2g(&v,&gbnd);
    print_gbnd(&gbnd);
     */

    /*
    unum_s v = 0b11000000110100101101100100010111011110110;
    gnum_s g;
    u2f(&v,&g);
    g.inf=0;
    print_gnum(&g);
     */

    // test of macro
    /*
    unum_s *q;
    qNaN(q);
    print_unum(q);
     */

    // test of g2u
    /*
    unum_s u = 0b00111111100100000000000000000000011110110;
    unum_s v = 0b01000000110100101101100100010111011110110;
    ubnd_s ubnd;
    ubnd.p=1;
    ubnd.l=&u;
    ubnd.r=&v;
    gbnd_s gbnd;
    ubnd2g(&ubnd,&gbnd);
    print_gbnd(&gbnd);

    ubnd_s ubnr;
    init_ubnd(&ubnr);
    g2u(&gbnd,&ubnr);
    print_unum(ubnr.l);
    print_unum(ubnr.r);
     */

    // test of plusg
    /*
    unum_s u = 0b00111111100100000000000000000000011110110;
    unum_s v = 0b01000000110100101101100100010111011110110;
    gbnd_s gu; gbnd_s gv; gbnd_s gr;
    unum2g(&u,&gu); unum2g(&v,&gv);
    print_gbnd(&gu);
    print_gbnd(&gv);
    plusg(&gr,&gu,&gv);
    print_gbnd(&gr);

    ubnd_s ub;
    init_ubnd(&ub);

    g2u(&gr,&ub);
    print_unum(&(ub.l));
     */

    /* random test */

    double d1 = 0.55679410628825154;
    double_str d_str1 = get_double_bits(&d1);
    printf("d1: %s\n",d_str1.to_string(2).c_str());

    unum_s u1 = 0;
    d2un(d1, &u1);
    print_unum(&u1);
    printf("\n");

    gbnd_s gb1;
    unum2g(&u1,&gb1);
    print_gbnd(&gb1);
    printf("\n");

    double d2 = 0.82218085174550337;
    double_str d_str2 = get_double_bits(&d2);
    printf("d2: %s\n",d_str2.to_string(2).c_str());

    unum_s u2 = 0;
    d2un(d2, &u2);
    //printf("%s\n",u.to_string().c_str());
    print_unum(&u2);
    printf("\n");

    gbnd_s gb2;
    unum2g(&u2,&gb2);
    print_gbnd(&gb2);
    printf("\n");

    double d3 = d1 + d2;
    double_str d_str3 = get_double_bits(&d3);
    printf("d3: %s\n",d_str3.to_string(2).c_str());

    gbnd_s gb3;
    plusg(&gb3,&gb1,&gb2);
    print_gbnd(&gb3);
    ubnd_s ub3;
    g2u(&gb3,&ub3);
    print_ubnd(&ub3);
    printf("\n");

    //bool b = check(&ub3,d_str3);
    //printf("%s\n",b?"yes":"no");




    /* test of negopenInfu and posopenInfu */
    /*
    unum_s u;
    negopenInfu(&u);
    print_unum(&u);
    printf("\n");

    unum_s v;
    posopenInfu(&v);
    print_unum(&v);
    printf("\n");
     */

    /* test of open/closed g2u */
    gbnd_s gb;
    gb.nan = 0;
    gb.l.inf = 0;
    gb.l.e = 0;
    gb.l.f = -1;
    gb.l.open = 1;
    gb.r.inf = 0;
    gb.r.e = 0;
    gb.r.f = 0;
    gb.r.open = 1;
    print_gbnd(&gb);

    ubnd_s ub;
    g2u(&gb,&ub);
    print_ubnd(&ub);
    printf("\n");

    print_ubnd_value(&ub);


}