//
// Created by kiraMac on 9/4/18.
//
#include "iostream"
#include "uenv.h"
#include "support.h"
#include "glayer.h"
#include "conv.h"
#include "ulayer.h"

void test(gnum_s g){
    g.f = 0;
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

    //test of ubnd2g

    unum_s u = 0b00111111100100000000000000000000011110110;
    unum_s v = 0b11000000110100101101100100010111011110110;
    ubnd_s ubnd;
    ubnd.p=1;
    ubnd.l=&u;
    ubnd.r=&v;
    gbnd_s gbnd;
    ubnd2g(&ubnd,&gbnd);
    print_gbnd(&gbnd);

    /*
    unum_s v = 0b11000000110100101101100100010111011110110;
    gnum_s g;
    u2f(&v,&g);
    g.inf=0;
    print_gnum(&g);
     */
}