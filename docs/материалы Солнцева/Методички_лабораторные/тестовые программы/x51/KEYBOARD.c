#include "KEYBOARD.h"
#include "DELAY.H"

unsigned char KEY_getkey()
{
	KEY_PORT = 240;	
	DELAY_MCS(40);
 	if (KEY_PORT == 240) 
		return 0; 
	KEY_PORT = 254;
	DELAY_MCS(40);
 	if (KEY_PORT != 254) 
		return ~KEY_PORT; 
	KEY_PORT = 253;
	DELAY_MCS(40);
 	if (KEY_PORT != 253) 
		return ~KEY_PORT; 
	KEY_PORT = 251;
	DELAY_MCS(40);
 	if (KEY_PORT != 251) 
		return ~KEY_PORT; 
	KEY_PORT = 247;
	DELAY_MCS(40);
	return ~KEY_PORT;
}

void KEY_waitforkeyrelease()
{
	while( KEY_getkey() );
}

unsigned char KEY_getkeynumber( unsigned char keycode )
{
	switch( keycode ) {
    case KEY_0:
		return 0;
	case KEY_1:
		return 1;
	case KEY_2:
		return 2;
	case KEY_3:
		return 3;
	case KEY_4:
		return 4;
	case KEY_5:
		return 5;
	case KEY_6:
		return 6;
	case KEY_7:
		return 7;
	case KEY_8:
		return 8;
	case KEY_9:
		return 9;
	case KEY_A:
		return 10;
	case KEY_B:
		return 11;
	case KEY_C:
		return 12;
	case KEY_D:
		return 13;
	case KEY_ASTERISK:
		return 14;
	case KEY_BAR:
		return 15;
	default:
		return 16;
	}
}

