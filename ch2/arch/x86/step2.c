#include "x86/multiboot.h"
#include "x86/vgaterm.h"


void mboothander(unsigned int magic, struct multiboot_header * header){
    char *h = "myhelloworld\n";
    vgaterm_writestring(h);
}