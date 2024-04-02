#include "DS1820.h"
#include "DELAY.h"
#include "CONVERT.H"

// Функция сброса
// Возвращает 1 если на шине 1-Wire присутствует устройство
// 			  0 если устройств нет
unsigned char DS1820_reset()				
{
	DS_BUS_PORT = 0;
	DS_BUS_DDR = 0;
	DELAY_MCS( 6 );
	DS_BUS_DDR = 1;
	DELAY_MCS( 500 );
	DS_BUS_DDR = 0;
	DELAY_MCS( 80 );
	if( DS_BUS_PIN )
		return 0;
	else
	{
		DELAY_MCS( 420 );
		return 1;
	}
} 							

// Функция записи/чтения байта линии 1-Wire
// Для чтения необходимо передать в качестве параметра 0xFF
unsigned char DS1820_rw( unsigned char data )
{
	register unsigned char i;

	DS_BUS_PORT = 0;
	DS_BUS_DDR = 0;	
	for ( i = 0; i <= 7; i++ ) 
	{
		DS_BUS_DDR = 1;
		DELAY_MCS( 2 );
		
		if( data & 0x01 )
			DS_BUS_DDR = 0;
		else
			DS_BUS_DDR = 1;

		DELAY_MCS( 8 );

		data >>= 1;

		if( DS_BUS_PIN )
			data |= ( 1 << 7 );
		else
			data &= ~( 1 << 7 );
				
		DELAY_MCS( 55 );
		DS_BUS_DDR = 0;
		DELAY_MCS( 6 );
	}

	return data; 
}

// Функция старта преобразования температуры
unsigned char DS1820_startconverttemp()
{
	if( !DS1820_reset() )		// Если на шине никого нет
		return 0;			

    DS1820_rw( 0xcc );  		// Пропускаем ROM
    DS1820_rw( 0x44 );  		// Выводим команду преобразования температуры
	
	return 1;
}

// Функция проверки статуса преобразования температуры
unsigned char DS1820_getconvertstatus()
{
	if( DS1820_rw( 0xFF ) != 0x00 )	// Проверяем состояние линии
		return 1;
	else
		return 0;
}

// Функция чтения данных из памяти микросхемы DS1820
unsigned char DS1820_readdata(unsigned char* buff)
{
	unsigned char n;

	if( !DS1820_reset() )
		return 0;

    DS1820_rw( 0xcc );  			// Пропускаем ROM
    DS1820_rw( 0xbe );  			// Выводим функцию чтения данных памяти
    
    for (n = 0; n < 9; n++)     	// Читаем 9 байт памяти
       buff[ n ] = DS1820_rw( 0xFF ); 

	return 1;
}

// Функция получения значения температуры
unsigned char DS1820_gettemperature( char strTemp[] )
{
	unsigned char buff[ 9 ];
	signed short temperature;

	if( !DS1820_getconvertstatus() )
		return 1;

	if( !DS1820_readdata( buff ) )
		return 0;
	
	temperature = buff[ 1 ];
	temperature <<= 8;
	temperature = buff[ 0 ];

	unsigned char intLen = ShortToString( temperature / 2, strTemp );
	strTemp[ intLen ] = '.';
	strTemp[ intLen + 1 ] = ( temperature & 0x01 ) ? '5' : '0';
	strTemp[ intLen + 2 ] = 0;

	return 2;
}
