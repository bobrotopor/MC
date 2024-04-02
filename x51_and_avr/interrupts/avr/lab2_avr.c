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
flash char UartMessageTask3[] = "Задача №3\r\n";

char key = 0;                // ключ символа нажатой кнопки клавиатуры
char prev_key = 0;           // ключ символа нажатой кнопки клавиатуры на прошлом опросе кнопки
char is_seq_started = 0;     // флажок запуска последовательности сигналов после нажатия кнопки
char is_need_delay = 0;      // флажок для организации паузы между сигналами
int curr_signal_dur_idx = 0; // индекс проигрываемого сигнала в серии сигналов
unsigned int ms_ctr = 0;     // счетчик миллисекунд от начала работы программы
unsigned int beep_ctr = 0;   // счетчик для задания длительности сигналов зумера

void beep_on() {BEEP_BIT = 0;}
void beep_off() {BEEP_BIT = 1;}

void init_timer1()
{   // Настройка таймера 1 (частота atmega16 - 8МГц) 1000 Гц (1 мс)
    // Настройка тактирования таймера 1000 Гц (1 мс)
    TCCR1B = 0;                    // Очистить регистр управления компаратором 1В
    TCCR1B |= (1 << TCCR1B_WGM12); // устанавливаем режим СТС (сброс по совпадению)
    TCCR1B |= (1 << TCCR1B_CS11);  // Предделитель 8

    OCR1A = 1000;      // Значение для достижения 1000 Гц (1 мс) компаратор А
    TIMSK_OCIE1A |= 1; // Запуск компаратора таймера А
}

#pragma vector = TIMER1_COMPA_vect
__interrupt void Timer1_COMPA(void)
{
    ms_ctr++;
}

void change_key_handler()
{// обработчик изменения нажатой клавиши клавиатуры
    beep_ctr = ms_ctr;

    curr_signal_dur_idx = 0;
    is_seq_started = TRUE;
    is_need_delay = FALSE;
    
    LCD_clrscr();

    keyname = KEY_getkeyname(key);
    if (keyname >= '0' && keyname <= '9')
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

int main()
{

    I2C_init();
    SPI_init();
    LCD_init();
    LCD_clrscr();

    UART_init(CALC_UBRR(57600));
    __enable_interrupt();

    init_timer1();
    UART_sendstring_flash(UartMessageTask3);
    OutputStartLCDMessage();

    while (1)
    {
        button_handler(); // читаем данные и выполняем соответсвующие инструкции
    }
}
