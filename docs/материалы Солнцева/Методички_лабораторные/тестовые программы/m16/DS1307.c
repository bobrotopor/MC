#include "DS1307.h"
#include "I2C.h"
#include "DELAY.h"

unsigned char Clock_Init[] = {0x00, 0x00, 0x23, 0x02, 0x19, 0x04, 0x05};

// Функция записи значения ячейки микросхемы DS1307
void DS1307_write(unsigned char addr, unsigned char value)
{
	I2C_start();
	I2C_write(0xd0);		// Команда записи
	I2C_write(addr);		// Установка адреса
	I2C_write(value);		// Запись значения
	I2C_stop();
}

// Функция чтения значения ячейки микросхемы DS1307
unsigned char DS1307_read(unsigned char addr)
{
	unsigned char value;  
	I2C_start();
	I2C_write(0xd0);    	// Команда записи
	I2C_write(addr);       	// Установка адреса
	I2C_stop();

	DELAY_MCS(68);

	I2C_start(); 
	I2C_write(0xd1);         // Команда чтения
	value = I2C_read();	
	I2C_stop();

	return value;
}

// Функция получения значения времени
// Длина строки timestr должна быть равна 9
void DS1307_gettime(char* timestr)
{
	unsigned char value;

	value = DS1307_read(2);					// Читаем часы
	timestr[0] = (value >> 4) + '0';
	timestr[1] = (value & 0x0F) + '0';
	timestr[2] = ':';

	value = DS1307_read(1);					// Читаем минуты
	timestr[3] = (value >> 4) + '0';
	timestr[4] = (value & 0x0F) + '0';
	timestr[5] = ':';

	value = DS1307_read(0);					// Читаем секунды
	if(value & 0x80)
		DS1307_write(0, 0);					// Чтобы не зависали часы	
	timestr[6] = (value >> 4) + '0';
	timestr[7] = (value & 0x0F) + '0';
	timestr[8] = 0;
}

// Фукция преобразования числа в BCD-формат, когда на один десятичный разряд 
// отводится 4 бита
unsigned char ByteToBCD( unsigned char value )
{
	return ( ( value / 10 ) << 4 ) + ( value % 10 );
}

// Функция задания значения времени
void DS1307_settime( unsigned char hour, unsigned char minute, unsigned char second )
{
	DS1307_write( 2, ByteToBCD( hour ) );
	DS1307_write( 1, ByteToBCD( minute ) );
	DS1307_write( 0, ByteToBCD( second ) );
}

// Функция получения значения даты
// Длина строки datestr должна быть равна 9
void DS1307_getdate(char* datestr)
{
	unsigned char value;

	value = DS1307_read(4);					// Читаем день(число)
	datestr[0] = (value >> 4) + '0';
	datestr[1] = (value & 0x0F) + '0';
	datestr[2] = '.';

	value = DS1307_read(5);					// Читаем месяц
	datestr[3] = (value >> 4) + '0';
	datestr[4] = (value & 0x0F) + '0';
	datestr[5] = '.';

	value = DS1307_read(6);					// Читаем год
	datestr[6] = (value >> 4) + '0';
	datestr[7] = (value & 0x0F) + '0';
	datestr[8] = 0;
}

// Функция задания значения даты
void DS1307_setdate( unsigned char day, unsigned char month, unsigned char year )
{
	DS1307_write( 6, ByteToBCD( year ) );
	DS1307_write( 5, ByteToBCD( month ) );
	DS1307_write( 4, ByteToBCD( day ) );
}
