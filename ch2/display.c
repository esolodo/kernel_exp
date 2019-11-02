#include <stdint.h>

void show(){
    int len = 13;
    char * content = "hello world!\n";
    uint16_t * begin = (uint16_t*)0xC00B8000;
    int i = 0, j = 0;
    for (; i < len; i++, j += 2) {
        begin[j] = content[i] + 0x0200;
    }
}