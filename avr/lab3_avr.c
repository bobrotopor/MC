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
Задача: 4

1)  При запуске программы на экран терминала выводится сообщение «Задача №4.
2)  При нажатии клавиши 1 клавиатуры PC предлагается ввести время и дату,
    которые затем устанавливаются как текущие в RTC DS1307.
    При нажатии клавиши 2 на ЖКИ стенда в первой строке выводится текущее время,
    во второй дата. При нажатии клавиши 3 происходит очистка ЖКИ.
    Нажатие на другие клавиши игнорируется.
3)  Выводить на экран показания двух АЦП в вольтах, показания второго АЦП
кратные 10 выводить на семисегментный индикатор. При показаниях больше 99 включить зуммер.
*/

#define FALSE 0
#define TRUE 1

flash char UartMessageTask4[] = "Задача №4\r\n";
flash char enterHourStr[] = "Enter hour: ";
flash char enterMinuteStr[] = "Enter minute: ";
flash char enterSecondStr[] = "Enter second: ";
flash char enterDayStr[] = "Enter day: ";
flash char enterMonthStr[] = "Enter month: ";
flash char enterYearStr[] = "Enter year: ";
flash char sensorNotConnectedStr[] = "Sensor is not connected";

int mode = 0;
char is_mode_executed = FALSE;
unsigned char pc_keyboard_data = 0;


void init_ADC() {ADCSRA |= (1 << ADEN);}    // активация АЦП
void init_timer1()
{ // Настройка таймера 1 (частота atmega16 - 8МГц) 1000 Гц (1 мс)
    // Настройка тактирования таймера 1000 Гц (1 мс)
    TCCR1B = 0;                    // Очистить регистр управления компаратором 1В
    TCCR1B |= (1 << TCCR1B_WGM12); // устанавливаем режим СТС (сброс по совпадению)
    TCCR1B |= (1 << TCCR1B_CS11);  // Предделитель 8

    OCR1A = 1000;      // Значение для достижения 1000 Гц (1 мс) компаратор А
    TIMSK_OCIE1A |= 1; // Запуск компаратора таймера А
}

void echo_ADC(int adc1, int adc2) 
{ // Вывод консоль по UART показания двух АЦП в вольтах
    char str_data_ADC1[10];
    char str_data_ADC2[10];
    ShortToString( adc1, str_data_ADC1 );
	ShortToString( adc2, str_data_ADC2 );
    UART_sendstring( "ADC1: " );
    UART_sendstring( str_data_ADC1 );
    UART_sendstring("\n\r");
    UART_sendstring( "ADC2: " );
    UART_sendstring( str_data_ADC2 );
    UART_sendstring("\n\r");
}

void beep_if_ADC_over99(int adc1, int adc2) 
{ // Включение зуммера при преодолении порога 99
    if ((ADC_get( 0 ) > 99) || (ADC_get( 1 ) > 99)) 
        BEEP_PORT = 0;
    else 
        BEEP_PORT = 1;
}


void ADC_handler() 
{
    if (ms % 10 == 0) 
    {
        adc1 =  ADC_get( 0 );
        adc2 =  ADC_get( 1 );
        echo_ADC(adc1, adc2);
        beep_if_ADC_over99(adc1, adc2) ;
        LED7_setdigit(int(adc2/10)); // Вывод данных второго АЦП на семисигментный индикатор
    }
}

void change_time_n_date()
{ // установить новое время в микросхему DS1307 через UART-интерфейс
    unsigned char hour, minute, second;
    unsigned char day, month, year;

    UART_sendstring_flash(enterHourStr);
    hour = UART_receivevalue();
    UART_sendstring_flash(enterMinuteStr);
    minute = UART_receivevalue();
    UART_sendstring_flash(enterSecondStr);
    second = UART_receivevalue();

    DS1307_settime(hour, minute, second);

    UART_sendstring_flash(enterDayStr);
    day = UART_receivevalue();
    UART_sendstring_flash(enterMonthStr);
    month = UART_receivevalue();
    UART_sendstring_flash(enterYearStr);
    year = UART_receivevalue();

    DS1307_setdate(day, month, year);
    mode = 0; // тк нужно выполнить однократно сбрасываем индес режима
}

void display_time_n_date()
{ // вывести время и дату на ЖК дисплей
    if (ms % 250 == 0) // обновляем данные каждые 250 мс
    {
        LCD_clrscr();
        char time[9];
        char date[9];
        DS1307_gettime(time);
        DS1307_getdate(date);
        LCD_gotoxy(0, 0);
        LCD_print(time);
        LCD_gotoxy(1, 0);
        LCD_print(date);
    }
}

void mode_handler()
{
    if (mode == 1)
        change_time_n_date();
    if (mode == 2)
        display_time();
}

#pragma vector = TIMER1_COMPA_vect
__interrupt void Timer1_COMPA(void)
{
    ms_ctr++;
    mode_handler();
    ADC_handler();
}

#pragma vector = UART_RXC_vect
__interrupt void UART0_RX_interrupt()
{ // Прерывание по UART
    pc_keyboard_data = UART_receivevalue();
    UART_sendstring("sended: ");
    UART_sendvalue(pc_keyboard_data);
    UART_sendvalue('\n');

    if (pc_keyboard_data == '1')
        mode = 1;   //режим изменения даты и времени
    if (pc_keyboard_data == '2')
        mode = 2;   //режим отображения даты и времени
    if (pc_keyboard_data == '3')
    {               //отчистка дисплея и сброс индекса режима
        mode = 0;
        LCD_clrscr();
    }

}


int main()
{

    I2C_init();
    SPI_init();
    LCD_init();
    LCD_clrscr();
    init_ADC();

    UART_init(CALC_UBRR(57600));
    __enable_interrupt();

    init_timer1();
    UART_sendstring_flash(UartMessageTask4);
    OutputStartLCDMessage();

    while (1)
    {
    }
}
