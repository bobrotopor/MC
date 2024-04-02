#ifndef _LCD_HD44780_H_
#define _LCD_HD44780_H_

#include "PINDEF.H"

// Ширина LCD-дисплея
#define LCD_WIDTH	16

// Функция записи байта в LCD
// Если rs == 0 - режим записи комманд
// Если rs == 1 - режим записи данных
void LCD_write(unsigned char w_byte, unsigned char rs);

// Функция инициализации LCD
void LCD_init();

// Функция очистки экрана LCD
void LCD_clrscr();

// Функция установки позиция курсора
void LCD_gotoxy(unsigned char x, unsigned char y);

// Функия вывода символа на экран
void LCD_printchar( char ch );

// Функция вывода строки из памяти
void LCD_print( char string[] );

// Функция вывода строки из flash
void LCD_print_flash(flash unsigned char string[]);

#endif
