/*
 * string.h
 *
 *  Created on: Nov 2, 2014
 *      Author: hackeris
 */

#ifndef STRING_H_
#define STRING_H_

#include <types.h>

// compare two strings
int strcmp(const char* str1, const char* str2);

// copies string s2 to s1
char *strcpy(char *s1, const char *s2);

// returns length of string
size_t strlen(const char* str);

// copies count bytes from src to dest
void *memcpy(void *dest, const void *src, size_t count);

// sets count bytes of dest to val
void *memset(void *dest, char val, size_t count);

// sets count bytes of dest to val
unsigned short *memsetw(unsigned short *dest, unsigned short val, size_t count);

// locates first occurance of character in string
char* strchr(char * str, int character);

#endif /* STRING_H_ */
