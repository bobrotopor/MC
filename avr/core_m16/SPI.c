#include "SPI.h"

// Функция инициализации SPI-интерфейса
void SPI_init()
{
	SPI_MOSI_DDR = 1;		// MOSI настроен как выход
	SPI_MISO_DDR = 0;		// MISO настроен как вход
	SPI_SCK_DDR = 1;		// SCK настроен как выход
	SPI_SS_DDR = 1;			// SS настроен как выход
	SPI_SS = 1;

	SPCR = 0x5C;			// Включаем SPI: режим ведущий, порядок загрузки регистра
							// от старшего бита к младшему
}

// Функция записи/чтения данных
unsigned char SPI_rw(const unsigned char w_data)
{
	SPDR = w_data;			//	Заполняем SPI буфер
	while(!(SPSR & 0x80));	// Ждем пока передача данных будет завершена
	return SPDR;
}


