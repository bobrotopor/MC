#include <intrins.h>
#include <stdio.h>
#include <string.h>

#include "AT24CXX.H"
#include "BEEP.H"
#include "DELAY.H"
#include "I2C.H"
#include "KEYBOARD.H"
#include "LCD_HD44780.H"
#include "LED7.H"
#include "PINDEF.h"
#include "SHIFT.H"
#include "SPI.H"

#define CLOCK_FREQUENCY 22118400

void set_LED7(char value)
{ // вывод цифры на семисегментный индикатор
    if ((value > 9) || (value < 0))
        value = 0; // если значение больше 9 или меньше 0, то выводится 0
    LED7_setdigit(value);
}

void init_UART() // инициализация UART
{
    SCON = 0x50; // 8-битовый UART
    TMOD = 0x20; // Таймер 1: 8-битовый режим, авто-перезагружаемый
    TH1 = 250;   // Задаем начальное значение таймера для скорости 9600 бод
    TL1 = 250;
    TR1 = 1; // Запускаем таймер
    TI = 1;
    ES = 1; // Разрешаем прерывание от UART
}

void timer_ini(void)
{
    unsigned int counts;
    TMOD |= 0x01; // Режим 1: 16-битный таймер

    counts = 0xFFFF - (CLOCK_FREQUENCY / 12 / 50); // Рассчитываем значения для периода в 20 миллисекунд

    TH0 = (unsigned char)(counts >> 8); // Значение для старшего байта
    TL0 = (unsigned char)(counts);      // Значение для младшего байта

    ET0 = 1; // Разрешение прерывания от таймера 0
    TR0 = 1; // Запуск таймера 0
}

char key = 0;
char keynumber;
uint8_t prev_state = 0;
uint8_t current_state;
volatile int flag_delay = 1;
void set_key(void)
{                                 // Обработка клавиатуры
    current_state = KEY_getkey(); // Получить код нажатой кнопки
    if (current_state != prev_state)
    {
        If(flag_delay)
        {
            flag_delay = 0;
            return;
        }
        key = current_state;
        key_number = KEY_getkeynumber(key);
        step1 = 1;
        flag_delay = 1;
        prev_state = current_state;
    }
    if (key != key_ASTERISK && key != key_BAR)
        set_LED7(keynumber); // Нажата клавиша, кроме звездочки или решетки - выводим её на индикатор
}
volatile int value_timer = 0;
void ISR_Interrupt(void) interrupt 1 using 0
{
    set_key();
    if (value_timer % 1000 == 0)
    { // Смена на значение меньше семисегментника через секунду
        keynumber--;
        set_LED7(keynumber);
    }
    value_timer += 20; // добавляем к переменной-счетчику значение периода прерывания
}

void main()
{
    P0 = 0xFF; // настраиваем порты на вывод
    P1 = 0xFF;
    P2 = 0xFF;
    P3 = 0xFF;

    SPI_init();
    I2C_init();
    init_UART();
    timer_ini(); // Инициализация таймера
    printf("                -=  STEND RK-10  =-\r\n"
           "       Hardware ver. 1.0, Firmware ver. 2.0\r\n"
           "                   Movtyan Vadim\n"
           "                        LAB1 x51\n        ");

    while (1)
    {
    }
}
