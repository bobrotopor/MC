#ifndef _PINDEF_H_
#define _PINDEF_H_

#include <AT898252.H>

#define BEEP_BIT	SS
#define KEY_PORT	P2
sbit LED1 = P1 ^ 0;
sbit LED2 = P1 ^ 1;
sbit LED3 = P3 ^ 5;
sbit OW_DATABIT	= P0 ^ 2;
sbit ST		= P3 ^ 4;
sbit LCD_RS	= P3 ^ 3;
sbit LCD_E 	= P3 ^ 2;
sbit SCL = P0 ^ 0;
sbit SDA = P0 ^ 1;

#endif
