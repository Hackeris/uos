/*
 * screen.h
 *
 *  Created on: Nov 2, 2014
 *      Author: hackeris
 */

#ifndef _SCREEN_HEADER_
#define _SCREEN_HEADER_

/* Some screen stuff. */
/* The number of columns. */
#define COLUMNS                 80
/* The number of lines. */
#define LINES                   24
/* The attribute of an character. */
#define ATTRIBUTE               7
/* The video memory address. */
#define VIDEO                   0xB8000

/* Variables. */
/* Save the X position. */
extern int xpos;
/* Save the Y position. */
extern int ypos;
/* Point to the video memory. */
extern volatile unsigned char *video;


void cls(void);
void putchar(int c);


#endif
