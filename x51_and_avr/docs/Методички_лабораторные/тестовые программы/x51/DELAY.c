#include "DELAY.H"
#include <intrins.h>

void delay_func_mcs( int count )
{
	int i;
	for ( i = 0; i < count; i++ );
}

void delay_ms( int count )
{
	unsigned char i;
	while (count != 0) 
	{
		count--; 
		for ( i = 0; i < 250; i++);
	}
}
