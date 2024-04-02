#ifndef _DELAY_H_
#define _DELAY_H_

void delay_func_mcs( int count);
void delay_ms( int count );

#define CLOCK_FREQUENCY		22.1184
#define TACT_TIME_MCS		( 12.0 / CLOCK_FREQUENCY )

// ƒанный макрос позвол€ет с большой точностью сделать паузу, 
// длинной в заданное количество микросекунд
// ћинимальное значение паузы - 20 циклов = 20 * TACT_TIME_MCS мкс
// Ќапример при частоте генератора 22.1184 - минимальное значение 11 мкс
#define DELAY_MCS( mcs ) delay_func_mcs( ( mcs - 20 * TACT_TIME_MCS ) / ( 16 * TACT_TIME_MCS ) );

#endif
