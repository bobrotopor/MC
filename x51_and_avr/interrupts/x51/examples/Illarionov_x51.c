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

char isLED = 0;                                     // �������� �� ��������� �����������
char isBEEP = 0;                                    // �������� �� ��������� �������
char isIND = 1;                                     // �������� �� ��������� 7-�� ���������

void setLED(char state) {                           // ������� ������������ �����������
  if (isLED) {                                      // �������� �� ��������� �����������
    LED1 = state;                                   // ����������� ��������� �����������
    LED2 = state;
    LED3 = state;
  } else {                                          // �����
    LED1 = 0;                                       // ��������� �����
    LED2 = 0;
    LED3 = 0;
  }
}

void setBEEP(char state) {                          // ������� ������������ ������
  if (isBEEP) {                                     // �������� �� ��������� �������
    BEEP_BIT = state;                               // ����������� ���������
  } else {                                          // �����
    BEEP_BIT = 0;                                   // ��������� �����
  }
}

void setIND(unsigned char digit) {                  // ������� ������������ 7-�� �����������
  if (isIND) {                                      // �������� �� ��������� 7-�� ���������
    LED7_setdigit(digit);                           // ����������� ���������
  } else {                                          // �����
    LED7_setdigit(0);                               // ���������� 0
  }
}

void initTimer50ms(void) {                          // ������� ��������� ������� �� 50��
  unsigned int counts;                              // �������� ��� ������� � 50 ����������� (20 Hz)
  TMOD |= 0x01;                                     // ����� 1: 16-������ ������

  counts = (unsigned int)(50.0 * 1000.0 /
                          TACT_TIME_MCS);           // ������������ �������� ��� �������
                                                    // � 50 ����������� (20 Hz)

  TH0 = 0xFFFF - counts + 1;                        // �������� ��� �������� �����
  TL0 = 0xFFFF - counts + 1;                        // �������� ��� �������� �����

  ET0 = 1;                                          // Бит разрешения прерывания от таймера 0
  TR0 = 1;                                          // Бит управления таймера 0 для пуска/останова таймера/счетчика 
}

const int BLINK_TIMER_T = 50;                       // ������ �� �������� �������� ���������
volatile char enabled = 0;                          // �������� �� ���������
volatile long duration =
    0;                                              // ����������������� ��������� ��� ���������� �� �������
void blinkTimer_handle(void) {                      // ������� ��������� "����������" ��� �������� ���������
  if (enabled) {                                    // ���� ��������� ��������
    setBEEP(1);                                     // �������� �����
    setLED(1);                                      // �������� �����
    if (duration > 0) {                             // ���� ��� �� ���� ���������
      duration -= BLINK_TIMER_T;                    // �������� �����, ������� ��� ����������
    } else {                                        // �����
      enabled = 0;                                  // ��������� � ���������� ����
      setBEEP(0);                                   // �����
      setLED(0);                                    // �����
    }
  } else {                                          // ���� ��������� ���������
    setBEEP(0);                                     // ���������� �����
    setLED(0);                                      // �����
  }
}

const char title[] = "LED BEEP INDIC";              // ��������� ��� LED
const char on[] = "On";                             // ����������� �� ������ ���� ���� ��������
const char off[] = "Off";                           // ����������� �� ������ ���� ���� �� ��������
void update_LCD() {                                 // ������� ����������� ��������� �� ������
  LCD_clrscr();                                     // �������� LCD
  LCD_print(title);                                 // �������� ���������
  LCD_gotoxy(0, 1);                                 // ����� ������
  if (isLED) {                                      // ���� ��������� ������� ��������
    LCD_print(on);                                  // �� ����� ON
    LCD_print("  ");                                // ���������
  } else {                                          // ����� �� ����� OFF
    LCD_print(off);
    LCD_print(" ");                                 // ���������
  }

  if (isBEEP) {                                     // ���� ��������� ������� ��������
    LCD_print(on);                                  // ����������
    LCD_print("   ");
  } else {
    LCD_print(off);
    LCD_print("  ");
  }

  if (isIND) {                                      // ���� ��������� ����������� ��������
    LCD_print(on);                                  // ����������
  } else {
    LCD_print(off);
  }
}

char key = 0;                                       // ��� ������� ������
void keyboardTimer_handle(void) {                   // ������� ��������� ������� �� ����������
  key = KEY_getkey();                               // ��������� ���� ������
  if (!key) return;                                 // ������ �� ������ - �������

  switch (key) {
    case KEY_A: {                                   // ��������� ��������� ������������ ������� �
      isLED = !isLED;
      break;
    }
    case KEY_B: {                                   // ��������� ��������� ������� ������� �
      isBEEP = !isBEEP;
      break;
    }
    case KEY_C: {                                   // ��������� ��������� 7 ����������� ������� �
      isIND = !isIND;
      break;
    }
  }
  update_LCD();                                     // �������� ��������� ��������� �� LCD
}

volatile unsigned char tick = 0;                    // ������� ����������� �����
int blink_duration = 100;                           // ����������������� ��������� ���� ��
int bpm = 120;                                      // �������� ��������� ��/���
unsigned char signature = 4;                        // ����������� ������ ����� ��/����
void metronomeTimer_handle(void) {                  // ������� ��������� ����
  setIND(tick % signature + 1);                     // �������� ����� ���� �� ����������
  enabled = 1;                                      // �������� ���������
  duration = blink_duration;                        // ��  blink_duration ��
  ++tick;
}

int bpm2tick(int bpm) {                             // ������� ������� bpm -> timer_ticks_count_50ms
  return 60000 / bpm / 50;
}                                                   // ��������� ��/��� � ������(��) / ������_�������(50��)

volatile int timer50ms_counter = 0;                 // ������� ������� �� ��
void timer50ms_ISR(void) interrupt 1 using 0 {      // ������� ���������� �������
  blinkTimer_handle();                              // 50��: ��������� ��������� ���������

  if (timer50ms_counter % 2 == 0) {                 // 100��: ��������� �����
    keyboardTimer_handle();
  }

  if (timer50ms_counter % bpm2tick(bpm) ==
      0) {                                          // bpm -> ��: ������� �� ��������� ��� �����
    metronomeTimer_handle();
  }

  ++timer50ms_counter;
}

void initUART() {                                   // ������� ������������� UART
  SCON = 0x50;                                      // 8-������� UART
  TMOD = 0x20;                                      // ������ 1: 8-������� �����, ����-���������������
  TH1 = 250;                                        // ������ ��������� �������� ������� ��� �������� 9600 ���
  TL1 = 250;
  TR1 = 1;                                          // ��������� ������
  TI = 1;
  ES = 1;                                           // ��������� ���������� �� UART
}

void UART_ISR(void) interrupt 4 using 0 {           //�������  ��������� UART ����������
  if (RI) {
    scanf("%i", &blink_duration);                   //
    scanf("%i", &bpm);
    scanf("%i", &signature);
    printf("Blink duration is %i ms\nBPM: %i\nMusic takts: %i\n",
           blink_duration, bpm, signature);
  }
}

void main() {                                       // ������� Main
  P0 = 0xFF;                                        // ��������� ������ �� �����
  P1 = 0xFF;
  P2 = 0xFF;
  P3 = 0xFF;

  printf("                -=  STEND RK-10  =-\n");  // ��������� � ��������
  printf("       Hardware ver. 1.0, Firmware ver. 1.0\n");

  DELAY_MCS(174);                                   // ��������

  SPI_init();                                       // ������������� SPI

  I2C_init();                                       // ������������� I2C

  LCD_init();                                       // ������������� LCD
  LCD_clrscr();                                     // ������� LCD

  initTimer50ms();                                  //  ������������� �������

  while (1) {
                                                    // Loop
  }
}
