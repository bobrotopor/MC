#include "SPI.h"

void SPI_init()
{
	SPCR = 0x5C;			// Включаем SPI: режим ведущий, порядок загрузки регистра
							// от старшего бита к младшему
}

unsigned char SPI_rw(const unsigned char w_data)
{
	SPDR = w_data;			//	Заполняем SPI буфер
	while(!(SPSR & 0x80));	// Ждем пока передача данных будет завершена
	return SPDR;
}


