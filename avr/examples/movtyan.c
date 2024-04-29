#include <intrins.h>
#include <stdio.h>
#include <string.h>
#include <util/delay.h>

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

flash char UartMessage[] = "                -=  STEND RK-10  =-\r\n"
                           "       Hardware ver. 1.0, Firmware ver. 2.0\r\n"
                           "                   Movtyan Vadim\n"
                           "                        LAB1 AVR\n        ";

void timer_ini(void)
{
    TCCR1B |= (1 << WGM12); // Устанавливаем режим СТС (сброс по совпадению)
    TIMSK |= (1«OCIE1A);    // Устанавливаем бит разрешения прерывания 1ого счетчика по совпадению с OCR1A(H и L)
    TIMSK |= (1«OCIE1B);    // Устанавливаем бит разрешения прерывания 1ого счетчика по совпадению с OCR1B(H и L)

    OCR1AH = 0b00000110; // Записываем в регистр число для сравнения (14756000/ 1024) * 0.1 = 1562
    OCR1AL = 0b00011010;

    OCR1BH = 0b00111101; // Записываем в регистр число для сравнения (14756000/ 1024) * 1 = 15625
    OCR1BL = 0b00001001;
    TCCR1B |= (1 << CS12) | (1 << CS10); // Установим делитель на 1024
}

char key = 0;
char keynumber;
uint8_t prev_state = 0;
uint8_t current_state;
volatile int flag_delay = 1;
void beep1s_or_beep2s(void)
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
    switch (key_number)
    {
    case 1:
    {                        // Если нажали 1
        OCR1BH = 0b00111101; // Записываем в регистр число для сравнения (14756000/ 1024) * 1 = 15625
        OCR1BL = 0b00001001;
        break;
    }
    case 2:
    {                        // Если нажали 2
        OCR1BH = 0b01111010; // Записываем в регистр число для сравнения (14756000/ 1024) * 2 = 31250
        OCR1BL = 0b00010010;
        break;
    }
    }
}
ISR(TIMER1_COMPA_vect)
{
    beep1s_or_beep2s();
}

char flag_BEEP = 1;
ISR(TIMER1_COMPB_vect)
{
    BEEP_BIT = flag_BEEP;
    flag_BEEP != flag_BEEP;
}

int main()
{

    timer_ini();
    DDRB = 0x00;
    PORTD = 0b11111111;
    PORTB = 0b00000001;
    UART_init(CALC_UBRR(57600)); // инициализация UART
    sei();                       // Разрешим глобальные прерывания
    UART_sendstring_flash(UartMessage);
    while (1)
    {
    }
}
