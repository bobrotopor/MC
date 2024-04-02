#include "BEEP.H"
#include "DELAY.H"

// Функция включения зуммера на определенное время
void Beep( unsigned int ms )
{
    BEEP_DDR = 1;
	BEEP_PORT = 0;
	while( ms-- > 0 )
		DELAY_MS( 1 );
    BEEP_PORT = 1;
}

