#include "AT24CXX.h"
#include "I2C.h"
#include "DELAY.H"

// Функция записи значения ячейки памяти EEPROM AT24CXX
void AT24CXX_write(unsigned char addr, unsigned char value)
{
	I2C_start();
	I2C_write(0xA0);			// Команда записи
	I2C_write(addr);			// Установка адреса
	I2C_write(value);         	// Запись данных
	I2C_stop();
	DELAY_MS( 15 );				// Ждем окончания записи
}

// Функция чтения значения ячейки памяти EEPROM AT24CXX
unsigned char AT24CXX_read(unsigned char addr)
{
	unsigned char value;
	I2C_start();
	I2C_write(0xA0);	    	// Команда записи
	I2C_write(addr);   	 		// Установка адреса
	I2C_stop();
	DELAY_MCS(68);
	I2C_start(); 
	I2C_write(0xA1);         	// Уоманда чтения
	value = I2C_read();
	I2C_stop();
	
	return value;
}
