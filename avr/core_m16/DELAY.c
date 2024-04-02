#include "DELAY.H"

// Вспомогательная функция для выполнения задержки
void delay_func_mcs( unsigned short count)
{
	while( count-- > 0 );
}

// Вспомогательная функция для выполнения задержки
void delay_func_ms( unsigned short count)
{
	while( count-- > 0 )
		DELAY_MCS( 100 );
}
