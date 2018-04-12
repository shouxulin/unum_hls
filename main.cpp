//
// Created by kiraMac on 9/4/18.
//
#include "iostream"
#include "uenv.h"
#include "support.h"
#include "glayer.h"
#include "conv.h"

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
    unum_s x = 0b01000100011000001101000001101111111110110;
    utag_s ut{};
    utag(&ut,x);
    printutag(ut);

    gbnd_s y{};
    unum2g(&y,x);
    print_gbnd(y);
}

