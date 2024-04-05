Код задачи №2
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

unsigned char task;

void ExecuteTask1()			//ввод пользователем даты и времени
{
	unsigned char hour, minute, second;
	unsigned char day, month, year;

printf( "Enter hour: ");
	scanf( "%bu", &hour );
printf( "Enter minute: ");
	scanf( "%bu", &minute );
printf( "Enter second: ");
	scanf( "%bu", &second );

	DS1307_settime( hour, minute, second );
	
	printf( "Enter day: ");
	scanf( "%bu", &day );
printf( "Enter month: ");
	scanf( "%bu", &month );
printf( "Enter year: ");
	scanf( "%bu", &year );

	DS1307_setdate( day, month, year ); 
}

void ExecuteTask2()			//вывод даты и времени на экран
{
	char time[9] = { 0 };
	char date[9] = { 0 };
    static char sec=0;

    DS1307_gettime(time);
    if(time[7]==sec)
	  {
	     return;	
	  }
    sec = time[7]; 
		
		DS1307_getdate(date);	
	
		LCD_gotoxy( 0, 0 );
		LCD_print( time );

		LCD_gotoxy( 0, 1 );
		LCD_print( date );	
}

void ExecuteTask3()		//очистка экрана
{
	LCD_clrscr();
}

// LCD 

void OutputStartLCDMessage()
{
	unsigned char mes_line1[] = "task 2";

	LCD_print( mes_line1 );
}


//UART

void Serial_ISR(void) interrupt 4 using 0 		//обработчик прерываний от UART
{
if(RI){
	
		scanf( "%bu", &task );
		switch( task ) {
		case 1:
			printf( "task 1" );
			ExecuteTask1();
			break;
		}
		printf( "task number" );
			printf( "%c", task );
		
		}
}

void main()
{   

    	P0 = 0xFF;				
	P1 = 0xFF;
	P2 = 0xFF;	
	P3 = 0xFF;

	SCON  = 0x50;			
	TMOD  = 0x20;			
	TH1   = 250;			
	TL1   = 250;			
	TR1   = 1;				
	TI    = 1;
	ES=1;                   // разрешение прерываний от UART

	printf ("                -=  STEND RK-10  =-\n");			
	printf ("       Hardware ver. 1.0, Firmware ver. 1.0\n");

	DELAY_MCS( 174 );

	SPI_init();             //иниц SPI, I2C, DS1307, LCD 

	I2C_init();				
	DS1307_init();          
	DS1820_startconverttemp();

	LCD_init();				//  LCD 
	LCD_clrscr();			// LCD 

	OutputStartLCDMessage();

printf( "Enter task number:" );
	

		EA=1;
while(1) 
	{ 
		if(task==2) {
			ExecuteTask2();
		}
		if(task==3){
			ExecuteTask3();
		}
		delay_ms( 5 );
	}
}
