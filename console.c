#include "console.h"



extern void console_putbytes(const char *s, int len){
    int i;
    for(i=0;i<len;i++){
        traite_car(s[i]);
    }
}
