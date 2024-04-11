#include <stdio.h>
#include <intrins.h>
#include <string.h>
#include "KEYBOARD.H"
#include "SPI.H"
#include "LCD_HD44780.H"
#include "DELAY.H"
#include "LED7.H"
#include "DS1307.H"
#include "DS1820.H"
#include "AT24CXX.H"
#include "BEEP.H"
#include "I2C.H"
#include "SHIFT.H"

int x = 0;
int z = 0;
int s = 0;
int i = 0;
unsigned char y = 100;
unsigned char a = 0;
unsigned char key = 0;

#define CLOCK 22110000
#define TIMER_FREQ_HZ 1000
#define RELOAD_VALUE (65536 - (CLOCK / 12 / TIMER_FREQ_HZ))
#define RELOAD_VALUE_H ((unsigned char)(RELOAD_VALUE >> 8))
#define RELOAD_VALUE_L ((unsigned char)(RELOAD_VALUE))

void initTimer(void) // Функция инициализации таймера
{
	TMOD = 0x01;
	TL0 = RELOAD_VALUE_L;
	TH0 = RELOAD_VALUE_H;
	ET0 = 1;
	TR0 = 1;
}
void led(void) interrupt 1 using 0 // Функция, вызываемая по прерыванию
{
	x++;
	s++;
	key = KEY_getkey();
	if (key)
	{
		y = KEY_getkeynumber(key);
		a = y;
	}
}
void main() // Основная функция
{
	P0 = 0xFF; // Иниц LED, LCD, SPI, I2C, прерывания разрешены
	P1 = 0xFF;
	P2 = 0xFF;
	P3 = 0xFF;
	EA = 1;
	initTimer();
	SPI_init();
	I2C_init();
	LCD_init();
	LCD_clrscr();
	LCD_gotoxy(0, 0); // Вывод номера задачи
	LCD_print("Zadacha_1");
	while (1)
	{
		if (x > 50) // цикл мигания LED для проверки работы прерываний
		{
			LED1 = !LED1;
			x = 1;
		}
		LED7_setdigit(a); // семисегментник
		delay_ms(50);
		LCD_gotoxy(0, 1);
		if (y != "")
		{
			if (key == KEY_ASTERISK) // очистка экрана
			{
				LCD_clrscr();
				LCD_gotoxy(0, 0);
				LCD_print("Zadacha_4");
				z = 0;
			}
			if (key == KEY_BAR || (y > 9 && y < 14)) // сигнал по нажатию
			{
				while (i < 3)
				{
					s = 0;
					while (s < 30)
					{
						BEEP_BIT = 0;
					}
					BEEP_BIT = 1;
					s = 0;
					while (s < 10)
					{
						BEEP_BIT = 1;
					}
					i++;
				}
				i = 0;
				s = 0;
				while (s < 20)
				{
					BEEP_BIT = 1;
				}
				while (i < 3)
				{
					s = 0;
					while (s < 10)
					{
						BEEP_BIT = 0;
					}
					BEEP_BIT = 1;
					s = 0;
					while (s < 10)
					{
						BEEP_BIT = 1;
					}
					i++;
				}
				i = 0;
				s = 0;
				while (s < 20)
				{
					BEEP_BIT = 1;
				}
				while (i < 3)
				{
					s = 0;
					while (s < 30)
					{
						BEEP_BIT = 0;
					}
					BEEP_BIT = 1;
					s = 0;
					while (s < 10)
					{
						BEEP_BIT = 1;
					}
					i++;
				}
				i = 0;
				s = 0;
				while (s < 20)
				{
					BEEP_BIT = 1;
				}
				y = "";
			}
			if (y <= 9) // вывод на экран при нажатии
			{
				if (z < 16)
				{
					LCD_gotoxy(z, 1);
					LCD_printchar(y + '0');
					z++;
					y = "";
				}
				else
				{
					z = 0;
					LCD_gotoxy(z, 1);
					LCD_printchar(y + '0');
					z++;
					y = "";
				}
			}
			KEY_waitforkeyrelease();
		}
	}