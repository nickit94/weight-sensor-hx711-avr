/*
 * UART.c
 *
 * Created: 07.02.2018 23:31:54
 *  Author: Никита
 */ 

 #include "UART.h"

void UART_Init()
{
	UBRR0H = 0;																	/* Baud rate 19200			: 0000 0000 */
	UBRR0L = 47;//95;															/* Baud rate 19200			: 0011 0011 */
	ENABLE_RX;																	/* ICR_RX, RX, TX	        : 1001 1000 */
	UCSR0C =  (1 << UCSZ01) | (1 << UCSZ00);				/* Asynchronous USART, 8 bit, 1 stop bit		: 0000 0110 */
}

/* Прерывание на прием данных */
ISR(USART_RX_vect)
{
	char status = UCSR0A;
	char data   = UDR0;

	if (!(status & (FRAMING_ERROR | PARITY_ERROR | DATA_OVERRUN)))
	{
		rx_buffer[0] = data;
		rx_flag = 1;
	} 
}

/* Прерывание по опустошениею UDR (готовность отправки) */
ISR (USART_UDRE_vect)
{
	if (tx_buffer_index <= SIZE_TX_BUFFER)
	{
		UDR0 = tx_buffer[tx_buffer_index];
	}
	else
	{
		/* Разрешение приема и прерывания приема UART
		 * Переключение RS485 в режим прослушки */
		ENABLE_RX;
		RECEIVE_DIR();
	}

	tx_buffer_index++;
}