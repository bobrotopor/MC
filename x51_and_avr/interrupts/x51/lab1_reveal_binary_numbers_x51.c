/*
Студент: 	Балкунов К.С.
Группа:		СМ7-74Б
Формулировка задачи:
	1) Вывести цифры от 0 до 7 десятичной системы счисления, используя клавиатуру,
		в двоичном представолении, используя светодиоды стенда.
	2) Параллеьно с включением светодиодов, воспроизвести динамиком двоичное число 
		по следущем правилам: 
			* Динный сигнал соответствует биту 1, короткий - 0. 
			* Число воспроизводится от младшего бита к старшему.
	3) При нажатии * светодиоды выключаются. Динамик при этом перестаёт издавать звуки.
*/

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

bool led_pins_signals[][LEDS_COUNT] = {
	{LOW, LOW, LOW},	// 000
	{HIGH, LOW, LOW},	// 001
	{LOW, HIGH, LOW},	// 010
	{HIGH, HIGH, LOW},	// 011
	{LOW, LOW, HIGH},	// 100
	{HIGH, LOW, HIGH},	// 101
	{LOW, HIGH, HIGH},	// 110
	{HIGH, HIGH, HIGH}, // 111
};

unsigned char key = 0;	 // Адрес клавиши
int curr_key_number = 0; // цифра по адресу клавиши
bool is_reveal_process_started = false;	// флажок процесса проигрывания числа и отображения
bool need_duration = false;		// флажок задержки между проигрыванием битов

void button_handler() {
	// Обработчик нажатия кнопок клавиатуры
	key = KEY_getkey(); 						// подлучаем адрес нажатой клавиши
	curr_key_number = KEY_getkeynumber(key);	// получаем цифру по адресу клавиши

	if (key == KEY_ASTERISK) {					//При нажатии * светодиоды выключаются. 
		is_reveal_process_started = false;		//Динамик при этом перестаёт издавать звуки.
		return;
	}		
	if (curr_key_number >= 0 && curr_key_number < 8)
		is_reveal_process_started = true;
	
}

void reveal_curr_number(unsigned int ms_counter)
{
	// Вывести цифры от 0 до 7 десятичной системы счисления в двоичном представолении,
	// через светодиоды стенда.

	static int singed_bits_counter = 0;		// счетчик сыгнранных зуммером битов двоичного числа
	static unsigned int local_counter = 0;	// миллисекндный счетчик для задержки между сигналами
	bool is_bit_singed = false;				// проигран ли один из битов двоичного числа?  
	
	// если нажата * (звездочка) выключаем все светодиоды и зуммер
	if (key == KEY_ASTERISK)
	{
		LED1 = LOW;
		LED2 = LOW;
		LED3 = LOW;

		singed_bits_counter = 0;

		BEEP_BIT = LOW;
		need_duration = false;
	}

	// если нажата цифра в диапоне от 0 до 7,
	// то зажигаем светодиоды в соответсвии с битами двоичного числа
	if (curr_key_number >= 0 && curr_key_number < 8 && is_reveal_process_started == true)
	{
		set_led_pins_signals(curr_key_number);			// включить светодиоды в соответсвтии 
														// с двоичным числом

		// вопспроизвести динамиком бит двочиного числа
		is_bit_singed = sing_binary_bit_by_zoomer(curr_key_number, singed_bits_counter, ms_counter);		
		if (is_bit_singed)
			singed_bits_counter++;
	}

	if (singed_bits_counter == BITS_COUNT_MINUS_1) {	// если сыграли зуммером все биты сбрасываем счетчик,
		singed_bits_counter = 0;						// сбрасываем флаг is_reveal_process_started на 0
		is_reveal_process_started = false;
	}
}


void set_led_pins_signals(unsigned int curr_number)
{
	// Установить уровни сигналов пинов светодиодов в соответсвтии с двоичным числом
	LED1 = led_pins_signals[curr_number][0];
	LED2 = led_pins_signals[curr_number][1];
	LED3 = led_pins_signals[curr_number][2];
}

bool sing_binary_bit_by_zoomer(unsigned int curr_number, unsigned int curr_bit, unsigned int ms_counter) 
{
	// Вопспроизвести динамиком (зумером) двочиное число. 
	// Динный сигнал соответствует биту 1, короткий - 0. 
	// Число воспроизводится от младшего бита к старшему.
	static unsigned int local_counter = 0;
	bool is_bit_singed = false;	// возвращаемое значение
	
	if (led_pins_signals[curr_number][curr_bit] == HIGH && need_duration == false) {
		BEEP_BIT = HIGH;			// если текущий бит двочиного числа равен 1,
								// то включаем зумер на время BEEP_HIGH.
		if (ms_counter - local_counter > BEEP_HIGH) {
			local_counter = ms_counter;
			BEEP_BIT = LOW;
			need_duration = true;
		}
	}
	else						
		BEEP_BIT = HIGH;			// иначе включаем зумер на время BEEP_LOW
		if (ms_counter - local_counter > BEEP_LOW && need_duration == false) {
			local_counter = ms_counter;
			BEEP_BIT = LOW;
			need_duration = true;
		}
	// задержка между сыгранными битами величиной в BEEP_DURATION
	if (ms_counter - local_counter > BEEP_DURATION && need_duration == true) {
		need_duration = false;
		is_bit_singed = true;
	}
	return is_bit_singed;
}


void init_timer0(void) {                            // инициализировать таймер 0
  unsigned int counts;                              
  TMOD |= 0x01;                                     // 16-битовый режим

  counts = (unsigned int)(ISR_DURATION * 1000.0 / TACT_TIME_MCS); 
                                                    // 25 мс (40 Гц)

  TH0 = 0xFFFF - counts + 1;                        // начальное значение таймера 0 
  TL0 = 0xFFFF - counts + 1;                       
  ET0 = 1;                                          // Бит разрешения прерывания от таймера 0
  TR0 = 1;                                          // Бит управления таймера 0 для пуска/останова таймера/счетчика 
                                         
}


void timer0_ISR(void) interrupt 1 using 0 {      // обработчик прерывания по таймеру 

	static unsigned int ms_counter = 0;
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

	SCON = 0x50; // 8-битовый UART
	TMOD = 0x20; // Таймер 1: 8-битовый режим, авто-перезагружаемый
	TH1 = 250;	 // Задаем начальное значение таймера для скорости
				 // 9600 бод.
	TL1 = 250;
	TR1 = 1; // Запускаем таймер
	TI = 1;
	ES = 1; // Разрешаем прерывание от UART

	while (1)
	{
		/* бсконечный цикл */
	}
}
