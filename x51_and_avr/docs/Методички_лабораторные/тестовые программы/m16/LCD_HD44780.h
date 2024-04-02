#ifndef _LCD_HD44780_H_
#define _LCD_HD44780_H_

#include "PINDEF.H"

// ������ LCD-�������
#define LCD_WIDTH	16

// ������� ������ ����� � LCD
// ���� rs == 0 - ����� ������ �������
// ���� rs == 1 - ����� ������ ������
void LCD_write(unsigned char w_byte, unsigned char rs);

// ������� ������������� LCD
void LCD_init();

// ������� ������� ������ LCD
void LCD_clrscr();

// ������� ��������� ������� �������
void LCD_gotoxy(unsigned char x, unsigned char y);

// ������ ������ ������� �� �����
void LCD_printchar( char ch );

// ������� ������ ������ �� ������
void LCD_print( char string[] );

// ������� ������ ������ �� flash
void LCD_print_flash(flash unsigned char string[]);

#endif
