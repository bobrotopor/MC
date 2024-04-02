#ifndef _UART_H_
#define _UART_H_

#include "COMMON.H"
#include "PINDEF.H"

// ������� ������������� UART
void UART_init( unsigned short UBRR_value );

// ������� ������ ����� � ���� UART
void UART_sendbyte( unsigned char data );

// ������� ������ ������ �� ������
void UART_sendstring( char str[] );

// ������� ������ ������ �� flash
void UART_sendstring_flash( flash char str[] );

// ������� ������ ��������� ��������
void UART_sendvalue( unsigned char value );

// ������� ������ �������(��) �������� �������
void UART_sendcrlf();

// ������� ��������� �����
unsigned char UART_receivebyte();

// ������� ��������� �����
void UART_receiveline( char* pStr, unsigned char maxsize );

// ������� ��������� ��������� ��������
unsigned char UART_receivevalue();

// ������ ����������� ��������� �������� ������� UBRR � ����������� �� 
// ����������� �������� ��������
#define CALC_UBRR( baudrate ) ( ( 62500.0 * CLOCK_FREQUENCY - baudrate ) / baudrate )

#endif
