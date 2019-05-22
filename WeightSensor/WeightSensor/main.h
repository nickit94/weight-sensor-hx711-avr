/*
 * main.h
 *
 * Created: 18.03.2019 18:38:49
 *  Author: Никита
 */ 


#ifndef MAIN_H_
#define MAIN_H_

#define F_CPU 14745600UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "UART.h"
#include "HX711.h"

enum
{
	ID0 = 0,
	ID1,
	DATA0,
	DATA1,
	DATA2,
	DATA3,
	MASK,
	CRC
};

/* Settings */
#define ID 103

/* Error codes RX */
#define FRAMING_ERROR (1 << FE0)
#define PARITY_ERROR  (1 << UPE0)
#define DATA_OVERRUN  (1 << DOR0)

/* Sizes */
#define SIZE_TX_BUFFER	  8
#define SIZE_RX_BUFFER	  10
#define COUNT_MEASURE	  15		// Кол-во замеров для усреднения

/* Ports and Pins */
#define PORT_DOUT	PORTD
#define PORT_SCK	PORTD
#define PORT_DIR	PORTD	//PORTB

#define DDR_DOUT	DDRD
#define DDR_SCK		DDRD
#define DDR_DIR		DDRD

#define PIN_DOUT	PIND
#define PIN_SCK		PIND

#define DOUT		PD5	//PD6
#define SCK			PD6	//PD7
#define DIR		    PD2	//PB1

/* Bit operations */
#define clr_bit(reg, bit)	     reg &= (~(1 << bit))
#define set_bit(reg, bit)	     reg |= (1 << bit)

#define BitIsSet(reg, bit)     ((reg & (1 << bit)) != 0)
#define BitIsClear(reg, bit)   ((reg & (1 << bit)) == 0)

/* Digital Write */
#define RECEIVE_DIR()	clr_bit(PORT_DIR, DIR)
#define TRANSMIT_DIR()	set_bit(PORT_DIR, DIR)

#define ENABLE_RX	UCSR0B =   ((1 << RXCIE0) | (1 << RXEN0))
#define ENABLE_TX	UCSR0B =   ((1 << TXEN0)  | (1 << UDRIE0))
//#define DISABLE_RX  UCSR0B &= (~((1 << RXCIE0) | (1 << RXEN0)))
//#define DISABLE_TX  UCSR0B &= (~((1 << TXEN0)  | (1 << UDRIE0)))

#define WAIT(x)		_delay_ms(x)

extern volatile uint8_t	tx_buffer[SIZE_TX_BUFFER];
extern volatile uint8_t	rx_buffer[SIZE_RX_BUFFER];
extern volatile uint8_t	rx_flag;
extern volatile uint8_t	tx_buffer_index;
extern volatile uint8_t	rx_buffer_index ;

#endif /* MAIN_H_ */