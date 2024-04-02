#ifndef _AT24CXX_H_
#define _AT24CXX_H_

#include "PINDEF.H"

// ������� ������ �������� ������ ������ EEPROM AT24CXX
void AT24CXX_write(unsigned char addr, unsigned char value);

// ������� ������ �������� ������ ������ EEPROM AT24CXX
unsigned char AT24CXX_read(unsigned char addr);

#endif
