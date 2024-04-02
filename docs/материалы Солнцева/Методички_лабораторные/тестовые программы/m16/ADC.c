#include "ADC.H"
#include "DELAY.H"

// Функция получения значения с определенного канала АЦП
unsigned short ADC_get( unsigned char channel )
{
	ADMUX = channel;
	DELAY_MCS( 150 );
	ADCSRA = 210;
	while ( ( ADCSRA & 0x10 ) == 0 );
	return ADC;
}
