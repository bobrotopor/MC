#include "BEEP.H"
#include "DELAY.H"

// ������� ��������� ������� �� ������������ �����
void Beep( unsigned int ms )
{
    BEEP_DDR = 1;
	BEEP_PORT = 0;
	while( ms-- > 0 )
		DELAY_MS( 1 );
    BEEP_PORT = 1;
}

