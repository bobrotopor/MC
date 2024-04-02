#ifndef _LCD_HD44780_H_
#define _LCD_HD44780_H_

#include "PINDEF.H"

#define LCD_WIDTH	16

void LCD_write(unsigned char w_byte, unsigned char rs);
void LCD_init();
void LCD_clrscr();
void LCD_gotoxy(unsigned char x, unsigned char y);
void LCD_printchar( unsigned char ch );
void LCD_print(unsigned char string[]);

#endif
