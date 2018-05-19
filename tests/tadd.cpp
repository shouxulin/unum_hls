//
// Created by kiraMac on 15/5/18.
//
#include "../unum.h"
#include "../conv.h"
#include "../glayer.h"
#include "../ubnd.h"
#include "../support.h"
#include <limits>
#include "float.h"


using namespace std;
typedef std::numeric_limits< double > dbl;

double random_double(double fMin, double fMax)
{
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}

float random_float(float a, float b) {
    float random = ((float) rand()) / (float) RAND_MAX;
    float diff = b - a;
    float r = random * diff;
    return a + r;
}

float rand_FloatRange(float a, float b)
{
    return ((b - a) * ((float)rand() / RAND_MAX)) + a;
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

bool test(double_str expected, const ubnd_s *ubnd){
    ap_uint<11> e = expected.range(62,52);
    int bias = (1<<(11-1))-1;
    bool e_all_0 = !e.or_reduce();
    int e_value = e.to_int() - bias + (e_all_0?1:0);
    //printf("e value: %d\n",e_value);

    gbnd_s gb;
    init_gbnd(&gb);
    u2g(ubnd,&gb);

    utag_s ut;
    utag(&ut,&(ubnd->l));

    // if the ubit is not set
    if (!ubnd->l.test(utagsize-1)){ // an exact value
        printf("it is an exact value\n");

        // check the exponent
        if (gb.l.e.to_int()!=e_value){
            return false;
        }

        // check the fraction
        for (int i = 0; i < ut.fsize; ++i) {
            if (ubnd->l.bit(utagsize+ut.fsize-1-i) != expected.bit(51-i)){
                return false;
            }
        }

        for (int j = 0; j < 52-ut.fsize; ++j) {
            if (expected.test(j)){
                return false;
            }
        }
    } else{
        printf("it is an interval\n");
        unum_s exactu = ubnd->l;
        exactu.bit(utagsize-1) = 0;

        unum_s bigunum = 0;
        bigu(&bigunum,&(ubnd->l));

        unum_s negbigunum = 0;
        bigu(&negbigunum,&(ubnd->l));
        negbigunum.bit(utagsize+ut.esize+ut.fsize) = 1;

        if (exactu == bigunum){
            printf("the interval is (bigu,+Inf)\n");
            //check exponent
            if (gb.l.e.to_int() > e_value){
                return false;
            }
        }

        if (exactu == negbigunum){
            printf("the interval is (-Inf,-bigu)\n");
            if (gb.r.e.to_int() < e_value){
                return false;
            }
        }

        if (!expected.test(63)){
            //printf("the result is positive\n");

            // compare the left endpoint

            // check the exponent
            if (gb.l.e.to_int()!=e_value){
                //printf("the left end e is wrong\n");
                return false;
            }

            // check the fraction
            for (int i = 0; i < ut.fsize; ++i) {
                if (ubnd->l.bit(utagsize+ut.fsize-1-i) != expected.bit(51-i)){
                    return false;
                }
            }

            // compare the right endpoint

        } else {
            //printf("the result is negative\n");
            // compare the right endpoint

            // check the exponent
            if (gb.r.e.to_int()!=e_value){
                return false;
            }

            // check the fraction
            for (int i = 0; i < ut.fsize; ++i) {
                if (ubnd->l.bit(utagsize+ut.fsize-1-i) != expected.bit(51-i)){
                    return false;
                }
            }
        }
    }
    return true;

}

int main(){
    //unum_s u = 0;
    //d2un(2,&u);
    //unum_s v = 0;
    //d2un(3,&v);
    //ubnd_s result;
    ///plus_unum(&result, &u, &v);
    //print_ubnd_value(&result);

    srand((unsigned int)time(NULL));

    const float posinf = std::numeric_limits<float>::infinity();
    const float neginf = -posinf;
    const float max_float =FLT_MAX;
    const float min_float = -FLT_MAX;
    //printf("%f,%f\n",posinf,neginf);

    /* ------ Randome test ------ */
    int no_tests = 200;
    bool test_result = true;
    for (int i = 0; i < no_tests; ++i) {

        //printf("min:%f, max:%f\n",min_float,max_float-100);

        float d1 = rand_FloatRange(min_float/2, max_float/2);
        float d2 = random_float(min_float/2, max_float/2);
        float d3 = d1 + d2;
        double d4 = (double)d1 + (double)d2;
        double_str d4_str = get_double_bits(&d4);
        //printf("d1:%f, d1:%f, d3:%f, d4:%f\n",d1,d2,d3,d4);

        //printBits(sizeof(d1), &d1);
        //printf(" + ");
        //printBits(sizeof(d2), &d2);
        //printf(" = ");
        //printBits(sizeof(d3), &d3);
        //printf("\n");

        //printBits(sizeof(d4), &d4);
        //printf("\n");

        unum_s u1 = 0;
        unum_s u2 = 0;
        unum_s u3 = 0;
        f2un(d1, &u1);
        f2un(d2, &u2);
        f2un(d3, &u3);

        //print_unum(&u1);
        //printf("\n");
        //print_unum(&u2);
        //printf("\n");
        //print_unum(&u3);
        //printf("\n");

        ubnd_s ub;
        init_ubnd(&ub);
        plus_unum(&ub,&u1,&u2);
        //print_ubnd(&ub);
        //printf("\n");
        //print_ubnd_value(&ub);

        gbnd_s gb;
        init_gbnd(&gb);
        u2g(&ub,&gb);
        //compare(d4_str, &(gb.l));

        bool compare_result = test(d4_str,&ub);
        //printf("%s\n",test_result?"true":"false");
        if (!compare_result){
            test_result=false;
        }



    }
    printf("%s\n",test_result?"true":"false");

    /* ------ test of a positive big number + a small number ------ */
    printf("------ test of a positive big number + a small number ------\n");
    test_result = true;
    for (int j = 0; j < no_tests; ++j) {
        float d1 = rand_FloatRange(0, max_float/2);
        float d2 = random_float(0, 1);
        double d4 = (double)d1 + (double)d2;

        double_str d4_str = get_double_bits(&d4);

        //printf("d1:%f, d2:%f, d4:%f\n",d1,d2,d4);

        //printBits(sizeof(d4), &d4);
        //printf("\n");

        unum_s u1 = 0;
        unum_s u2 = 0;
        f2un(d1, &u1);
        f2un(d2, &u2);

        ubnd_s ub;
        init_ubnd(&ub);
        plus_unum(&ub,&u1,&u2);

        gbnd_s gb;
        init_gbnd(&gb);
        u2g(&ub,&gb);

        //print_ubnd_value(&ub);

        bool compare_result = test(d4_str,&ub);
        if (!compare_result){
            test_result=false;
        }

    }
    printf("%s\n",test_result?"true":"false");

    /* ------ test of a negative big number + a small number ------ */
    printf("------ test of a negative big number + a small number ------\n");
    test_result = true;
    for (int j = 0; j < no_tests; ++j) {
        float d1 = rand_FloatRange(-max_float/2, 0);
        float d2 = random_float(0, 1);
        double d4 = (double)d1 + (double)d2;

        double_str d4_str = get_double_bits(&d4);

        //printf("d1:%f, d2:%f, d4:%f\n",d1,d2,d4);

        //printBits(sizeof(d4), &d4);
        ///printf("\n");

        unum_s u1 = 0;
        unum_s u2 = 0;
        f2un(d1, &u1);
        f2un(d2, &u2);

        ubnd_s ub;
        init_ubnd(&ub);
        plus_unum(&ub,&u1,&u2);

        gbnd_s gb;
        init_gbnd(&gb);
        u2g(&ub,&gb);

        //print_ubnd_value(&ub);

        bool compare_result = test(d4_str,&ub);
        //printf("%s\n",test_result?"true":"false");
        if (!compare_result){
            test_result=false;
        }
    }
    printf("%s\n",test_result?"true":"false");

    /* ------ test of overflow cases(positive) ------ */
    printf("------ test of overflow cases(positive) ------\n");
    test_result = true;
    float_str f1 = 0b01111111111111111111111111111111;
    float_str f2 = 0b01111010001010101010101010101010;
    unum_s u,v = 0;
    fstr2un(f1,&u);
    fstr2un(f2,&v);
    //print_unum(&u);
    //printf("\n");
    //print_unum(&v);
    //printf("\n");

    ubnd_s ub;
    init_ubnd(&ub);
    plus_unum(&ub,&u,&v);
    //print_ubnd(&ub);
    //printf("\n");
    //print_ubnd_value(&ub);

    gbnd_s gb;
    init_gbnd(&gb);
    u2g(&ub,&gb);

    if (gb.r.inf != 1 && gb.r.f <=0){
        test_result = false;
    }
    printf("%s\n",test_result?"true":"false");

    /* ------ test of overflow cases(positive) ------ */
    printf("------ test of overflow cases(negative) ------\n");
    test_result = true;
    f1 = 0b11111111111111111111111111111111;
    f2 = 0b11111010001010101010101010101010;
    u,v = 0;
    fstr2un(f1,&u);
    fstr2un(f2,&v);
    //print_unum(&u);
    //printf("\n");
    //print_unum(&v);
    //printf("\n");

    init_ubnd(&ub);
    plus_unum(&ub,&u,&v);
    //print_ubnd(&ub);
    //printf("\n");
    //print_ubnd_value(&ub);

    init_gbnd(&gb);
    u2g(&ub,&gb);

    if (gb.l.inf != 1 && gb.l.f ==0){
        test_result = false;
    }
    printf("%s\n",test_result?"true":"false");


    return 0;
}

