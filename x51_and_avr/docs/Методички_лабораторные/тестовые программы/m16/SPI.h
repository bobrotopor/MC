#ifndef _SPI_H_
#define _SPI_H_

#include "PINDEF.H"

// ������� ������������� SPI-����������
void SPI_init();

// ������� ������/������ ������
unsigned char SPI_rw(const unsigned char w_data);

#endif
