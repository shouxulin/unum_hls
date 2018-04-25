//
// Created by kiraMac on 9/4/18.
//

#include "ulayer.h"
#include "support.h"

void print_unum(unum_s *u){
    utag_s ut;
    utag(&ut,u);
    // sign
    printf("%d ",u->bit(utagsize+ut.fsize+ut.esize)?1:0);
    // exponent
    for (int i = utagsize+ut.fsize+ut.esize-1; i >= utagsize+ut.fsize  ; --i) {
        printf("%d",u->bit(i)?1:0);
    }
    printf(" ");
    // fraction
    for (int i = utagsize+ut.fsize-1; i >= utagsize; --i) {
        printf("%d",u->bit(i)?1:0);
    }
    printf(" ");
    // ubit
    printf("%d ",u->bit(utagsize-1)?1:0);
    // ess
    for (int i = fsizesize+esizesize-1; i >= fsizesize; --i) {
        printf("%d",u->bit(i)?1:0);
    }
    printf(" ");
    // fss
    for (int i = fsizesize-1; i >= 0; --i) {
        printf("%d",u->bit(i)?1:0);
    }
    printf("\n");

}

unum_s get_maxrealu(){
    unum_s result = 0;
    for (int i = 0; i < maxubits-1; ++i) {
        result[i]=1;
    }
    result[utagsize]=0;
    return result;
}
