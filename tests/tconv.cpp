//
// Created by kiraMac on 14/5/18.
//
#include "iostream"
#include "../conv.h"
#include "../ulayer.h"
#include "../glayer.h"

using namespace std;
typedef std::numeric_limits< double > dbl;

double random_double(double fMin, double fMax)
{
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}

void printBits(size_t const size, void const * const ptr)
{
    unsigned char *b = (unsigned char*) ptr;
    unsigned char byte;
    int i, j;

    for (i=size-1;i>=0;i--)
    {
        for (j=7;j>=0;j--)
        {
            byte = (b[i] >> j) & 1;
            printf("%u", byte);
        }
    }
    //puts("");
}

int main(){
    printf("This is a test of basic conversion functions, like u2g, g2u!\n");
    srand((unsigned int)time(NULL));
    bool correct = true;
    for (int i = 0; i < 30; ++i) {
        printf("------- test %d ------\n",i);
        double d1 = random_double(-2,0);
        double d2 = random_double(0,2);

        while (d2<d1){
            d2 = random_double(-2,2);
        }
        cout.precision(dbl::max_digits10);
        cout << "d1: " << fixed << d1 << ", d2: "<< d2 << endl;
        //printBits(sizeof(d1),&d1);
        //printf(", ");
        //printBits(sizeof(d2),&d2);
        //printf("\n");

        unum_s u1 = 0;
        d2un(d1,&u1);
        //print_unum(&u1);
        //printf("\n");

        unum_s u2 = 0;
        d2un(d2,&u2);
        //print_unum(&u2);
        //printf("\n");

        ubnd_s ub;
        ub.p = 1;
        ub.l = u1;
        ub.r = u2;

        gbnd_s gb;
        u2g(&ub,&gb);
        gb.l.open = 0;
        gb.r.open = 0;
        //print_gbnd(&gb);
        string str1 = get_gbnd_str(&gb);
        //printf("%s\n",str1.c_str());

        ubnd_s ub2;
        init_ubnd(&ub2);
        g2u(&gb,&ub2);
        //print_ubnd_value(&ub2);

        //print_ubnd(&ub2);
        //printf("\n");

        gbnd_s gb2;
        u2g(&ub2,&gb2);
        string str2 = get_gbnd_str(&gb2);
        //printf("%s\n",str2.c_str());

        if (str1!=str2){
            correct = false;
            cout.precision(dbl::max_digits10);
            cout << "d1: " << fixed << d1 << ", d2: "<< d2 << endl;
            printf("%s\n",str1.c_str());
            printf("%s\n",str2.c_str());
        }
    }
    for (int i = 30; i < 60; ++i) {
        printf("------- test %d ------\n",i);
        double d1 = random_double(-10,10);
        double d2 = random_double(-10,10);

        //while (d2<d1){
         //   d2 = random_double(-10,10);
        //}
        cout.precision(dbl::max_digits10);
        cout << "d1: " << fixed << d1 << ", d2: "<< d2 << endl;
        //printBits(sizeof(d1),&d1);
        //printf(", ");
        //printBits(sizeof(d2),&d2);
        //printf("\n");

        unum_s u1 = 0;
        d2un(d1,&u1);
        //print_unum(&u1);
        //printf("\n");

        unum_s u2 = 0;
        d2un(d2,&u2);
        //print_unum(&u2);
        //printf("\n");

        ubnd_s ub;
        ub.p = 1;
        ub.l = u1;
        ub.r = u2;

        gbnd_s gb;
        u2g(&ub,&gb);
        gb.l.open = 1;
        gb.r.open = 1;
        //print_gbnd(&gb);
        string str1 = get_gbnd_str(&gb);
        //printf("%s\n",str1.c_str());

        ubnd_s ub2;
        init_ubnd(&ub2);
        g2u(&gb,&ub2);
        //print_ubnd_value(&ub2);

        //print_ubnd(&ub2);
        //printf("\n");

        gbnd_s gb2;
        u2g(&ub2,&gb2);
        string str2 = get_gbnd_str(&gb2);
        //printf("%s\n",str2.c_str());

        if (d1>d2){
            if (str2!="NaN"){
                correct = false;
                printf("%s\n",str1.c_str());
                printf("%s\n",str2.c_str());
                printf("it should be NaN\n");
            }
        } else{
            if (str1!=str2 ){
                correct = false;
                //cout.precision(dbl::max_digits10);
                //cout << "d1: " << fixed << d1 << ", d2: "<< d2 << endl;
                printf("%s\n",str1.c_str());
                printf("%s\n",str2.c_str());
            }
        }
    }

    printf("%s\n",correct?"all test passed":"tests failed");
    return 0;
}
