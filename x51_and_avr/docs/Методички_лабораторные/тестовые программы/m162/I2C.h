#ifndef _I2C_H_
#define _I2C_H_

#include "PINDEF.H"

// Инициализация шины I2C
void I2C_init();

// Функция начала передачи данных
void I2C_start();

// Функция окончания передачи данных
void I2C_stop();

// Функция передачи байта по шине
unsigned char I2C_write(unsigned char value);

// Функция чтения байта
unsigned char I2C_read();

#endif
