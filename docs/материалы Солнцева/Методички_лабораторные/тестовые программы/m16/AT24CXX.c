#include "AT24CXX.h"
#include "I2C.h"
#include "DELAY.H"

// ������� ������ �������� ������ ������ EEPROM AT24CXX
void AT24CXX_write(unsigned char addr, unsigned char value)
{
	I2C_start();
	I2C_write(0xA0);			// ������� ������
	I2C_write(addr);			// ��������� ������
	I2C_write(value);         	// ������ ������
	I2C_stop();
	DELAY_MS( 15 );				// ���� ��������� ������
}

// ������� ������ �������� ������ ������ EEPROM AT24CXX
unsigned char AT24CXX_read(unsigned char addr)
{
	unsigned char value;
	I2C_start();
	I2C_write(0xA0);	    	// ������� ������
	I2C_write(addr);   	 		// ��������� ������
	I2C_stop();
	DELAY_MCS(68);
	I2C_start(); 
	I2C_write(0xA1);         	// ������� ������
	value = I2C_read();
	I2C_stop();
	
	return value;
}
