#include "SPI.h"

void SPI_init()
{
	SPCR = 0x5C;			// �������� SPI: ����� �������, ������� �������� ��������
							// �� �������� ���� � ��������
}

unsigned char SPI_rw(const unsigned char w_data)
{
	SPDR = w_data;			//	��������� SPI �����
	while(!(SPSR & 0x80));	// ���� ���� �������� ������ ����� ���������
	return SPDR;
}


