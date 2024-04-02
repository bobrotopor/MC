#include <intrins.h>
#include "LCD_HD44780.h"
#include "SHIFT.h"
#include "DELAY.h"

void LCD_write(unsigned char w_byte, unsigned char rs)
{
	if(rs == 0)
		LCD_RS = 0;				// Режим записи команд
	else
		LCD_RS = 1;				// Режим записи данных
	
	LCD_E = 1;					// Выставляем строб для записи данных
	
	SHIFT_write(w_byte);		// Выводим данные в SPI порт

	_nop_();					// Ждем такт
	LCD_E = 0;					// Сбрасываем строб

	DELAY_MCS(500);				// Ждем пока данные будут обработаны
}                          	

void LCD_init()
{
	LCD_E = 0;
	LCD_RS = 0;
	DELAY_MCS(41000);			// Выдерживаем паузу после подачи питания на LCD
	
	LCD_write(0x30, 0);		// Необходимые действия для инициализации LCD
	DELAY_MCS(5000);
	LCD_write(0x30, 0);
	DELAY_MCS(700);
	LCD_write(0x30, 0);
	DELAY_MCS(700);
	LCD_write(0x38, 0);		// Инциализируем LCD: 2 строки, символы 5*7, 8-битный интерфейс
	LCD_write(0x01, 0);		// Очищаем экран
	DELAY_MCS(3000);
	LCD_write(0x06, 0);     // Задаем направление движения курсора - направо
	LCD_write(0x0C, 0);		// Включаем дисплей, выключаем курсор, выключаем мерцание
}

void LCD_clrscr()			// Эта функция очищает экран
{
	LCD_write(0x01, 0);
	DELAY_MCS(5000);
}

void LCD_gotoxy(unsigned char x, unsigned char y)	// Эта функция перемещает курсор на LCD в точку (x, y)
{
	if(y != 0)
		x += 0x40;
	x |= 0x80;													
	LCD_write(x, 0);
}

void LCD_printchar( unsigned char ch )
{
	LCD_write( ch, 1 );
}

void LCD_print( unsigned char string[] )				// Эта функция выводит строку на LCD дисплей
{																	// Если count < 0, то выводится вся строка
	while((*string) != 0)
		LCD_printchar(*(string++));
}
