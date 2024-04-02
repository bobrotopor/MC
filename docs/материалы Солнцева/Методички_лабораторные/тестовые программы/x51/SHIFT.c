#include "SHIFT.h"
#include "SPI.h"

void SHIFT_write(const unsigned char w_data)
{
	SPI_rw(w_data);
	ST = 1;
	ST = 0;
}
