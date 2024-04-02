#ifndef _DELAY_H_
#define _DELAY_H_

#include "COMMON.h"

// Вспомогательная функция для выполнения задержки
void delay_func_mcs( unsigned short count);

// Вспомогательная функция для выполнения задержки
void delay_func_ms( unsigned short count);

// Данный макрос позволяет с большой точностью сделать паузу, 
// длинной в заданное количество микросекунд
// Минимальное значение паузы - 24 цикла = 24 * TACT_TIME_MCS мкс
// Например при частоте генератора 14.7456 - минимальное значение 2 мкс
#define DELAY_MCS( mcs ) delay_func_mcs( ( mcs - 17 * TACT_TIME_MCS ) / ( 6 * TACT_TIME_MCS ) );

// Точность данной функции +- 0.1 мс 
// Погрешность связана с ошибкой округления числа до целого
#define DELAY_MS( ms ) delay_func_ms( ( 1000.0 * ms - 28 * TACT_TIME_MCS ) / ( 1472 * TACT_TIME_MCS ) );

#endif
