#include "DELAY.H"

// ��������������� ������� ��� ���������� ��������
void delay_func_mcs( unsigned short count)
{
	while( count-- > 0 );
}

// ��������������� ������� ��� ���������� ��������
void delay_func_ms( unsigned short count)
{
	while( count-- > 0 )
		DELAY_MCS( 100 );
}
