#ifndef _DS1307_H_
#define _DS1307_H_

#include "PINDEF.H"

// Функция записи значения ячейки микросхемы DS1307
void DS1307_write(unsigned char addr, unsigned char value);

// Функция чтения значения ячейки микросхемы DS1307
unsigned char DS1307_read(unsigned char addr);

// Функция получения значения времени
// Длина строки timestr должна быть равна 9
void DS1307_gettime(char* timestr);

// Функция задания значения времени
void DS1307_settime( unsigned char hour, unsigned char minute, unsigned char second );

// Функция получения значения даты
// Длина строки datestr должна быть равна 9
void DS1307_getdate(char* datestr);

// Функция задания значения даты
void DS1307_setdate( unsigned char day, unsigned char month, unsigned char year );

#endif
