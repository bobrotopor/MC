/*#include "DS1820.h"
#include "DELAY.h"

bit DS1820_reset()				// 0 = presence, 1 = no part
{
	bit presence;
	OW_DATABIT = 0; 			// pull OW_DATABIT line low
	DELAY_MCS(500);
	OW_DATABIT = 1; 			// allow line to return high
	DELAY_MCS(70); 				// wait for presence 55 uS
	presence = OW_DATABIT; 		// get presence signal
	DELAY_MCS(320); 				// wait for end of timeslot 316 uS 
	return !presence; 			// presence signal returned
} 							

//-----------------------------------------
// Read one bit from DS1820
//-----------------------------------------
bit DS1820_readbit()
{
	OW_DATABIT = 0; 					// pull OW_DATABIT low to start timeslot
	OW_DATABIT = 1;
	DELAY_MCS( 20 );
	return OW_DATABIT;	 				// return value of OW_DATABIT line
}

//-----------------------------------------
// Write one bit to DS1820
//-----------------------------------------
void DS1820_writebit(bit bitData)
{
    OW_DATABIT=0;	
	OW_DATABIT = bitData ? 1 : 0;
	DELAY_MCS(40); 			
	OW_DATABIT = 1;
}

//-----------------------------------------
// Read 1 byte from DS1820
//-----------------------------------------
unsigned char DS1820_readbyte()
{
	unsigned char i;
	unsigned char datain = 0;
	for ( i = 0; i < 8; i++ )
	{
		datain |= DS1820_readbit() ? ( 0x01 << i ) : datain;
		DELAY_MCS( 120 ); 
	}
	return datain;
}

//-----------------------------------------
// Write 1 byte
//-----------------------------------------
void DS1820_writebyte(unsigned char out)
{
	unsigned char i;
	for ( i = 0; i < 8; i++ ) // writes byte, one bit at a time
	{	    
		DS1820_writebit( ( bit )( out & 0x1 ) ); 		// write bit in temp into
		out = out >> 1;
	}
	DELAY_MCS(110);
}

//-----------------------------------------
// Read temperature
//-----------------------------------------
bit DS1820_readtemperature(unsigned char* buff)
{
	unsigned char n;
	bit oldEA = EA;

	EA = 0;								// disable all interrupt
	if( !DS1820_reset() )
		return 0;

    DS1820_writebyte(0xcc);  			// skip ROM
    DS1820_writebyte(0x44);  			// perform temperature conversion
    while (DS1820_readbyte()==0xff); 	// wait for conversion complete	
    DS1820_reset();
    DS1820_writebyte(0xcc);  			// skip ROM
    DS1820_writebyte(0xbe);  			// read the result
    
    for (n = 0; n < 9; n++)     		// read 9 bytes but, use only one byte
    {
       buff[n] = DS1820_readbyte();  		// read DS1820
    }
	EA = oldEA;

	return 1;
}

bit DS1820_gettemperature( float* pTemp )
{
	unsigned char buff[ 9 ];
	unsigned short temperature;

	if( !DS1820_readtemperature( buff ) )
		return 0;

	temperature = buff[ 1 ];
	temperature = temperature << 8 + buff[ 2 ];

	*pTemp = temperature;
	*pTemp /= 2;


	return 1;
}
*/
/*
#include "DS1820.h"
#include "DELAY.h"
#include "CONVERT.H"
#include "stdio.h"

// Функция сброса
// Возвращает 1 если на шине 1-Wire присутствует устройство
// 			  0 если устройств нет
unsigned char DS1820_reset()				
{
	OW_DATABIT = 1;
	//DELAY_MCS( 6 );
	delay_func_mcs(6);
	OW_DATABIT = 0;
	DELAY_MCS( 530 );
	OW_DATABIT = 1;
	DELAY_MCS( 80 );
	if( OW_DATABIT )
		return 0;
	else
	{
		DELAY_MCS( 420 );
		return 1;
	}
} 							

// Функция записи/чтения байта линии 1-Wire
// Для чтения необходимо передать в качестве параметра 0xFF
unsigned char DS1820_rw( unsigned char tdata )
{
	register unsigned char i;

	OW_DATABIT = 1;	
	for ( i = 0; i <= 7; i++ ) 
	{
		OW_DATABIT = 0;
		//DELAY_MCS( 2 );
		delay_func_mcs(2);
		
		if( tdata & 0x01 )
			OW_DATABIT = 1;
		else
			OW_DATABIT = 0;

	    //DELAY_MCS( 8 );
		delay_func_mcs(8);

		tdata >>= 1;

		if( OW_DATABIT )
			tdata |= ( 1 << 7 );
		else
			tdata &= ~( 1 << 7 );
				
		DELAY_MCS( 55 );
		OW_DATABIT = 1;
	//	DELAY_MCS( 6 );
	    delay_func_mcs(6);
	}

	return tdata; 
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
	unsigned char intLen;

	if( !DS1820_getconvertstatus() )
		return 1;

	if( !DS1820_readdata( buff ) )
		return 0;
	
	temperature = buff[ 1 ];
	temperature <<= 8;
	temperature = buff[ 0 ];

	printf( "%d\n", temperature );
	printf( "%d\n", sizeof(char) );


	intLen = ShortToString( temperature / 2, strTemp );
	strTemp[ intLen ] = '.';
	strTemp[ intLen + 1 ] = ( temperature & 0x01 ) ? '5' : '0';
	strTemp[ intLen + 2 ] = 0;

	return 2;
}

*/


#include "DS1820.h"
#include "DELAY.h"
#include "CONVERT.H"

// Функция сброса
// Возвращает 1 если на шине 1-Wire присутствует устройство
// 			  0 если устройств нет
unsigned char DS1820_reset()
{
	bit presence;
	OW_DATABIT = 0; 			// pull OW_DATABIT line low
	DELAY_MCS(500);
	OW_DATABIT = 1; 			// allow line to return high
	DELAY_MCS(70); 				// wait for presence 55 uS
	presence = OW_DATABIT; 		// get presence signal
	DELAY_MCS(320); 				// wait for end of timeslot 316 uS 
	return !presence; 			// presence signal returned
}

// Функция записи/чтения байта линии 1-Wire
// Для чтения необходимо передать в качестве параметра 0xFF
unsigned char DS1820_rw( unsigned char byte )
{
	register unsigned char i;

	OW_DATABIT = 1;
	for ( i = 0; i <= 7; i++ ) 
	{
		OW_DATABIT = 0;
		
		if( byte & 0x01 )
			OW_DATABIT = 1;
		else
			OW_DATABIT = 0;

		DELAY_MCS( 11 );

		byte >>= 1;

		if( OW_DATABIT )
			byte |= ( 1 << 7 );
		else
			byte &= ~( 1 << 7 );
				
		DELAY_MCS( 55 );
		OW_DATABIT = 1;
	}

	return byte; 
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
// Размер буфера должен быть равен 9
unsigned char DS1820_readdata(unsigned char * buff)
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
// Возвращает 0 если сенсор не подключен
// Возвращает 1 если данные не готовы(преобразование не завершилось)
// Возвращает 2 если операция успешна
unsigned char DS1820_gettemperature( char strTemp[] )
{
	unsigned char buff[ 9 ];
	unsigned short temperature;
	unsigned char intLen;

	if( !DS1820_getconvertstatus() )
		return 1;

	if( !DS1820_readdata( buff ) )
		return 0;
	
	temperature = buff[ 1 ];
	temperature <<= 8;
	temperature = buff[ 0 ];

	intLen = ShortToString( temperature / 2, strTemp );
	strTemp[ intLen ] = '.';
	strTemp[ intLen + 1 ] = ( temperature & 0x01 ) ? '5' : '0';
	strTemp[ intLen + 2 ] = 0;

	return 2;
}

