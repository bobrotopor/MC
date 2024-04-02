#include "Convert.h"
#include <string.h>

/*unsigned char StringToByte( char* str )
{
	unsigned char value = 0, coeff = 1;
	int len = strlen( str );
	
	for( signed char i = len - 1; i >= 0 && i >= len - 3; i-- )
	{
		value += ( str[ i ] - '0' ) * coeff;
		coeff *= 10;
	}
	
	return value;
}

unsigned char ByteToString( unsigned char value, char str[] )
{
	unsigned char nIndex = 0, digit, startFlag = 0;

	if( value != 0 )
		for( unsigned char d = 100; d > 0; d /= 10 )
		{
			if( startFlag || value >= d )
			{
				digit = value / d;
				str[ nIndex++ ] = digit + '0';
				value -= digit * d;
				startFlag = 1;
			}
		}
	else
		str[ nIndex++ ] = '0';
			
	str[ nIndex ] = 0;
	
	return nIndex;
}
*/

unsigned char ShortToString( signed short value, char str[] )
{
	unsigned char nIndex = 0, digit, startFlag = 0;
	signed short d;

	if( value < 0 )
	{
		str[ nIndex++ ] = '-';
		value *= -1;
	}

	if( value != 0 )
		for( d = 10000; d > 0; d /= 10 )
		{
			if( startFlag || value >= d )
			{
				digit = value / d;
				str[ nIndex++ ] = digit + '0';
				value -= digit * d;
				startFlag = 1;
			}
		}
	else
		str[ nIndex++ ] = '0';
			
	str[ nIndex ] = 0;
	
	return nIndex;
}
