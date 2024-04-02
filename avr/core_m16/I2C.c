#include "I2C.h"
#include "DELAY.h"

// Инициализация шины I2C
void I2C_init()
{
	SDA_DDR = 0;
	SCL_DDR = 0;
	
	SDA_PORT = 0;
	SCL_PORT = 0;
}

// Функция начала передачи данных
void I2C_start()
{
	SCL_DDR = 0;
	SDA_DDR = 0;
	DELAY_MCS( 20 );
	SDA_DDR = 1;
	DELAY_MCS( 20 );	
	SCL_DDR = 1;
}

// Функция окончания передачи данных
void I2C_stop()
{
	SDA_DDR = 1;
	DELAY_MCS( 20 );
	SCL_DDR = 0;
	DELAY_MCS( 20 );
	SDA_DDR = 0;
}

// Функция передачи байта по шине
unsigned char I2C_write(unsigned char value)
{
	unsigned char i = 8;
	while( i-- )
	{
		if( value & 0x80 )
			SDA_DDR = 0;
		else
			SDA_DDR = 1;
		value = value << 1;
		SCL_DDR = 0;
		DELAY_MCS( 20 );
		SCL_DDR = 1;
		DELAY_MCS( 20 );
	}
	SDA_DDR = 0;
	SCL_DDR = 0;
	DELAY_MCS( 20 );
	if( SDA_PIN )
		value = 255;
	else
		value= 0;
	DELAY_MCS( 20 );
	SCL_DDR = 1;
	return value;
}

// Функция чтения байта
unsigned char I2C_read()
{
	unsigned char value = 0, i = 8;
	SDA_DDR = 0;
	while( i-- )
	{
		SCL_DDR = 0;
		DELAY_MCS( 20 );
		value = value << 1;
		if( SDA_PIN )
			value |= 1;

		DELAY_MCS( 20 );
		SCL_DDR = 1;
		DELAY_MCS( 20 );
	}
	SCL_DDR = 0;
	DELAY_MCS( 20 );
	SCL_DDR = 1;
	
	return value;
}
