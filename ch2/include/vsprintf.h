/* vsprintf.c -- Lars Wirzenius & Linus Torvalds. */
/*
 * Wirzenius wrote this portably, Torvalds fucked it up :-)
 */
#ifndef __MINK_VSPRINTF_H
#define __MINK_VSPRINTF_H

#include <stdarg.h>

int vsprintf(char *buf, const char *fmt, va_list args);

#endif

void printk(char * buf, const char *fmt, ...) {

  va_list args;
  int i;

  va_start(args, fmt);
  i = vsprintf(buf,fmt,args);
  va_end(args);

  buf[i] = '\0';
}