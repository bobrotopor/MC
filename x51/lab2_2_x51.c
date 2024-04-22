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
кото-рые затем устанавливаются как текущие в RTC DS11507. 
При нажатии клавиши 2 на ЖКИ стенда в первой строке выводится текущее время, 
во второй дата. При нажатии клавиши 3 происходит очистка ЖКИ.
Нажатие на другие клавиши игнорируется.

*/

unsigned char task;

void ExecuteTask1() // ввод пользователем даты и времени
{
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
}

void ExecuteTask2() // вывод даты и времени на экран
{
	char time[9] = {0};
	char date[9] = {0};
	static char sec = 0;

	DS1307_gettime(time);
	if (time[7] == sec)
	{
		return;
	}
	sec = time[7];

	DS1307_getdate(date);

	LCD_gotoxy(0, 0);
	LCD_print(time);

	LCD_gotoxy(0, 1);
	LCD_print(date);
}

void ExecuteTask3() // очистка экрана
{
	LCD_clrscr();
}

// LCD

void OutputStartLCDMessage()
{
	unsigned char mes_line1[] = "task 2";

	LCD_print(mes_line1);
}

// UART

void Serial_ISR(void) interrupt 4 using 0 // обработчик прерываний от UART
{
	if (RI)
	{

		scanf("%bu", &task);
		switch (task)
		{
		case 1:
			printf("task 1");
			ExecuteTask1();
			break;
		}
		printf("task number");
		printf("%c", task);
	}
}

void main()
{

	P0 = 0xFF; // Иниц LED, LCD, SPI, I2C, прерывания разрешены
	P1 = 0xFF;
	P2 = 0xFF;
	P3 = 0xFF;
	EA = 1;
	init_timer();
	SPI_init();
	I2C_init();
	LCD_init();

	printf("Task_2"); 
	while (1)
	{

	}
}
