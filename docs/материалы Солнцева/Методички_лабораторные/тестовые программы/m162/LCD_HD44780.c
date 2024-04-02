#include "LCD_HD44780.h"
#include "SHIFT.h"
#include "DELAY.h"

// ������� ������ ����� � LCD
// ���� rs == 0 - ����� ������ �������
// ���� rs == 1 - ����� ������ ������
void LCD_write(unsigned char w_byte, unsigned char rs)
{
	if(rs == 0)
		LCD_RS_PORT = 0;	// ����� ������ ������
	else
		LCD_RS_PORT = 1;	// ����� ������ ������
	
	LCD_E_PORT = 1;			// ���������� ����� ��� ������ ������
	
	SHIFT_write(w_byte);	// ������� ������ � SPI ����

	__no_operation();		// ���� ����
	LCD_E_PORT = 0;			// ���������� �����

	DELAY_MCS(500);			// ���� ���� ������ ����� ����������
}                          	

// ������� ������������� LCD
void LCD_init()
{
	LCD_E_DDR = 1;
	LCD_RS_DDR = 1;
	
	LCD_E_PORT = 0;
	LCD_RS_PORT = 0;
	DELAY_MS(41);			// ����������� ����� ����� ������ ������� �� LCD
	
	LCD_write(0x30, 0);		// ����������� �������� ��� ������������� LCD
	DELAY_MS(5);
	LCD_write(0x30, 0);
	DELAY_MCS(700);
	LCD_write(0x30, 0);
	DELAY_MCS(700);
	LCD_write(0x38, 0);		// ������������� LCD: 2 ������, ������� 5*7, 8-������ ���������
	LCD_write(0x01, 0);		// ������� �����
	DELAY_MS(3);
	LCD_write(0x06, 0);     // ������ ����������� �������� ������� - �������
	LCD_write(0x0C, 0);		// �������� �������, ��������� ������, ��������� ��������
}

// ������� ������� ������ LCD
void LCD_clrscr()
{
	LCD_write(0x01, 0);
	DELAY_MS(5);
}

// ������� ��������� ������� �������
void LCD_gotoxy(unsigned char x, unsigned char y)
{
	if(y != 0)
		x += 0x40;
	x |= 0x80;													
	LCD_write(x, 0);
}

// ������ ������ ������� �� �����
void LCD_printchar( char ch )
{
	LCD_write( ch, 1 );
}

// ������� ������ ������ �� ������
void LCD_print( char string[] )				
{									
	while((*string) != 0)
		LCD_printchar(*(string++));
}

// ������� ������ ������ �� flash
void LCD_print_flash(flash unsigned char string[])
{
		while((*string) != 0)
		LCD_printchar(*(string++));
}
