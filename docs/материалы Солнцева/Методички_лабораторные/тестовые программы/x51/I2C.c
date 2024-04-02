#include "I2C.h"
#include "DELAY.h"

void I2C_init()
{
	SDA = 1;
	SCL = 1;

//	SDA = 1;
//	SCL = 1;
}

void I2C_start()
{
	SCL = 1;
	SDA = 1;
	DELAY_MCS(20);
	SDA = 0;
	DELAY_MCS(20);
	SCL = 0;
}

void I2C_stop()
{
	SDA = 0;
	DELAY_MCS(20);
	SCL = 1;
	DELAY_MCS(20);
	SDA = 1;	
}

unsigned char I2C_write(unsigned char value)
{
	unsigned char i=8;
	while(i--)
	{
		if (value & 0x80)
			SDA = 1;
		else
			SDA = 0;
		value <<= 1;
		SCL = 1;
		DELAY_MCS(20);
		SCL = 0;
		DELAY_MCS(20);
	}
	SDA = 1;
	SCL = 1;
	DELAY_MCS(20);

	if(SDA)
		value = 255;				// Если не получен acknowledge
	else
		value = 0;

	DELAY_MCS(20);
	SCL = 0;
	return value;
}

unsigned char I2C_read()
{
	unsigned char value = 0, i = 8;

	SDA = 1;
	while(i--)
	{
		SCL = 1;
		DELAY_MCS(20);
		value <<= 1;
		if(SDA)
			value |= 1;
		else 
			value &= ~1;
		DELAY_MCS(20);
		SCL = 0;
		DELAY_MCS(20);
 	}
	SCL = 1;
	DELAY_MCS(20);
	SCL = 0;

	return value;
}		  

