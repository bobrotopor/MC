#include <stdio.h>
#include <intrins.h>
#include <string.h>
#include "KEYBOARD.H"
#include "SPI.H"
#include "LCD_HD44780.H"
#include "DELAY.H"
#include "LED7.H"
#include "DS1307.H"
#include "DS1820.H"
#include "AT24CXX.H"
#include "BEEP.H"
#include "I2C.H"
#include "SHIFT.H"

/*
Задача №1

1) При запуске программы на экран терминала выводится сообщение «Задача №1».
2) При нажатии на любую кнопку клавиатуры стенда соответствующий символ выводится на ЖКИ стенда 
и семисегментный индикатор. Зуммер дает 3 сигнала длительностью 300 мс с паузой 100 мс, 
после паузы 300 мс – 3 сигнала длительностью 100 мс с паузой 100 мс, после паузы 300 мс - 3 сигнала 
дли-тельностью 300 мс с паузой 100 мс. 
3) Реализовать удаление последнего введенного сим-вола на экране ЖКИ кнопкой “*”, а на 
семисегментном индикаторе при этом должна выводиться цифра, стоящая последней перед удаленной. 

*/

#define CLOCK 22110000
#define TIMER_FREQ_HZ 1000
#define RELOAD_VALUE (65536 - (CLOCK / 12 / TIMER_FREQ_HZ))
#define RELOAD_VALUE_H ((unsigned char)(RELOAD_VALUE >> 8))
#define RELOAD_VALUE_L ((unsigned char)(RELOAD_VALUE))

#define ONESHOT_DELAY 100    // пауза 100 мс между одиночными сигналами
#define PAUSE_DUR 300        // пауза 300 мс между сериями сигналов
#define BUTTONS_ASK_DUR 10   // время опроса нопок клавиатуры

#define FALSE 0
#define TRUE 1

int signal_dur_seq[] = {100, 300, 100}; // последовательность длительности сигналов в серии

char key = 0;                // ключ символа нажатой кнопки клавиатуры
char prev_key = 0;           // ключ символа нажатой кнопки клавиатуры на прошлом опросе кнопки
char is_beep_seq_started = 0;     // флажок запуска последовательности сигналов после нажатия кнопки
char is_need_delay = 0;      // флажок для организации паузы между сигналами
int curr_signal_dur_idx = 0; // индекс проигрываемого сигнала в серии сигналов
unsigned int ms_ctr = 0;     // счетчик миллисекунд от начала работы программы
unsigned int beep_ctr = 0;   // счетчик для задания длительности сигналов зумера

void beep_on() {BEEP_BIT = 0;}
void beep_off() {BEEP_BIT = 1;}

void init_timer(void) 
{// инициализация таймера
	TMOD = 0x01;
	TL0 = RELOAD_VALUE_L;
	TH0 = RELOAD_VALUE_H;
	ET0 = 1;
	TR0 = 1;
}

void change_key_handler()
{// обработчик изменения нажатой клавиши клавиатуры
    beep_ctr = ms_ctr;

    curr_signal_dur_idx = 0;
    is_beep_seq_started = TRUE;
    is_need_delay = FALSE;
    
    //отчистка дисплеев
	LCD_clrscr();
	SHIFT_write(0);

    curr_keyname = KEY_getkeyname(key);
    prev_keyname = KEY_getkeyname(prev_key);

	if (curr_keyname == KEY_ASTERISK) {
		LED7_setdigit( prev_keyname );
	}
	else {
		LED7_setdigit( curr_keyname );
		LCD_print( curr_keyname );
	}

    beep_on();
}

void beep_handler() 
{// обработчик сигналов зумера
    if (ms_ctr - beep_ctr > signal_dur_seq[curr_signal_dur_idx] && is_need_delay == FALSE)
    {
        is_need_delay = TRUE;
        beep_ctr = ms_ctr;
        beep_off();
        curr_signal_dur_idx++;
    }

    if (ms_ctr - beep_ctr > PAUSE_DUR && is_need_delay == TRUE) {
        is_need_delay = FALSE;
        if (curr_signal_dur_idx == 2) {
            curr_signal_dur_idx = 0;
            is_beep_seq_started = FALSE;
            continue;
        }
        beep_on();
    }
}

void button_handler()
{ // обработчик нажатия кнопки

    if (ms_ctr % BUTTONS_ASK_DUR == 0) // опрашиваем клавиатуру раз в 10 мс
        key = KEY_getkey();
    if (!key)
        continue;
    if (key != prev_key)
        change_key_handler();
    
    beep_handler();
    prev_key = key;
}

void ISR(void) interrupt 1 using 0 
{// обработка прерывания
	ms_ctr++;
}

void main() // Основная функция
{
	P0 = 0xFF; // Иниц LED, LCD, SPI, I2C, прерывания разрешены
	P1 = 0xFF;
	P2 = 0xFF;
	P3 = 0xFF;
	EA = 1;
	init_timer();
	SPI_init();
	I2C_init();
	LCD_init();

	printf("Task_1"); 
	while (1)
	{
	}
}