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
char curr_key_name = 0;      // значение кнопки по ключу key
char prev_key_name = 0;      // значение кнопки по ключу prev_key

char is_beep_seq_started = 0;     // флажок запуска последовательности сигналов после нажатия кнопки
char is_need_delay = 0;      // флажок для организации паузы между сигналами
char button_read_flag = FALSE;	//флажок чтения адресов кнопок клавиатуры стенда

int curr_signal_dur_idx = 0; // индекс проигрываемого сигнала в серии сигналов

unsigned int ms_ctr = 0;     // счетчик миллисекунд от начала работы программы
unsigned int beep_ctr = 0;   // счетчик для задания длительности сигналов зумера
unsigned int button_ms_ctr = 0; // счетчик миллисекнуд для исключения дребезга контактов


void beep_on() {BEEP_BIT = 0;}
void beep_off() {BEEP_BIT = 1;}

void init_timer0(void) 
{// инициализация таймера
	TMOD = 0x01;
	TL0 = RELOAD_VALUE_L;
	TH0 = RELOAD_VALUE_H;
	ET0 = 1;
	TR0 = 1;
}

void init_UART() 
{// инициализация UART
    SCON = 0x50; // 8-битовый UART
    TMOD = 0x20; // Таймер 1: 8-битовый режим, авто-перезагружаемый
    TH1 = 250;   // Задаем начальное значение таймера для скорости 9600 бод
    TL1 = 250;
    TR1 = 1; // Запускаем таймер
    TI = 1;
    ES = 1; // Разрешаем прерывание от UART
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

	if (key == KEY_ASTERISK) {
		LED7_setdigit( prev_key_name );
	}
	else {
		LED7_setdigit( curr_key_name );
		LCD_print( curr_key_name );
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
            return;
        }
        beep_on();
    }
}

void button_handler()
{
	// Обработчик нажатия кнопок клавиатуры
	key = KEY_getkey(); // подлучаем адрес нажатой клавиши

	if (key != prev_key)
	{
		if (button_read_flag == FALSE)
		{
			button_read_flag = TRUE;
			button_ms_ctr = ms_counter;
			return;
		}

		if (button_read_flag == TRUE && ms_counter - button_ms_ctr > 20)
			button_read_flag = FALSE;
		else
			return;

		key_name = KEY_getkeynumber(key); // получаем цифру по адресу клавиши

        change_key_handler();

		prev_key = key;
        prev_key_name = KEY_getkeyname(prev_key);
	}
}

void ISR(void) interrupt 1 using 0 
{// обработка прерывания

    if (ms_ctr % 10 == 0)
        button_handler();

    if (ms_ctr % 100 == 0)
        beep_handler();

	ms_ctr++;
}

void main() // Основная функция
{
    // Иниц LED, LCD, SPI, I2C, прерывания разрешены
	P0 = 0xFF; 
	P1 = 0xFF;
	P2 = 0xFF;
	P3 = 0xFF;
	EA = 1;
    SCON  = 0x50;			
	TMOD  = 0x20;			
	TH1   = 250;			
	TL1   = 250;			
	TR1   = 1;
	TI    = 1;
	ES=1; 
    
	init_timer0();
    init_UART();
	SPI_init();
	I2C_init();
	LCD_init();

	printf("Task_1"); 
	while (1)
	{
	}
}