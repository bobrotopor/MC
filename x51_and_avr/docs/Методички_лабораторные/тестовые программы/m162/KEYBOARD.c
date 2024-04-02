#include "KEYBOARD.h"
#include "DELAY.H"

// Функция получения кода нажатой клавиши
// Сканируем клавиатуру по линиям: выводим сигнал в одну линию, проверяем 
//								   статус, во вторую, и так далее
unsigned char KEY_getkey()
{
	KEY_DDR = 0x0F;
	
	KEY_PORT = 240;	
	DELAY_MCS(40);
 	if (KEY_PIN == 240) 
		return 0; 
	KEY_PORT = 254;
	DELAY_MCS(40);
 	if (KEY_PIN != 254) 
		return ~KEY_PIN; 
	KEY_PORT = 253;
	DELAY_MCS(40);
 	if (KEY_PIN != 253) 
		return ~KEY_PIN;
	KEY_PORT = 251;
	DELAY_MCS(40);
 	if (KEY_PIN != 251) 
		return ~KEY_PIN; 
	KEY_PORT = 247;
	DELAY_MCS(40);
	return ~KEY_PIN;
}

// Функция ожидания отпескания клавиши
void KEY_waitforkeyrelease()
{
	while( KEY_getkey() );
}

// Функция преобразования кода клавиши в номер
char KEY_getkeyname( unsigned char keycode )
{
	switch( keycode ) {
	case KEY_0:
		return '0';
	case KEY_1:
		return '1';
	case KEY_2:
		return '2';
	case KEY_3:
		return '3';
	case KEY_4:
		return '4';
	case KEY_5:
		return '5';
	case KEY_6:
		return '6';
	case KEY_7:
		return '7';
	case KEY_8:
		return '8';
	case KEY_9:
		return '9';
	case KEY_A:
		return 'A';
	case KEY_B:
		return 'B';
	case KEY_C:
		return 'C';
	case KEY_D:
		return 'D';
	case KEY_ASTERISK:
		return '*';
	case KEY_BAR:
		return '#';
	default:
		return 0;
	}
}

