#ifndef _UART_H_
#define _UART_H_

#include "COMMON.H"
#include "PINDEF.H"

// Функция инициализации UART
void UART_init( unsigned short UBRR_value );

// Функция вывода байта в порт UART
void UART_sendbyte( unsigned char data );

// Функция вывода строки из памяти
void UART_sendstring( char str[] );

// Функция вывода строки из flash
void UART_sendstring_flash( flash char str[] );

// Функция вывода числового значения
void UART_sendvalue( unsigned char value );

// Функция вывода символа(ов) перевода каретки
void UART_sendcrlf();

// Функция получения байта
unsigned char UART_receivebyte();

// Функция получения линии
void UART_receiveline( char* pStr, unsigned char maxsize );

// Функция получения числового значения
unsigned char UART_receivevalue();

// Макрос позволяющий вычислить значение региста UBRR в зависимовти от 
// необходимой скорости передачи
#define CALC_UBRR( baudrate ) ( ( 62500.0 * CLOCK_FREQUENCY - baudrate ) / baudrate )

#endif
