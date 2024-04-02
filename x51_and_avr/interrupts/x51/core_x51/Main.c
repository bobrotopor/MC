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

//������� 1: ��� ������� �� ������(�����) ���������� ����������
//			 �� LCD ����� ��������� �������� ������,
//			 ��� ������� �� ������� ���������� ����� � ������� ����

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

//������� 2: ��� ������� �� ������(�����) ���������� ����������
//		     �� 7-���������� ���������� ��������� �������� ������,
// 			 ��� ������� �� ������� ���������� ����� � ������� ����

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

//������� 3: ��� ������� �� ������ ���������� ���������� ��������� ������,
//			 ��� ������� �� ������� ���������� ����� � ������� ����

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

//������� 4: ��� ������� �� ������(1, 2, 3) ���������� ��� ������
//			 ��������������� �������� ���������,
//		     ��� ������� �� ������� ���������� ����� � ������� ����

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

//������� 5: � ���������������� ���� ���������� ������� �����,
//			 ���������� � ������� DS1307

void ExecuteTask5()
{
    char time[9], date[9];
	DS1307_gettime(time);
	DS1307_getdate(date);

	printf("%s\n",time);
	printf("%s\n",date);
}

//������� 6: � ������������ ��������� ������������ ������ ����� � 
//           ���� � ��� ��������������� ��� ������� � ������ DS1307

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

//������� 7: � ������������ ��������� ������������ ������ ����� � 
//           �������� ������, ����� ������ ������������ � EEPROM AT24CXX

void ExecuteTask7()
{
    unsigned char addr, value;
	printf( "Enter address: ");
	scanf( "%bu", &addr );
	printf( "Enter value(0..255): ");
	scanf( "%bu", &value );

	AT24CXX_write( addr, value );
}

// ������� 8: ����� UART-��������� ������������ ������ ����� ������, �������� 
//            ������� ����������� �� ������� EEPROM ������ AT24CXX � ���������� � UART

void ExecuteTask8()
{
	unsigned char addr;

	printf( "Enter address: ");
	scanf( "%bu", &addr );
  
   	printf( "Value: %bu\n", AT24CXX_read( addr ) );
}

//������� 9: � ���������������� ���� ���������� �����������,
//			 ���������� � ������� DS1820

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

// ����� ����������� �� LCD �����

void OutputStartLCDMessage()
{
	unsigned char mes_line1[] = { 45, 61, 67, 84, 69, 72, 224, 32, 32, 80, 75, 45, 49, 48, 61, 45, 0 };
	unsigned char mes_line2[] = "   Ver. 1.0";

	LCD_print( mes_line1 );
	LCD_gotoxy( 0, 1 );
	LCD_print( mes_line2 );
}

// ����� ���������� �� LCD � ���������� ������

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
 
   	switch( DS1820_gettemperature( strTemp ) ) {	// �������� ����������� 
	case 0:											// ������ �� ���������
		strTemp[ 0 ] = 0;
		break;
	case 2:											// ������ �� ������
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

// ���������� ���������� �� UART

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
    
    P0 = 0xFF;				// ����������� ����� �� ����� 
	P1 = 0xFF;
	P2 = 0xFF;	
	P3 = 0xFF;

	SCON  = 0x50;			// 8-������� UART
	TMOD  = 0x20;			// ������ 1: 8-������� �����, ����-��������������� 
	TH1   = 250;			// ������ ��������� �������� ������� ��� �������� 9600 ���
	TL1   = 250;			
	TR1   = 1;				// ��������� ������
	TI    = 1;
	ES=1;                   // ��������� ���������� �� UART

	printf ("                -=  STEND RK-10  =-\n");			// �������� � UART ��������� 
	printf ("       Hardware ver. 1.0, Firmware ver. 1.0\n");

	DELAY_MCS( 174 );

	SPI_init();             // �������������� SPI ���������

	I2C_init();				// �������������� I2C ���������
	DS1307_init();          // �������������� ���� ��������� �������
	DS1820_startconverttemp();
   
	LCD_init();				// �������������� LCD �������
	LCD_clrscr();			// ������� ����� LCD �������

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
