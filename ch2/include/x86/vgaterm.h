
#ifndef __VGATERM__
#define __VGATERM__ 1

#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_BUFFSIZE VGA_WIDTH * VGA_HEIGHT

/* If you're going to redefine this (maybe you've remapped it) you MUST do so
 * before calling terminal_init!
 */
#define VRAM_START 0xC00B8000

/* Hardware text mode color constants. */

void vgaterm_init();
void vgaterm_cls();
void vgaterm_setcolor(uint8_t color);
uint8_t vgaterm_getcolor();
void vgaterm_putchar(char c);
void vgaterm_writestring(const char* data);
void vgaterm_moveto(uint8_t x, uint8_t y);



#endif