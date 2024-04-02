#ifndef _DS1307_H_
#define _DS1307_H_

#include "PINDEF.H"

void DS1307_write(unsigned char addr, unsigned char value);
unsigned char DS1307_read(unsigned char addr);
void DS1307_init();

void DS1307_gettime(char* timestr);		// Длина строки для времени должна быть равна 9
void DS1307_getdate(char* datestr);

void DS1307_settime( unsigned char hour, unsigned char minute, unsigned char second );
void DS1307_setdate( unsigned char day, unsigned char month, unsigned char year );


#endif