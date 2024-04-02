#include "BEEP.H"
#include "DELAY.H"

void Beep( unsigned int ms )
{
	BEEP_BIT = 0;
	delay_ms( ms );
	BEEP_BIT = 1;
}

