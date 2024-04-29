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
Группа:		СМ7-74Б
Формулировка задачи:
1) Вывести название режима - ClockMode на LCD дисплей;
2) Выводить текущее время на LCD дисплей, полученное из микросхемы DS1307 
   (информация на дисплее обновляетя каждые 250 мс - 4 Гц);						
3) При нажатии # клавиатуры стенда включается режим изменения времени - пользователю нужно последовательно
   через клавиатуру ПК ввести час, минуты, секунды;
4) После изменения текущего времени снова включается режим отображения.
*/

#define DISPLAY_DELAY 500 // [мс], время отображения одного кадра на LCD дисплее   

#define FALSE 0
#define TRUE 1

flash char UartMessageClockMode[] = "ClockMode\r\n";

flash char timeStr[] = "Time: ";
flash char enterNewTimeStr[] = "Enter new time: ";
flash char enterHourStr[] = "Enter hour: ";
flash char enterMinuteStr[] = "Enter minute: ";
flash char enterSecondStr[] = "Enter second: ";

char is_time_change_mode = FALSE;	// флаг режима изменения времени
char button_read_flag = FALSE;		// флажок чтения адресов кнопок клавиатуры стенда
unsigned char key = 0;				// ключ символа нажатой кнопки клавиатуры
unsigned char prev_key = 0; // Адрес клавиши на прошлом тике опроса клавиатуры
unsigned int ms_ctr = 0;			// счетчик миллисекунд от начала работы программы
unsigned int button_ms_counter = 0; // счетчик миллисекнуд для исключения дребезга контактов

// установить новое время в микросхему DS1307 через UART-интерфейс 
void set_new_time()
{

    unsigned char hour, minute, second;
    UART_sendstring_flash( enterHourStr );
	hour = UART_receivevalue();	
	UART_sendstring_flash( enterMinuteStr );
	minute = UART_receivevalue();	
	UART_sendstring_flash( enterSecondStr );
	second = UART_receivevalue();	

    DS1307_settime( hour, minute, second );
	is_time_change_mode = FALSE;

}

// вывести текущее время на LCD дисплей
void display_time()
{
    LCD_clrscr();
    char time[9];
    DS1307_gettime(time); // получить текущее вермя в массив time

    UART_sendstring_flash( timeStr );
	UART_sendstring( time ); // вывести время в консоль
	LCD_gotoxy( 0, 0 );
	LCD_print(time);		// вывести время на ЖК дисплей
}


void init_timer1() 
{// Настройка таймера 1                                

	// Настройка компоратора B для оргнаизации миллисекундного таймера 1000 Гц (1 мс)
	// и обновления дисплея с частотой 40 Гц (25 мс)
	TCCR1B = 0;   									     // Очистить регистр управления компаратором 1В
	TCCR1B |= (1 << TCCR1B_WGM12); 					     // устанавливаем режим СТС (сброс по совпадению)                                                   
	TCCR1B |= (1 << TCCR1B_CS11);   					 // Предделитель 8	

	OCR1A = 1844;                                        // Значение для достижения 1000 Гц (1 мс) компаратор А
	OCR1B = 46080;                                       // Значение для достижения 40 Гц (25 мс) копаратор В
	TIMSK_OCIE1A |= 1;                                   // Запуск компаратора таймера А
	TIMSK_OCIE1B |= 1;                                   // Запуск компаратора таймера В
}

#pragma vector = TIMER1_COMPA_vect
__interrupt void Timer1_COMPA(void)
{
	if (ms_ctr % 10 == 0)
		button_handler();	// читаем данные с клавиатуры раз в 10 мс

	if (is_time_change_mode == TRUE) {
		set_new_time();
		is_time_change_mode == FALSE;
	}
	ms_ctr++;
}

#pragma vector = TIMER1_COMPB_vect
__interrupt void Timer1_COMPB(void)
{
	if ( is_time_change_mode == FALSE ) // выводим текущее время
		display_time();					// если не находимя в режиме изменения времени
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
			button_ms_counter = ms_counter;
			return;
		}

		if (button_read_flag == TRUE && ms_counter - button_ms_counter > 20)
			button_read_flag = FALSE;
		else
			return;

		if (key == KEY_BAR) //если нажата # активируем флажок изменения времени
			is_time_change_mode = TRUE;

		prev_key = key;
	}
}


int main()
{
	I2C_init();
	SPI_init();
	LCD_init();
	LCD_clrscr();

	UART_init( CALC_UBRR( 9600 ) );
	__enable_interrupt();
	
	UART_sendstring_flash( UartMessageClockMode );
	OutputStartLCDMessage();

	while(1) 
	{
	}
}
