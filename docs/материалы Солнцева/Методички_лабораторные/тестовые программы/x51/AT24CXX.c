#include "AT24CXX.h"
#include "I2C.h"
#include "DELAY.H"
#include <stdio.h>

void AT24CXX_write(unsigned char addr, unsigned char value)
{
	I2C_start();
	I2C_write(0xA0);
	I2C_write(addr);			// ��������� ������
	I2C_write(value);         	// ������ ������
	I2C_stop();
   
}

unsigned char AT24CXX_read(unsigned char addr)
{
	unsigned char value;
	I2C_start();
	I2C_write(0xA0);	    	// ������� ������   #10100000b
	I2C_write(addr);   	 		// ��������� ������
	I2C_stop();
	DELAY_MCS(68);
	I2C_start(); 
	I2C_write(0xA1);         	// ������ ������
	value = I2C_read();
	I2C_stop();
	
	return value;
}
