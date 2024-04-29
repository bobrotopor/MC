#include "SPI.H"
#include "LCD_HD44780.H"
#include "I2C.H"
#include "KEYBOARD.H"
#include "LED7.H"
#include "UART.H"
#include "DS1307.H"
#include "DS1820.H"
#include "AT24CXX.H"
#include "CONVERT.H"
#include "PINDEF.h"

/*
Студент: 	Балкунов К.С.
Группа:		СМ7-84Б
Вариант: 4
Задача: 3

1) При запуске программы на экран терминала выводится сообщение «Задача №3»;
2) При нажатии на любую кнопку клавиатуры PC зуммер дает 3 сигнала
   длительностью 100 мс с паузой 100 мс,
   после паузы 300 мс – 3 сигнала длительностью 300 мс с паузой 100 мс,
   после паузы 300 мс - 3 сигнала длительностью 100 мс с паузой 100 мс,
   и символ выводится на ЖКИ стенда. При нажатии же клавиш 0-9 соответствующие цифры,
   помимо звукового сигнала, выводятся еще на семисегментный индикатор.
*/

#define ONESHOT_DELAY 100    // пауза 100 мс между одиночными сигналами
#define PAUSE_DUR 300        // пауза 300 мс между сериями сигналов
#define BUTTONS_ASK_DUR 10   // время опроса нопок клавиатуры

#define FALSE 0
#define TRUE 1

int signal_dur_seq[] = {100, 300, 100}; // последовательность длительности сигналов в серии
flash char UartMessageTask3[] = "Task 3\r\n";

char key = 0;                // ключ символа нажатой кнопки клавиатуры
char prev_key = 0;           // ключ символа нажатой кнопки клавиатуры на прошлом опросе кнопки

char is_beep_seq_started = 0;     // флажок запуска последовательности сигналов после нажатия кнопки
char is_need_delay = 0;      // флажок для организации паузы между сигналами
char button_read_flag = FALSE;	//флажок чтения адресов кнопок клавиатуры стенда

int curr_signal_dur_idx = 0; // индекс проигрываемого сигнала в серии сигналов
unsigned int ms_ctr = 0;     // счетчик миллисекунд от начала работы программы
unsigned int beep_ctr = 0;   // счетчик для задания длительности сигналов зумера
unsigned int button_ms_ctr = 0; // счетчик миллисекнуд для исключения дребезга контактов

void beep_on() {BEEP_BIT = 0;}
void beep_off() {BEEP_BIT = 1;}

void init_timer0()
{   // Настройка таймера 0 на 1000 Гц (1 мс)
    // Настройка тактирования таймера 1000 Гц (1 мс)
    TCCR0 = 0;                    // Очистить конфигурационный регистр таймера 0
    TCCR0 |= (1 << WGM01);  // устанавливаем режим СТС (сброс по совпадению)
    TCCR0 |= (1 << CS02);     // Предделитель 256

    OCR0 = 58;         // Значение для достижения 1000 Гц (1 мс) компаратор А
    TIMSK_OCIE0 |= 1;  // Запуск компаратора таймера 0
}

#pragma vector = TIMER0_COMP_vect     
__interrupt void TIMER0_COMP(void)
{
    if (ms_ctr % 10 == 0) button_handler();
    if (ms_ctr % 100 == 0) beep_handler();
    ms_ctr++;
}

void change_key_handler()
{// обработчик нового значения нажатой клавиши клавиатуры
    beep_ctr = ms_ctr;

    curr_signal_dur_idx = 0;
    is_seq_started = TRUE;
    is_need_delay = FALSE;

    keyname = KEY_getkeyname(key);
    if (keyname >= '0' && keyname <= '9') {
        SHIFT_write(0);
        LED7_setdigit( keyname - '0' );
    }
    
    LCD_clrscr();
    LCD_print(keyname);

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
            is_seq_started = FALSE;
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
			button_ms_ctr = ms_ctr;
			return;
		}

		if (button_read_flag == TRUE && ms_ctr - button_ms_ctr > 20)
			button_read_flag = FALSE;
		else
			return;

		change_key_handler();

		prev_key = key;
	}
}


int main()
{

    I2C_init();
    SPI_init();
    LCD_init();
    LCD_clrscr();

    UART_init(CALC_UBRR(9600));
    __enable_interrupt();

    init_timer0();
    UART_sendstring_flash(UartMessageTask3);
    OutputStartLCDMessage();

    while (1)
    {
        button_handler(); // читаем данные и выполняем соответсвующие инструкции
    }
}
