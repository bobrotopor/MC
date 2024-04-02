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

//Задание 1: при нажатии на кнопки(цифры) телефонной клавиатуры
//			 на LCD экран выводится значение кнопки,
//			 при нажатии на решетку происходит выход в главное меню

void ExecuteTask1()
{
	unsigned char key = 0;
	unsigned char keynumber;	
	unsigned char keyCount = 0;

	LCD_clrscr();
	while( key != KEY_BAR )
	{
		key = KEY_getkey();

		if( !key )
			continue;

		keynumber = KEY_getkeynumber( key );
	
		if( keynumber <= 9 )
		{	
			LCD_printchar( keynumber + '0' );
			keyCount++;

			if( keyCount == LCD_WIDTH )
				LCD_gotoxy( 0, 1 );
		}
		else if( key == KEY_ASTERISK )
			LCD_clrscr();
	
		KEY_waitforkeyrelease();
		SHIFT_write(0);
	}
}

//Задание 2: при нажатии на кнопки(цифры) телефонной клавиатуры
//		     на 7-сегментном индикаторе выводится значение кнопки,
// 			 при нажатии на решетку происходит выход в главное меню

void ExecuteTask2()
{
	unsigned char key = 0;
	
	LCD_clrscr();
	
	while( key != KEY_BAR )
	{
		key = KEY_getkey();

		if( !key )
			continue;

		LED7_setdigit( KEY_getkeynumber( key ) );
		KEY_waitforkeyrelease();
	}
}

//Задание 3: при нажатии на кнопки телефонной клавиатуры раздается сигнал,
//			 при нажатии на решетку происходит выход в главное меню

void ExecuteTask3()
{
	unsigned char key = 0;
	
	while( key != KEY_BAR )
	{
		key = KEY_getkey();

		if( !key )
			continue;

		Beep( 100 );
		KEY_waitforkeyrelease();
	}
}

//Задание 4: при нажатии на кнопки(1, 2, 3) загорается или гаснет
//			 соответствующий световой индикатор,
//		     при нажатии на решетку происходит выход в главное меню

void ExecuteTask4()
{
	unsigned char key = 0;
	
	while( key != KEY_BAR )
	{
		key = KEY_getkey();

		if( !key )
			continue;

		switch( KEY_getkeynumber( key ) ) {
		case 1:
			LED1 = !LED1;
			break;
		case 2:
			LED2 = !LED2;
			break;
		case 3:
			LED3 = !LED3;
			break;	
		}
		KEY_waitforkeyrelease();
	}
}

//Задание 5: в последовательный порт выводиться текущее время,
//			 полученное с датчика DS1307

void ExecuteTask5()
{
    char time[9], date[9];
	DS1307_gettime(time);
	DS1307_getdate(date);

	printf("%s\n",time);
	printf("%s\n",date);
}

//Задание 6: в терминальной программе пользователь вводит время и 
//           дату и они устанавливаются как текущие в датчик DS1307

void ExecuteTask6()
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

//Задание 7: в терминальной программе пользователь вводит адрес и 
//           значение ячейки, далее данные записываются в EEPROM AT24CXX

void ExecuteTask7()
{
    unsigned char addr, value;
	printf( "Enter address: ");
	scanf( "%bu", &addr );
	printf( "Enter value(0..255): ");
	scanf( "%bu", &value );

	AT24CXX_write( addr, value );
}

// Задание 8: через UART-интерфейс пользователь вводит адрес ячейки, значение 
//            которой считывается из внешней EEPROM памяти AT24CXX и выводиться в UART

void ExecuteTask8()
{
	unsigned char addr;

	printf( "Enter address: ");
	scanf( "%bu", &addr );
  
   	printf( "Value: %bu\n", AT24CXX_read( addr ) );
}

//Задание 9: в последовательный порт выводиться температура,
//			 полученная с датчика DS1820

void ExecuteTask9()
{
    char strTemp[ 5 ];

	if( !DS1820_startconverttemp() )
	{
	    printf("Sensor is not connected\n");	
		return;
	}

	while( !DS1820_getconvertstatus() );
		
	if( DS1820_gettemperature( strTemp ) )
	{
	    printf("%s\n",strTemp);
	}
}

// Вывод приветствия на LCD экран

void OutputStartLCDMessage()
{
	unsigned char mes_line1[] = { 45, 61, 67, 84, 69, 72, 224, 32, 32, 80, 75, 45, 49, 48, 61, 45, 0 };
	unsigned char mes_line2[] = "   Ver. 1.0";

	LCD_print( mes_line1 );
	LCD_gotoxy( 0, 1 );
	LCD_print( mes_line2 );
}

// Вывод информации на LCD в автономном режиме

void AutonomousMode()
{
    static char strTemp[ 9 ] = { 0 };
	char time[9] = { 0 };
	char date[9] = { 0 };
    static char sec=0;
    
    DS1307_gettime(time);
    if(time[7]==sec)
	  {
	     return;	  
	  }
    sec = time[7]; 
 
   	switch( DS1820_gettemperature( strTemp ) ) {	// Получаем температуру 
	case 0:											// Сенсор не подключен
		strTemp[ 0 ] = 0;
		break;
	case 2:											// Данные не готовы
		DS1820_startconverttemp();
		break;
	}
	
	DS1307_getdate(date);

    LCD_clrscr();	
	
	LCD_gotoxy( 0, 0 );
	LCD_print( strTemp );
	
	LCD_gotoxy( 6, 0 );
	LCD_print( time );

	LCD_gotoxy( 6, 1 );
	LCD_print( date );	
					
	LED1 = !LED1;					    
	LED2 = !LED2;					
	LED3 = !LED3;
}

// Обработчик прерываний от UART

void Serial_ISR(void) interrupt 4 using 0 
{
    unsigned char task;

    if(RI){
  	
	scanf( "%bu", &task );

    switch( task ) {
		case 1:
			ExecuteTask1();
			break;
		case 2:
			ExecuteTask2();
			break;
		case 3:
			ExecuteTask3();
			break;
		case 4:
			ExecuteTask4();
			break;
		case 5:
			ExecuteTask5();
			break;
		case 6:
			ExecuteTask6();
			break;
		case 7:
			ExecuteTask7();
			break;
		case 8:
			ExecuteTask8();
			break;
        case 9:
            ExecuteTask9();
            break;	
		}

    printf( "Enter task number:" );}
}


void main()
{   
    
    P0 = 0xFF;				// Настраиваем порты на вывод 
	P1 = 0xFF;
	P2 = 0xFF;	
	P3 = 0xFF;

	SCON  = 0x50;			// 8-битовый UART
	TMOD  = 0x20;			// Таймер 1: 8-битовый режим, авто-перезагружаемый 
	TH1   = 250;			// Задаем начальное значение таймера для скорости 9600 бод
	TL1   = 250;			
	TR1   = 1;				// Запускаем таймер
	TI    = 1;
	ES=1;                   // Разрешаем прерывание от UART

	printf ("                -=  STEND RK-10  =-\n");			// Посылаем в UART сообщение 
	printf ("       Hardware ver. 1.0, Firmware ver. 1.0\n");

	DELAY_MCS( 174 );

	SPI_init();             // Инициализируем SPI интерфейс

	I2C_init();				// Инициализируем I2C интерфейс
	DS1307_init();          // Инициализируем часы реального времени
	DS1820_startconverttemp();
   
	LCD_init();				// Инициализируем LCD дисплей
	LCD_clrscr();			// Очищаем экран LCD дисплея

	OutputStartLCDMessage();

    printf( "Enter task number:" );

    Beep( 300 );

    while(1) 
	{ 
	     char keyname;
	     EA=0;	
	     
		 AutonomousMode();
			 
		 keyname = KEY_getkeynumber( KEY_getkey() );
         if( keyname <= 9 )
            LED7_setdigit( keyname );
		 else
		    SHIFT_write(0);

	     EA=1;

		 delay_ms( 10 );
	}
}   
