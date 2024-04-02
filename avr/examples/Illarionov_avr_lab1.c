#include "DELAY.H"
#include "I2C.H"
#include "KEYBOARD.H"
#include "LCD_HD44780.H"
#include "SHIFT.H"
#include "SPI.H"
#include "UART.H"
#include "PINDEF.h"

char x = 0, y = 0;                                                     // х y на LCD
char dx = 1, dy = 1;                                                   // смещение x и y
char sym = '*';                                                        // символ для отрисовки анимации

void move() {                                                          // функция перемещения символа по LCD
  x += dx;                                                             // Двигаем х
  if (x <= 0 || x >= LCD_WIDTH - 1) {                                  // Если достигли границ по х
    dx *= -1;                                                          // изменяем направление по х
    y += dy;                                                           // смещаея по у

    if (y <= 0 || y >= 1) {                                            // Если вышли за границы по у
      dy *= -1;                                                        // меняем направление
    }
  }
}

void draw() {                                                          // Отрисовка символа по координатам на LCD
  LCD_clrscr();                                                        // Очистка LCD
  LCD_gotoxy(x, y);                                                    // Смещение
  LCD_printchar(sym);                                                  // Рисование
}

#pragma vector = \
TIMER1_COMPB_vect 
__interrupt void handleDrawTimer() {                                   // Прерывание по компаратору B для анимации
  move();                                                              // Измененеие координат
  draw();                                                              // Отрисовка
}

char beeping = 0;
char beep_duration = 0;
void beep_handle(void) {                                               // Обработка зумера
  if(beeping) {
    BEEP_DDR = 1;
    BEEP_PORT = 0;
    if(beep_duration > 0) {
      beep_duration -= 100; 
    } else {
      beeping = 0;
      BEEP_PORT = 1;
    }
  } else {
      BEEP_PORT = 1;
  }
}

char key = 0;                                                          // код кнопки на клавиатуре
void keyboard_handle(void) {                                           // Обработка клавиатуры
  key = KEY_getkey();                                                  // Получить код нажатой кнопкт
  if (!key) return;                                                    // выйти если не нажимали
  switch (key) {
    case KEY_A: {                                                      // Если нажали А
      beeping = 1;                                                     // Попищать
      beep_duration = 200;
      TIMSK_OCIE1B |= 0;                                               // Остановить анимацию

      break;
    }

    case KEY_B: {                                                      // Если нажали В
      beeping = 1;                                                     // Попищать
      beep_duration = 200;
      TIMSK_OCIE1B |= 1;                                               // Запустить анимацию

      break;
    }
  }
}


#pragma vector = \
TIMER1_COMPA_vect                                                      // Прерывание по компаратору A для обработки событий
__interrupt void handleKeyboardTimer() {
  keyboard_handle();
  beep_handle();
}


void initTimer() {                                                     // Настройка таймера
  TCCR1B = 0;                                                          // Очистить регистр управления таймером 1
  TCCR1B |= (1 << TCCR1B_WGM12);                                       // Режим CTC
  TCCR1B |= (1 << TCCR1B_CS12) | (1 << TCCR1B_CS10);                   // Предделитель 1024

  OCR1A = 1562;                                                        // Значение для достижения 100 мс компаратор А
  OCR1B = 195;                                                         // Значение для достижения 125 мс копаратор В
  TIMSK_OCIE1A |= 1;                                                   // Запуск компаратора таймера А
  TIMSK_OCIE1B |= 1;                                                   // Запуск компара таймера В
}

#pragma vector = UART_RXC_vect                                         // Прерывание по UART
__interrupt void UART0_RX_interrupt() {
  sym = UART_receivevalue();                                           // Считать сивол для отрисовки анимации
  UART_sendstring("Sym: ");                                            // Эхо обратно в терминал
  UART_sendvalue(sym);
  UART_sendvalue('\n');
}

int main() {
  LED1_DDR = 1;                                                        // Настройка портов на выход
  LED2_DDR = 1;
  LED3_DDR = 1;

  I2C_init();                                                          // инициализация i2c
  SPI_init();                                                          // инициализация SPI
  LCD_init();                                                          // инициализация LCD
  LCD_clrscr();                                                        // ОЧиска LCD

  UART_init(CALC_UBRR(57600));                                         // инициализация UART
  __enable_interrupt();                                                // инициализация прерываний
  initTimer();                                                         // Запуск таймера

  DELAY_MS(100);                                                       // Задерка

  while (1) {
                                                                       // loop
  }
}
