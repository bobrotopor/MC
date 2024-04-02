/*#ifndef _DS1820_H_
#define _DS1820_H_

#include "PINDEF.H"

unsigned char DS1820_start();


bit DS1820_readbit();
void DS1820_writebit(bit bitData);
unsigned char DS1820_readbyte();
void DS1820_writebyte(unsigned char out);
bit DS1820_readtemperature(unsigned char * buff);
bit DS1820_gettemperature( float* pTemp );

#endif	    */


#ifndef _DS1820_H_
#define _DS1820_H_

#include "PINDEF.H"

// Функция сброса
// Возвращает 1 если на шине 1-Wire присутствует устройство
// 			  0 если устройств нет
unsigned char DS1820_reset();

// Функция записи/чтения байта линии 1-Wire
// Для чтения необходимо передать в качестве параметра 0xFF
unsigned char DS1820_rw( unsigned char tdata );

// Функция старта преобразования температуры
unsigned char DS1820_startconverttemp();

// Функция проверки статуса преобразования температуры
unsigned char DS1820_getconvertstatus();

// Функция чтения данных из памяти микросхемы DS1820
// Размер буфера должен быть равен 9
unsigned char DS1820_readdata(unsigned char * buff);

// Функция получения значения температуры
// Возвращает 0 если сенсор не подключен
// Возвращает 1 если данные не готовы(преобразование не завершилось)
// Возвращает 2 если операция успешна
unsigned char DS1820_gettemperature( char strTemp[] );

#endif

