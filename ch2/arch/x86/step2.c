#include "x86/multiboot.h"
#include "x86/vgaterm.h"
#include "vsprintf.h"


void mboothander(unsigned int magic, struct multiboot_header * header){
    char b[100];
    printk(b, "my name is %s age is %d", "piercewang", 18);
    vgaterm_writestring(b);

    ((uint16_t*)(VRAM_START))[0] = 0x0277;
}