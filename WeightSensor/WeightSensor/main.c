/*
 * main.c
 *
 * Created: 07.02.2018 22:15:30
 * Author : Никита
*/

#include "main.h"

volatile uint8_t	tx_buffer[SIZE_TX_BUFFER];
volatile uint8_t	rx_buffer[SIZE_RX_BUFFER];
volatile uint8_t	rx_flag         = 0;
volatile uint8_t	tx_buffer_index = 0;
volatile uint8_t	rx_buffer_index = 0;
static   int32_t	sum_measure = 0;			// Сумма всех замеров
static   uint8_t	count_measure = 0;			// Кол-во текущих замеров

void Initialization();
void PrepareTxBuffer(uint32_t data);
uint8_t CRC8(uint8_t *buffer, uint8_t lenght);

int main()
{
	Initialization();
	sei();
	
	while (1)
	{
		/* Если HX711 готов к чтению данных его АЦП - читаем */
		if (BitIsClear(PIN_DOUT, DOUT))
		{
			sum_measure += HX711_Read();
			count_measure++;
		}

		/* Если сделали кол-во замеров = COUNT_MEASURE, то формируем буфер для отправки */
		if (count_measure == COUNT_MEASURE)
		{
			sum_measure	= (int32_t)(sum_measure / COUNT_MEASURE);
			PrepareTxBuffer(sum_measure);

			sum_measure = 0;
			count_measure = 0;
		}

		if (rx_flag)
		{
			rx_flag = 0;

			if (rx_buffer[0] == ID)
			{
				tx_buffer_index = 0;

				/* Разрешение передачи по RS485 и 
				 * Разрешение отправки и прерывания по UART */
				TRANSMIT_DIR();
				ENABLE_TX;
			}
		}
	}
}

void Initialization()
{
	WAIT(100);
	set_bit(DDR_DIR, DIR);

	RECEIVE_DIR();
	UART_Init();
	HX711_Init();
}

uint8_t CRC8(uint8_t *buffer, uint8_t lenght)
{
	uint8_t crc = 0;

	for (uint8_t i = 0; i < lenght; i++)
	{
		uint8_t tmp = buffer[i];

		for (uint8_t j = 0; j < 8; j++)
		{
			if ((tmp ^ crc) & 0x01)
			{
				crc >>= 1;
				crc ^= 0x0c;
				crc |= 0x80;
			}
			else
			{
				crc >>= 1;
			}
			tmp >>= 1;
		}
	}

	return crc;
}

void PrepareTxBuffer(uint32_t data)
{
	tx_buffer[ID0]   = (uint8_t)(ID & 0xFF);					// Младший байт адреса контроллера;
	tx_buffer[ID1]   = (uint8_t)(ID >> 8);						// Старший байт адреса контроллера; 18 19 132
	tx_buffer[DATA0] = (uint8_t)(data & 0x000000FF);			// Биты 0…7 32-битного измеренного значения;
	tx_buffer[DATA1] = (uint8_t)((data & 0x0000FF00) >> 8);		// Биты 8…15 32-битного измеренного значения;
	tx_buffer[DATA2] = (uint8_t)((data & 0x00FF0000) >> 16);	// Биты 16…23 32-битного измеренного значения;
	tx_buffer[DATA3] = (uint8_t)((data & 0xFF000000) >> 24);	// Биты 24…31 32-битного измеренного значения;
	tx_buffer[MASK]  = 0;										// Битовая маска признаков измерения;
	tx_buffer[CRC]   = CRC8(tx_buffer, 7);						// Контрольная сумма (CRC8), вычисленная по байтам пакета от Байт1 до Байт7;
}