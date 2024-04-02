#ifndef _PINDEF_H_
#define _PINDEF_H_

#include <iom16.h>
#include <ina90.h> 

#define BEEP_PORT			PORTB_PORTB4
#define BEEP_DDR			DDRB_DDB4

#define LCD_E_PORT			PORTD_PORTD2
#define LCD_E_DDR			DDRD_DDD2
#define LCD_RS_PORT			PORTD_PORTD3
#define LCD_RS_DDR			DDRD_DDD3
#define ST_PORT				PORTB_PORTB1
#define ST_DDR				DDRB_DDB1

#define SCL_PORT			PORTB_PORTB2
#define SCL_DDR				DDRB_DDB2
#define SDA_PORT			PORTB_PORTB3
#define SDA_PIN				PINB_PINB3
#define SDA_DDR				DDRB_DDB3

#define LED1_PORT			PORTD_PORTD4
#define LED1_DDR			DDRD_DDD4
#define LED2_PORT			PORTD_PORTD5
#define LED2_DDR			DDRD_DDD5
#define LED3_PORT			PORTD_PORTD7
#define LED3_DDR			DDRD_DDD7

#define KEY_PORT			PORTC
#define KEY_PIN				PINC
#define KEY_DDR				DDRC

#define DS_BUS_PORT			PORTB_PORTB0
#define DS_BUS_PIN			PINB_PINB0
#define DS_BUS_DDR			DDRB_DDB0

#define SPI_MISO_DDR		DDRB_DDB6
#define SPI_MOSI_DDR		DDRB_DDB5
#define SPI_SCK_DDR			DDRB_DDB7
#define SPI_SS_DDR			DDRB_DDB4
#define SPI_SS				PORTB_PORTB4

#define UART_UBRRL			UBRRL
#define UART_UBRRH			UBRRH
#define UART_UCSRB_RXEN		UCSRB_RXEN
#define UART_UCSRB_TXEN		UCSRB_TXEN
#define UART_UCSRB_RXCIE	UCSRB_RXCIE
#define UART_UCSRA_UDRE		UCSRA_UDRE
#define UART_UDR			UDR
#define UART_UCSRA_RXC		UCSRA_RXC

#define UART_RXC_vect		USART_RXC_vect

#endif
