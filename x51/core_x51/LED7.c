#include "LED7.h"
#include "SHIFT.h"

// Коды цифр для 7-сегментного индикатора (при условии что первым передается MSB слова данных)
#define CODE_0	0xC0
#define CODE_1	0xF9
#define CODE_2	0xA4
#define CODE_3	0xB0
#define CODE_4	0x99
#define CODE_5	0x92
#define CODE_6	0x82
#define CODE_7	0xf8
#define CODE_8	0x80
#define CODE_9	0x90

void LED7_setdigit(unsigned char digit)
{
	switch(digit) {
	case 0:
		SHIFT_write(CODE_0);
		break;
	case 1:
		SHIFT_write(CODE_1);
		break;
	case 2:
		SHIFT_write(CODE_2);
		break;
	case 3:
		SHIFT_write(CODE_3);
		break;
	case 4:
		SHIFT_write(CODE_4);
		break;
	case 5:
		SHIFT_write(CODE_5);
		break;
	case 6:
		SHIFT_write(CODE_6);
		break;
	case 7:
		SHIFT_write(CODE_7);
		break;
	case 8:
		SHIFT_write(CODE_8);
		break;
	case 9:
		SHIFT_write(CODE_9);
		break;
	}
}
