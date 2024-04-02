#include "BEEP.H"
#include "DELAY.H"
#include "SPI.H"
#include "LCD_HD44780.H"
#include "I2C.H"
#include "KEYBOARD.H"
#include "LED7.H"
#include "UART.H"
#include "DS1307.H"
#include "DS1820.H"
#include "AT24CXX.H"
#include "CONVERT.H"

// ��������� ���������, ������� ��������� � flash-������, ����� ��������
// ������������ ����������� ������

flash char UartMessage[] = "                -=  STEND RK-10  =-\r\n"
						   "       Hardware ver. 1.0, Firmware ver. 2.0\r\n";

flash unsigned char LcdMessageLine1[] = { 45, 61, 67, 84, 69, 72, 224, 32, 32, 80, 75, 45, 49, 48, 61, 45, 0 };
flash unsigned char LcdMessageLine2[] = "   Ver. 2.0";

flash char timeStr[] = "Time: ";
flash char dateStr[] = "Date: ";
flash char enterAddressStr[] = "Enter address(0..255): ";
flash char enterValueStr[] = "Enter value(0..255): ";
flash char valueStr[] = "Value: ";
flash char temperatureStr[] = "Temperature: ";

flash char enterHourStr[] = "Enter hour: ";
flash char enterMinuteStr[] = "Enter minute: ";
flash char enterSecondStr[] = "Enter second: ";
flash char enterDayStr[] = "Enter day: ";
flash char enterMonthStr[] = "Enter month: ";
flash char enterYearStr[] = "Enter year: ";
flash char sensorNotConnectedStr[] = "Sensor is not connected";

flash char enterTaskNumberStr[] = "Enter task number: ";

// ������� 1: ������������ �������� �� ������(�����) ���������� ���������� � 
//			  ��������������� ����� ���������� �� LCD-������
//			  ��� ������� * ����� ���������
//			  ��� ������� # �������������� ����� �� �������
void ExecuteTask1()
{
	unsigned char key = 0;
	char keyname;	
	unsigned char keyCount = 0;

	LCD_clrscr();
	while( key != KEY_BAR )
	{
		key = KEY_getkey();

		if( !key )
			continue;

		keyname = KEY_getkeyname( key );
	
		if( keyname >= '0' && keyname  <= '9' )
		{	
			LCD_printchar( keyname );
			keyCount++;

			if( keyCount == LCD_WIDTH )
				LCD_gotoxy( 0, 1 );
		}
		else if( key == KEY_ASTERISK )
		{
			LCD_clrscr();
			keyCount = 0;
		}
	
		KEY_waitforkeyrelease();
	}
}

// ������� 2: ������������ �������� �� ������(�����) ���������� ���������� � 
//			  ��������������� ����� ���������� �� �������������� ����������
//			  ��� ������� # �������������� ����� �� �������
void ExecuteTask2()
{
	unsigned char key = 0;
	
	while( key != KEY_BAR )
	{
		key = KEY_getkey();

		if( !key )
			continue;

		LED7_setdigit( KEY_getkeyname( key ) - '0' );
		KEY_waitforkeyrelease();
	}
}

// ������� 3: ��� ������� �� ������ ���������� ���������� ��������� 
//			  ������ �������
//			  ��� ������� # �������������� ����� �� �������
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

// ������� 4: ��� ������� �� ������ ���������� ����������(1,2,3) ������ 
//			  ��������� ��������������� ���� ������ LED-���������
//			  ��� ������� # �������������� ����� �� �������
void ExecuteTask4()
{
	unsigned char key = 0;
	
	while( key != KEY_BAR )
	{
		key = KEY_getkey();

		if( !key )
			continue;

		switch( KEY_getkeyname( key ) ) {
		case '1':
			LED1_PORT = !LED1_PORT;
			break;
		case '2':
			LED2_PORT = !LED2_PORT;
			break;
		case '3':
			LED3_PORT = !LED3_PORT;
			break;
		}
		KEY_waitforkeyrelease();
	}
}

// ������� 5: � UART ��������� ������� ����� � ����, ���������� ��
// 			  ���������� DS1307
void ExecuteTask5()
{
	char time[9], date[9];
	DS1307_gettime(time);
	DS1307_getdate(date);

	UART_sendstring_flash( timeStr );
	UART_sendstring( time );
	UART_sendcrlf();
	UART_sendstring_flash( dateStr );
	UART_sendstring( date );
	UART_sendcrlf();
}

// ������� 6: ����� UART-��������� ������������ ������ ����� ����� � ����,
// 			  ������� ��������������� � ���������� DS1307
void ExecuteTask6()
{
	unsigned char hour, minute, second;
	unsigned char day, month, year;
	
	UART_sendstring_flash( enterHourStr );
	hour = UART_receivevalue();	
	UART_sendstring_flash( enterMinuteStr );
	minute = UART_receivevalue();	
	UART_sendstring_flash( enterSecondStr );
	second = UART_receivevalue();	
	
	DS1307_settime( hour, minute, second );

	UART_sendstring_flash( enterDayStr );
	day = UART_receivevalue();	
	UART_sendstring_flash( enterMonthStr );
	month = UART_receivevalue();	
	UART_sendstring_flash( enterYearStr );
	year = UART_receivevalue();	
	
	DS1307_setdate( day, month, year );
}

// ������� 7: ����� UART-��������� ������������ ������ ����� � �������� ������,
//			  ������� ��������� �� ������� EEPROM ������ AT24CXX
void ExecuteTask7()
{
	unsigned char addr, value;

	UART_sendstring_flash( enterAddressStr );
	addr = UART_receivevalue();
	UART_sendstring_flash( enterValueStr );
	value = UART_receivevalue();
	
	AT24CXX_write( addr, value );
}

// ������� 8: ����� UART-��������� ������������ ������ ����� ������,
//			  �������� ������� ����������� �� ������� EEPROM ������ AT24CXX
//			  � ���������� � UART
void ExecuteTask8()
{
	unsigned char addr;
	
	UART_sendstring_flash( enterAddressStr );
	addr = UART_receivevalue();

	UART_sendstring_flash( valueStr );
	UART_sendvalue( AT24CXX_read( addr ) );
	UART_sendcrlf();	
}

// ������� 9: � UART-��������� ���������� ������� �������� �����������,
//			  ���������� �� ���������� DS1820
void ExecuteTask9()
{
	char strTemp[ 5 ];

	if( !DS1820_startconverttemp() )
	{
		UART_sendstring_flash( sensorNotConnectedStr );
		UART_sendcrlf();
		return;
	}

	while( !DS1820_getconvertstatus() );
		
	if( DS1820_gettemperature( strTemp ) )
	{
		UART_sendstring_flash( temperatureStr );
		UART_sendstring( strTemp );
		UART_sendcrlf();
	}
}

// ������� ������� �� LCD �������������� ���������
void OutputStartLCDMessage()
{
	LCD_print_flash( LcdMessageLine1 );
	LCD_gotoxy( 0, 1 );
	LCD_print_flash( LcdMessageLine2 );
}

// �������-���������� ����������, ������������ ����� �� ������� ������ UART
// ������� ������. 
// � ���� ������� �� �������� ����� ������� � ��������� ���.

#pragma vector = UART_RXC_vect
 __interrupt void UART0_RX_interrupt()
{
	unsigned char task;
	task = UART_receivevalue();
	
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
	UART_sendstring_flash( enterTaskNumberStr );
}

// ������� ����������� ������, �� ���� ������, ����� ��� ����������� � 
// ���������� ����� UART-���������.
// �� LCD-����� ���������:
// 		� ������ ������ - ������� ����������� � ������� �����
//		�� ������ ������ - ������� ����
// LED ���������� ������ ���� ���������
void AutonomousMode()
{
	LCD_clrscr();
	
	static char strTemp[ 9 ] = { 0 };
	LCD_gotoxy( 0, 0 );

	switch( DS1820_gettemperature( strTemp ) ) {	// �������� ����������� 
	case 0:											// ������ �� ���������
		strTemp[ 0 ] = 0;
		break;
	case 2:											// ������ �� ������
		DS1820_startconverttemp();
		break;
	}
	LCD_print( strTemp );

	char time[9] = { 0 };
	DS1307_gettime(time);
	
	LCD_gotoxy( 6, 0 );
	LCD_print( time );

	
	LCD_gotoxy( 0, 1 );
	char keyname = KEY_getkeyname( KEY_getkey() );
	if( keyname != 0 )
		LCD_printchar( keyname );
	
	
	char date[9] = { 0 };
	DS1307_getdate(date);

	LCD_gotoxy( 6, 1 );
	LCD_print( date );
	
	static unsigned char ledCount = 0;
	if( ledCount % 3 == 0 )							// ����� ���������� �������
	{												// ����� �� ������, ������
		LED1_PORT = !LED1_PORT;						// �� ��������� ���� ��� ��
		LED2_PORT = !LED2_PORT;						// ��� ������ �������
		LED3_PORT = !LED3_PORT;
	}
	ledCount++;
}

// ������� ������� ���������
// ������������ ������������� ���������, ����� ��������� �� LCD-�����, � �����
// ����� ��������� � UART-���������
// ����� ���������� ������ �� 0.5 �
// � ����������� ����� ����������� ������� ����������� ������
int main()
{
	LED1_DDR = 1;
	LED2_DDR = 1;
	LED3_DDR = 1;
	
	I2C_init();
	SPI_init();
	LCD_init();
	LCD_clrscr();

	UART_init( CALC_UBRR( 57600 ) );
	__enable_interrupt();

	DS1820_startconverttemp();
	
	UART_sendstring_flash( UartMessage );
	OutputStartLCDMessage();

	UART_sendstring_flash( enterTaskNumberStr );

	Beep( 500 );

	DELAY_MS( 1000 );

	while(1) 
	{
		AutonomousMode();
		DELAY_MS( 200 );
	}
  
}
