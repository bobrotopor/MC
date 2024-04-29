#include <mega16.h>
#include <io.h>
#include <delay.h>
#include <interrupt.h>
#include "BEEP.h"
#include "DELAY.H"
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
#include "SHIFT.H"
#include "ADC.H"
#include "iom16.h"

#define F_CPU 14756000 // частота микроконтроллера
#define ENABLE_BIT_DEFINITIONS

flash char UartMessage[] = "                HELLO WORLD!\r\n";
flash char enterHourStr[] = "Enter hour: ";
flash char enterMinuteStr[] = "Enter minute: ";
flash char enterSecondStr[] = "Enter second: ";
unsigned int i;
unsigned char task;

// через UART пользователь вводит новое время
void UART_settime()
{
    unsigned char hour, minute, second;
    UART_sendstring_flash(enterHourStr);
    hour = UART_receivevalue();
    UART_sendstring_flash(enterMinuteStr);
    minute = UART_receivevalue();
    UART_sendstring_flash(enterSecondStr);
    second = UART_receivevalue();
    DS1307_settime(hour, minute, second);
}

// при нажатии на телефонную клавиатуру раздается сигнал
void UART_beep()
{
    unsigned char key = 0;
    while (key != KEY_ASTERISK)
    {
        key = KEY_getkey();
        if (!key)
            continue;
        Beep(100);
        KEY_waitforkeyrelease();
    }
}

// при нажатии на кнопки телефонной клавиатуры соответсвующая цифра появляется на led экране
void UART_lcd()
{
    unsigned char key = 0;
    char keyname;
    unsigned char keyCount = 0;
    LCD_clrscr();
    while (key != KEY_ASTERISK)
    {
        key = KEY_getkey();
        if (!key)
            continue;
        keyname = KEY_getkeyname(key);

        if (keyname >= '0' && keyname <= '9')
        {
            LCD_printchar(keyname);
            keyCount++;

            if (keyCount == LCD_WIDTH)
                LCD_gotoxy(0, 1);
        }
        else if (key == KEY_BAR)
        {
            LCD_clrscr();
            keyCount = 0;
        }

        KEY_waitforkeyrelease();
    }
}
void InitASS_Timer()
{
    if (ASSR & (1 << AS2)) // Если это второй вход то
    {
        if (ASSR & (1 << TCN2UB | 1 << OCR2UB | TCR2UB)) // проверяем есть ли хоть один бит флаговый
        {
            SetTask(InitASS_Timer); // Если есть, то отправляем на повторный цикл ожидания
        }
        else // Если все чисто, то можно запускать прерывания
        {
            TIFR |= 1 << OCF2 | 1 << TOV2; // Сбрасываем флаги прерываний, на всякий случай.
            TIMSK |= 1 << TOIE2;           // Разрешаем прерывание по переполнению
            return;
        }
    }
    TIMSK &= ~(1 << OCIE2 | 1 << TOIE2); // Запрещаем прерывания таймера 2
    ASSR = 1 << AS2;                     // Включаем асинхронный режим
    TCNT2 = 0;
    TCCR2 = 5 << CS20;      // Предделитель на 128 на 32768 даст 256 тиков в секунду
                            // Что даст 1 прерывание по переполнению в секунду.
    SetTask(InitASS_Timer); // Прогоняем через диспетчер, чтобы зайти снова.
}
// с помощью асинхронного таймера 2 создаем секундомер реального времени на семисегментном индикатора
#pragma vector = TIMER2_OFV_vect
__interrupt void Timer2_OFV(void)
{
    if (i > 9)
        i = 0;
    LED7_setdigit(i); // вывод на семисегментный индикатор
    i++;
}
// обработчик прерываний по UART
#pragma vector = UART_RXC_vect
__interrupt void UART0_RX_interrupt()
{
    task = UART_receivevalue(); // получение задачи
    UART_sendstring("Enter task number: ");
}
void main()
{
    i = 0;
    SREG |= 1 << 7;                    // глобальное разрешение прерываний
    InitRTOS();                        // Инициализируем ядро
    RunRTOS();                         // Старт ядра.
    I2C_init();                        // Инициализируем I2C интерфейс
    SPI_init();                        // Инициализируем SPI интерфейс
    LCD_init();                        // Инициализируем LCD дисплей
    LCD_clrscr();                      // Очищаем экран LCD дисплея
    SetTimerTask(InitASS_Timer, 1000); // асинхронный режим на таймере 2
                                       // запускается медленно, поэтому делаем выдержку
    UART_init(CALC_UBRR(9600));
    sei(); // разрешаем прерывания для UART
    UART_sendstring_flash(UartMessage);
    UART_sendstring("Enter task number: ");
    while (1)
    {
        switch (task)
        {
        case 1:
            UART_settime();
            break;
        case 2:
            UART_beep();
            break;
        case 3:
            UART_lcd();
            break;
        }
    }
}
