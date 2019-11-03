
#include <stddef.h>
#include <stdint.h>


#include "sys.h"
#include "x86/vgaterm.h"
#include "utils.h"

static size_t term_cursor_y;
static size_t term_cursor_x;
static uint8_t term_color = 0x2;
static uint16_t* term_frontbuff = (uint16_t*)(VRAM_START);
static uint16_t term_backbuff[VGA_BUFFSIZE];

static uint16_t make_vgaentry(char c, uint8_t color) {
	uint16_t c16 = c;
	uint16_t color16 = color;
	return c16 | color16 << 8;
}

void vgaterm_cls() {
	memsetw(term_backbuff, make_vgaentry(0x20, term_color), VGA_WIDTH * VGA_HEIGHT);
	memcpy(term_frontbuff, term_backbuff, VGA_WIDTH * VGA_HEIGHT * 2);
}

void vgaterm_setcolor(uint8_t color) {
	term_color = color;
}

uint8_t vgaterm_getcolor() {
  return term_color;
}

static void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) {
	const size_t index = y * VGA_WIDTH + x;
	term_frontbuff[index] = term_backbuff[index] = make_vgaentry(c, color);
}

/* Scroll the screen up by a line */
static void terminal_scroll(void) {
  unsigned blank, temp;

  // fill the new line with a space in the current color.
  blank = make_vgaentry(0x20, term_color);

  // scroll in back buffer
  temp = term_cursor_y - VGA_HEIGHT + 1;
  memcpy (term_backbuff, term_backbuff + temp * VGA_WIDTH, (VGA_HEIGHT - temp) * VGA_WIDTH * 2);

  // clear last line
  memsetw (term_backbuff + (VGA_HEIGHT - temp) * VGA_WIDTH, blank, VGA_WIDTH);

  // move back to last line
  term_cursor_y = VGA_HEIGHT - 1;

  // and display.
  memcpy(term_frontbuff, term_backbuff, VGA_WIDTH * VGA_HEIGHT * 2);
}

/* Move the hardware cursor to the current position */
static void terminal_update_cursor(void) {
  unsigned temp = term_cursor_y * 80 + term_cursor_x;
  outportb(0x3D4, 14);
  outportb(0x3D5, temp >> 8);
  outportb(0x3D4, 15);
  outportb(0x3D5, temp);
}

void vgaterm_putchar(char c) {
  if (c == '\n') {
    // newline
    term_cursor_x = 0;
    term_cursor_y++;
  } else {
	  terminal_putentryat(c, term_color, term_cursor_x, term_cursor_y);
	  if (++term_cursor_x == VGA_WIDTH) {
		  term_cursor_x = 0;
		  term_cursor_y++;
	  }
	}

	// scroll if necessary
	if (term_cursor_y >= VGA_HEIGHT) {
	  terminal_scroll();
  }

  terminal_update_cursor();
}

void vgaterm_writestring(const char* data) {
	size_t datalen = strlen(data);
	for ( size_t i = 0; i < datalen; i++ )
		vgaterm_putchar(data[i]);
}

void vgaterm_moveto(uint8_t x, uint8_t y) {
  term_cursor_y = y;
  term_cursor_x = x;
  terminal_update_cursor();
}

