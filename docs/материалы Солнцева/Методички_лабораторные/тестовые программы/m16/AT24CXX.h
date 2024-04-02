#ifndef _AT24CXX_H_
#define _AT24CXX_H_

#include "PINDEF.H"

// Функция записи значения ячейки памяти EEPROM AT24CXX
void AT24CXX_write(unsigned char addr, unsigned char value);

// Функция чтения значения ячейки памяти EEPROM AT24CXX
unsigned char AT24CXX_read(unsigned char addr);

#endif
