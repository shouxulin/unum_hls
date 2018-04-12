//
// Created by kiraMac on 9/4/18.
//
#include "iostream"
#include "glayer.h"

void print_gbnd(gbnd_s gbnd){
    if (gbnd.nan){
        printf("(NaN,Nan)\n");
        return;
    }

    printf("%s",(gbnd.l.open)?"(":"[");
    if (gbnd.l.inf){
        printf("%sInf",(gbnd.l.f>0)?"":"-");
    } else{
        printf("%s*2^%s",gbnd.l.f.to_string(10).c_str(),gbnd.l.e.to_string(10).c_str());
    }

    printf(" , ");

    if (gbnd.r.inf){
        printf("%sInf",(gbnd.r.f>0)?"":"-");
    } else{
        printf("%s*2^%s",gbnd.r.f.to_string(10).c_str(),gbnd.r.e.to_string(10).c_str());
    }
    printf("%s\n",(gbnd.r.open)?")":"]");

}