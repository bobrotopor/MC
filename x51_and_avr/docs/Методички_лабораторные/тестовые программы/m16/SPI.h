#ifndef _SPI_H_
#define _SPI_H_

#include "PINDEF.H"

// Функция инициализации SPI-интерфейса
void SPI_init();

// Функция записи/чтения данных
unsigned char SPI_rw(const unsigned char w_data);

#endif
