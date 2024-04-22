#include <stdio.h>
#include <intrins.h>
#include <string.h>

#include "PINDEF.H"
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
Студент: 	Балкунов К.С.
Группа:		СМ7-74Б
Формулировка задачи:
	1) Вывести цифры от 0 до 7 десятичной системы счисления, используя клавиатуру стенда,
		в двоичном представолении, используя светодиоды стенда.
	2) Параллеьно с включением светодиодов, воспроизвести динамиком двоичное число 
		по следущем правилам: 
			* Динный сигнал соответствует биту 1, короткий - 0. 
			* Число воспроизводится от младшего бита к старшему.
	3) При нажатии * светодиоды выключаются. Динамик при этом перестаёт издавать звуки.
*/

#define CLOCK 22110000
#define TIMER_FREQ_HZ 1000
#define RELOAD_VALUE (65536 - (CLOCK / 12 / TIMER_FREQ_HZ))
#define RELOAD_VALUE_H ((unsigned char)(RELOAD_VALUE >> 8))
#define RELOAD_VALUE_L ((unsigned char)(RELOAD_VALUE))

/*Что бы не запутаться и учесть то, что МК осуществляет управление выводом отрицательной
полярности добавим макросы ВКЛЮЧЕНИЯ и ВЫКЛЮЧЕНИЯ светодиодов*/
#define HIGH 0		 // сигнал для подачи высокого уровня на выход для включения светодиода
#define LOW 1		 // сигнал для подачи низкого уровня на выход для выключения светодиода
#define BITS_COUNT 3			// кол-во битов двоичного числа
#define LEDS_COUNT BITS_COUNT 	// количество светодиодов подключенных к пинам контроллера на стенде
#define BITS_COUNT_MINUS_1 BITS_COUNT - 1

#define BEEP_DURATION 250		// зажкржка между сигналами
#define BEEP_HIGH 500			// длительность сигнала соответствующему биту 1
#define BEEP_LOW 250			// длительность сигнала соответствующему биту 0

#define ISR_DURATION 25
#define ISR_DURATION_2 ISR_DURATION*2

#define FALSE 0
#define TRUE 1

char led_pins_signals[][LEDS_COUNT] = {
	{LOW, LOW, LOW},	// 000
	{HIGH, LOW, LOW},	// 001
	{LOW, HIGH, LOW},	// 010
	{HIGH, HIGH, LOW},	// 011
	{LOW, LOW, HIGH},	// 100
	{HIGH, LOW, HIGH},	// 101
	{LOW, HIGH, HIGH},	// 110
	{HIGH, HIGH, HIGH}, // 111
};

unsigned int ms_counter = 0;	// счетчик миллисекнуд
int singed_bits_counter = 0;	// счетчик сыгнранных зуммером битов двоичного числа
unsigned int dur_counter = 0;	// миллисекндный счетчик для задержки между сигналами

unsigned char key = 0;	 // Адрес клавиши
int curr_key_number = 0; // цифра по адресу клавиши
char is_reveal_process_started = FALSE;	// флажок процесса проигрывания числа и отображения
char need_duration = FALSE;		// флажок задержки между проигрыванием битов


void init_timer0(void) 
{// инициализация таймера 0
	TMOD = 0x01; // 16-битовый режим
	TL0 = RELOAD_VALUE_L;
	TH0 = RELOAD_VALUE_H;
	ET0 = 1; // Бит разрешения прерывания от таймера 0
	TR0 = 1; // Бит управления таймера 0 для пуска/останова таймера/счетчика 
}

void stop_revealing() 
{//остановить проигрывание и отображение битов двоичного числа
	LED1 = LOW;
	LED2 = LOW;
	LED3 = LOW;

	singed_bits_counter = 0;

	BEEP_BIT = LOW;
	need_duration = FALSE;
	is_reveal_process_started = FALSE;
}

void button_handler() {
	// Обработчик нажатия кнопок клавиатуры
	key = KEY_getkey(); 						// подлучаем адрес нажатой клавиши
	curr_key_number = KEY_getkeynumber(key);	// получаем цифру по адресу клавиши

	if (key == KEY_ASTERISK)					//При нажатии * светодиоды выключаются. 
		stop_revealing();						//Динамик при этом перестаёт издавать звуки.
			
	if (curr_key_number >= 0 && curr_key_number < 8) {
		is_reveal_process_started = TRUE;
		dur_counter = ms_counter; //обновляем счетчик для генерации сигналов зумера
	}
}

void reveal_curr_number()
{
	// Вывести цифры от 0 до 7 десятичной системы счисления в двоичном представолении,
	// через светодиоды стенда.
	char is_bit_singed = FALSE;				// проигран ли один из битов двоичного числа?  

	// если нажата цифра в диапоне от 0 до 7,
	// то зажигаем светодиоды в соответсвии с битами двоичного числа
	if (curr_key_number >= 0 && curr_key_number < 8 && is_reveal_process_started == TRUE)
	{
		set_led_pins_signals(curr_key_number);			// включить светодиоды в соответсвтии 
														// с двоичным числом
		
		// вопспроизвести динамиком бит двочиного числа
		is_bit_singed = sing_binary_bit_by_zoomer(curr_key_number, singed_bits_counter);		
		if (is_bit_singed)
			singed_bits_counter++;
	}

	if (singed_bits_counter == BITS_COUNT_MINUS_1) {	// если сыграли зуммером все биты сбрасываем счетчик битов,
		singed_bits_counter = 0;						// сбрасываем флаг is_reveal_process_started на 0
		is_reveal_process_started = FALSE;
	}
}


void set_led_pins_signals(unsigned int curr_number)
{
	// Установить уровни сигналов пинов светодиодов в соответсвтии с двоичным числом
	LED1 = led_pins_signals[curr_number][0];
	LED2 = led_pins_signals[curr_number][1];
	LED3 = led_pins_signals[curr_number][2];
}

char sing_binary_bit_by_zoomer(unsigned int curr_number, unsigned int curr_bit) 
{
	// Вопспроизвести динамиком (зумером) двочиное число. 
	// Динный сигнал соответствует биту 1, короткий - 0. 
	// Число воспроизводится от младшего бита к старшему.
	char is_bit_singed = FALSE;	// возвращаемое значение
	
	if (led_pins_signals[curr_number][curr_bit] == HIGH && need_duration == FALSE) {
		BEEP_BIT = HIGH;			// если текущий бит двочиного числа равен 1,
								// то включаем зумер на время BEEP_HIGH.
		if (ms_counter - dur_counter > BEEP_HIGH) {
			dur_counter = ms_counter;
			BEEP_BIT = LOW;
			need_duration = TRUE;
		}
	}
	else						
		BEEP_BIT = HIGH;			// иначе включаем зумер на время BEEP_LOW
		if (ms_counter - dur_counter > BEEP_LOW && need_duration == FALSE) {
			dur_counter = ms_counter;
			BEEP_BIT = LOW;
			need_duration = TRUE;
		}
	// задержка между сыгранными битами величиной в BEEP_DURATION
	if (ms_counter - dur_counter > BEEP_DURATION && need_duration == TRUE) {
		need_duration = FALSE;
		is_bit_singed = TRUE;
	}
	return is_bit_singed;
}


void timer0_ISR(void) interrupt 1 using 0 
{   // обработчик прерывания по таймеру 

	button_handler();
	
	if (ms_counter % ISR_DURATION_2 == 0)
		reveal_curr_number(ms_counter);

	ms_counter+=ISR_DURATION;
}


// главная функция
void main()
{
	// Настраиваем порты на вывод
	P0 = 0xFF;
	P1 = 0xFF;
	P2 = 0xFF;
	P3 = 0xFF;
	EA = 1;  // Разрешаем прерывания

	while (1)
	{
		/* бсконечный цикл */
	}
}
