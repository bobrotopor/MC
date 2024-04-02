#ifndef _I2C_H_
#define _I2C_H_

#include "PINDEF.H"

void I2C_init();
void I2C_start();
void I2C_stop();
unsigned char I2C_write(unsigned char value);
unsigned char I2C_read();

#endif 
