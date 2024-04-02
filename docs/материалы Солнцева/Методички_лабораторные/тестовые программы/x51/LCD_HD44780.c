#include <intrins.h>
#include "LCD_HD44780.h"
#include "SHIFT.h"
#include "DELAY.h"

void LCD_write(unsigned char w_byte, unsigned char rs)
{
	if(rs == 0)
		LCD_RS = 0;				// ����� ������ ������
	else
		LCD_RS = 1;				// ����� ������ ������
	
	LCD_E = 1;					// ���������� ����� ��� ������ ������
	
	SHIFT_write(w_byte);		// ������� ������ � SPI ����

	_nop_();					// ���� ����
	LCD_E = 0;					// ���������� �����

	DELAY_MCS(500);				// ���� ���� ������ ����� ����������
}                          	

void LCD_init()
{
	LCD_E = 0;
	LCD_RS = 0;
	DELAY_MCS(41000);			// ����������� ����� ����� ������ ������� �� LCD
	
	LCD_write(0x30, 0);		// ����������� �������� ��� ������������� LCD
	DELAY_MCS(5000);
	LCD_write(0x30, 0);
	DELAY_MCS(700);
	LCD_write(0x30, 0);
	DELAY_MCS(700);
	LCD_write(0x38, 0);		// ������������� LCD: 2 ������, ������� 5*7, 8-������ ���������
	LCD_write(0x01, 0);		// ������� �����
	DELAY_MCS(3000);
	LCD_write(0x06, 0);     // ������ ����������� �������� ������� - �������
	LCD_write(0x0C, 0);		// �������� �������, ��������� ������, ��������� ��������
}

void LCD_clrscr()			// ��� ������� ������� �����
{
	LCD_write(0x01, 0);
	DELAY_MCS(5000);
}

void LCD_gotoxy(unsigned char x, unsigned char y)	// ��� ������� ���������� ������ �� LCD � ����� (x, y)
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

void LCD_print( unsigned char string[] )				// ��� ������� ������� ������ �� LCD �������
{																	// ���� count < 0, �� ��������� ��� ������
	while((*string) != 0)
		LCD_printchar(*(string++));
}
