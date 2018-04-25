//
// Created by kiraMac on 9/4/18.
//

#include "support.h"

void utag(utag_s *ut,unum_s *u){
    /**
    ut->fsize = (*u & fsizemask)+1;
    ut->esize = ((*u & esizemask)>>fsizesize)+1;
    ut->ubit = (*u & ubitmask)>>(esizesize+fsizesize);
     */
    ut->fsize = static_cast<unsigned int>(u->range(fsizesize-1, 0) + 1);
    ut->esize = static_cast<unsigned int>(u->range(esizesize + fsizesize - 1, fsizesize) + 1);
    ut->ubit = (unsigned int) u[esizesize + fsizesize];
}

void printutag(utag_s *ut){
    printf("****utag\n");
    printf("esize:%d,fsize:%d,ubit:%d\n",ut->esize,ut->fsize,ut->ubit);
    printf("********\n");
}

bool nanQ(unum_s *u){
    return u->range(maxubits-2,0).and_reduce();
}

bool inf(unum_s *u){
    return (u->range(esizesize+fsizesize-1,0).and_reduce()) && (u->range(maxubits-1-1,utagsize).and_reduce());
}

void bigu(unum_s* v, unum_s *u){
    utag_s ut;
    utag(&ut,u);

    v->bit(ut.esize + ut.fsize + utagsize) = 1;
    //printf("v: %s\n",v->to_string(2).c_str());
    *v = *v - 1;
    if (ut.esize==esizemax && ut.fsize==fsizemax){
        v->bit(utagsize) = 0;
    }
    v->range(utagsize-2,0)=u->range(utagsize-2,0);
    v->clear(static_cast<uint32_t>(utagsize - 1));
}