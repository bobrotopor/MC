#include "SPI.h"

// ������� ������������� SPI-����������
void SPI_init()
{
	SPI_MOSI_DDR = 1;		// MOSI �������� ��� �����
	SPI_MISO_DDR = 0;		// MISO �������� ��� ����
	SPI_SCK_DDR = 1;		// SCK �������� ��� �����
	SPI_SS_DDR = 1;			// SS �������� ��� �����
	SPI_SS = 1;

	SPCR = 0x5C;			// �������� SPI: ����� �������, ������� �������� ��������
							// �� �������� ���� � ��������
}

// ������� ������/������ ������
unsigned char SPI_rw(const unsigned char w_data)
{
	SPDR = w_data;			//	��������� SPI �����
	while(!(SPSR & 0x80));	// ���� ���� �������� ������ ����� ���������
	return SPDR;
}


