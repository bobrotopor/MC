#include "SHIFT.h"
#include "SPI.h"

// ������� ������ ����� � ��������� ������� 74595
void SHIFT_write(const unsigned char w_data)
{
	SPI_rw(w_data);
	ST_DDR = 1;
	ST_PORT = 1;
	ST_PORT = 0;
}
