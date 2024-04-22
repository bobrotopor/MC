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

/*

1) При запуске программы на экран терминала выводится сообщение «Задача №2».
2) При нажатии клавиши 1 клавиатуры PC предлагается ввести время и дату,
кото-рые затем устанавливаются как текущие в RTC DS1307.
3) При нажатии клавиши 2 на ЖКИ стенда в первой строке выводится текущее время,
во второй дата.
4) При нажатии клавиши 3 происходит очистка ЖКИ.
Нажатие на другие клавиши игнорируется.

*/

#define CLOCK 22110000
#define TIMER_FREQ_HZ 1000
#define RELOAD_VALUE (65536 - (CLOCK / 12 / TIMER_FREQ_HZ))
#define RELOAD_VALUE_H ((unsigned char)(RELOAD_VALUE >> 8))
#define RELOAD_VALUE_L ((unsigned char)(RELOAD_VALUE))

int mode = 0;
unsigned char pc_keyboard_data = 0;
unsigned int ms_ctr = 0; // счетчик миллисекунд от начала работы программы

void init_timer(void)
{ // инициализация таймера
	TMOD = 0x01;
	TL0 = RELOAD_VALUE_L;
	TH0 = RELOAD_VALUE_H;
	ET0 = 1;
	TR0 = 1;
}

void change_time_n_date()
{ // установить новое время в микросхему DS1307 через UART
	unsigned char hour, minute, second;
	unsigned char day, month, year;

	printf("Enter hour: ");
	scanf("%bu", &hour);
	printf("Enter minute: ");
	scanf("%bu", &minute);
	printf("Enter second: ");
	scanf("%bu", &second);

	DS1307_settime(hour, minute, second);

	printf("Enter day: ");
	scanf("%bu", &day);
	printf("Enter month: ");
	scanf("%bu", &month);
	printf("Enter year: ");
	scanf("%bu", &year);

	DS1307_setdate(day, month, year);
	mode = 0; // тк нужно выполнить однократно устанавливаем режим бездейстивя
}

void display_time_n_date()
{						   // вывести время и дату на ЖК дисплей
	if (ms_ctr % 250 == 0) // обновляем данные каждые 250 мс
	{
		LCD_clrscr();
		char time[9];
		char date[9];
		DS1307_gettime(time);
		DS1307_getdate(date);
		LCD_gotoxy(0, 0);
		LCD_print(time);
		LCD_gotoxy( 0, 1 );
		LCD_print(date);
	}
}

void mode_handler()
{//обработчик режимов
	if (mode == 1)
		change_time_n_date();
	if (mode == 2)
		display_time_n_date();
}


void Serial_ISR(void) interrupt 4 using 0 
{// обработчик прерываний от UART
	if (RI)
	{
		scanf("%bu", &pc_keyboard_data);
		if (pc_keyboard_data == '1')
			mode = 1; // режим изменения даты и времени
		if (pc_keyboard_data == '2')
			mode = 2; // режим отображения даты и времени
		if (pc_keyboard_data == '3')
		{ // отчистка дисплея и установка режима бездейстивя
			mode = 0;
			LCD_clrscr();
		}
	}
}

void ISR(void) interrupt 1 using 0
{ // обработка прерывания милисекундного счётчика
	ms_ctr++;
	mode_handler();
}

void main()
{
	// Иниц LED, LCD, SPI, I2C, прерывания разрешены
	P0 = 0xFF;
	P1 = 0xFF;
	P2 = 0xFF;
	P3 = 0xFF;
	EA = 1;
	SCON = 0x50;
	TMOD = 0x20;
	TH1 = 250;
	TL1 = 250;
	TR1 = 1;
	TI = 1;
	ES = 1;

	init_timer();
	SPI_init();
	I2C_init();
	LCD_init();

	printf("Task_2");
	while (1)
	{
	}
}
