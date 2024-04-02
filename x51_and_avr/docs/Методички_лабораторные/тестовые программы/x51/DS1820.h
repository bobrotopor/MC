/*#ifndef _DS1820_H_
#define _DS1820_H_

#include "PINDEF.H"

unsigned char DS1820_start();


bit DS1820_readbit();
void DS1820_writebit(bit bitData);
unsigned char DS1820_readbyte();
void DS1820_writebyte(unsigned char out);
bit DS1820_readtemperature(unsigned char * buff);
bit DS1820_gettemperature( float* pTemp );

#endif	    */


#ifndef _DS1820_H_
#define _DS1820_H_

#include "PINDEF.H"

// ������� ������
// ���������� 1 ���� �� ���� 1-Wire ������������ ����������
// 			  0 ���� ��������� ���
unsigned char DS1820_reset();

// ������� ������/������ ����� ����� 1-Wire
// ��� ������ ���������� �������� � �������� ��������� 0xFF
unsigned char DS1820_rw( unsigned char tdata );

// ������� ������ �������������� �����������
unsigned char DS1820_startconverttemp();

// ������� �������� ������� �������������� �����������
unsigned char DS1820_getconvertstatus();

// ������� ������ ������ �� ������ ���������� DS1820
// ������ ������ ������ ���� ����� 9
unsigned char DS1820_readdata(unsigned char * buff);

// ������� ��������� �������� �����������
// ���������� 0 ���� ������ �� ���������
// ���������� 1 ���� ������ �� ������(�������������� �� �����������)
// ���������� 2 ���� �������� �������
unsigned char DS1820_gettemperature( char strTemp[] );

#endif

