#include "UART.h"
#include "CONVERT.H"

// Константное значение строки перевода каретки
flash char crlf[] = "\r\n";

// Функция инициализации UART
void UART_init( unsigned short UBRR_value )
{
	UART_UBRRL = ( unsigned char ) UBRR_value;
	UART_UBRRH = ( unsigned char ) ( UBRR_value >> 8 );
	
	UART_UCSRB_RXEN = 1;
	UART_UCSRB_TXEN = 1;
	UART_UCSRB_RXCIE = 1;
}

// Функция вывода байт в порт UART
void UART_sendbyte( unsigned char data )
{
	while( !UART_UCSRA_UDRE );
	
	UART_UDR = data;
}

// Функция вывода строки из памяти
void UART_sendstring( char str[] )
{
	unsigned char i = 0;
	while( str[ i ] != 0 )
		UART_sendbyte( str[ i++ ] ); 
}

// Функция вывода строки из flash
void UART_sendstring_flash( flash char str[] )
{
	unsigned char i = 0;
	while( str[ i ] != 0 )
		UART_sendbyte( str[ i++ ] ); 
}

// Функция вывода числового значения
void UART_sendvalue( unsigned char value )
{
	char str[ 4 ];
	ByteToString( value, str );
	UART_sendstring( str );
}

// Функция вывода символа(ов) перевода каретки
void UART_sendcrlf()
{
	UART_sendstring_flash( crlf );
}

// Функция получения байта
unsigned char UART_receivebyte()
{
	while( !UART_UCSRA_RXC );
	
	return UART_UDR;
}

// Функция получения линии
void UART_receiveline( char* pStr, unsigned char maxsize )
{
	unsigned char byte, nIndex = 0;
	unsigned char flag = 0;

	while( !flag )
	{
		byte = UART_receivebyte();
		switch( byte ) {
		case '\r':
		case '\n':
			pStr[ nIndex ] = 0;
			flag = 1;
			UART_sendcrlf();
			break;
		default:
			UART_sendbyte( byte );
			if( nIndex < maxsize - 1 )
				pStr[ nIndex++ ] = byte;
			else
			{
				pStr[ nIndex++ ] = byte;
				pStr[ nIndex ] = 0;
				flag = 1;
			}
			break;
		}
	}
}

// Функция получения числового значения
unsigned char UART_receivevalue()
{
	char line[ 20 ];
	
	UART_receiveline( line, sizeof( line ) );
	
	return StringToByte( line );
}
