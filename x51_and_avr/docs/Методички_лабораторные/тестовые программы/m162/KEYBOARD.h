#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include "PINDEF.H"

// Коды клавиш телефонной клавиатуры
#define KEY_1			0x11
#define KEY_2			0x21
#define KEY_3			0x41
#define KEY_4			0x12
#define KEY_5			0x22
#define KEY_6			0x42
#define KEY_7			0x14
#define KEY_8			0x24
#define KEY_9			0x44
#define KEY_0			0x28
#define KEY_A			0x81
#define KEY_B			0x82
#define KEY_C			0x84
#define KEY_D			0x88
#define KEY_ASTERISK	0x18
#define KEY_BAR			0x48

// Функция получения кода нажатой клавиши
unsigned char KEY_getkey();

// Функция ожидания отпескания клавиши
void KEY_waitforkeyrelease();

// Функция получения названия клавиши
char KEY_getkeyname( unsigned char keycode );

#endif
