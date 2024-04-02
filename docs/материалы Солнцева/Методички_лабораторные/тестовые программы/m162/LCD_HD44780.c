#include "LCD_HD44780.h"
#include "SHIFT.h"
#include "DELAY.h"

// Функция записи байта в LCD
// Если rs == 0 - режим записи комманд
// Если rs == 1 - режим записи данных
void LCD_write(unsigned char w_byte, unsigned char rs)
{
	if(rs == 0)
		LCD_RS_PORT = 0;	// Режим записи команд
	else
		LCD_RS_PORT = 1;	// Режим записи данных
	
	LCD_E_PORT = 1;			// Выставляем строб для записи данных
	
	SHIFT_write(w_byte);	// Выводим данные в SPI порт

	__no_operation();		// Ждем такт
	LCD_E_PORT = 0;			// Сбрасываем строб

	DELAY_MCS(500);			// Ждем пока данные будут обработаны
}                          	

// Функция инициализации LCD
void LCD_init()
{
	LCD_E_DDR = 1;
	LCD_RS_DDR = 1;
	
	LCD_E_PORT = 0;
	LCD_RS_PORT = 0;
	DELAY_MS(41);			// Выдерживаем паузу после подачи питания на LCD
	
	LCD_write(0x30, 0);		// Необходимые действия для инициализации LCD
	DELAY_MS(5);
	LCD_write(0x30, 0);
	DELAY_MCS(700);
	LCD_write(0x30, 0);
	DELAY_MCS(700);
	LCD_write(0x38, 0);		// Инциализируем LCD: 2 строки, символы 5*7, 8-битный интерфейс
	LCD_write(0x01, 0);		// Очищаем экран
	DELAY_MS(3);
	LCD_write(0x06, 0);     // Задаем направление движения курсора - направо
	LCD_write(0x0C, 0);		// Включаем дисплей, выключаем курсор, выключаем мерцание
}

// Функция очистки экрана LCD
void LCD_clrscr()
{
	LCD_write(0x01, 0);
	DELAY_MS(5);
}

// Функция установки позиция курсора
void LCD_gotoxy(unsigned char x, unsigned char y)
{
	if(y != 0)
		x += 0x40;
	x |= 0x80;													
	LCD_write(x, 0);
}

// Функия вывода символа на экран
void LCD_printchar( char ch )
{
	LCD_write( ch, 1 );
}

// Функция вывода строки из памяти
void LCD_print( char string[] )				
{									
	while((*string) != 0)
		LCD_printchar(*(string++));
}

// Функция вывода строки из flash
void LCD_print_flash(flash unsigned char string[])
{
		while((*string) != 0)
		LCD_printchar(*(string++));
}
