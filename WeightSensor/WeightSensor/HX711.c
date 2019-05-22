/*
 * HX711.c
 *
 * Created: 07.02.2018 23:22:35
 *  Author: Никита
 */ 

 #include "HX711.h"

 void HX711_Init()
 {
	 /* SCK - OUTPUT, DATA - INPUT */
	 set_bit(DDR_SCK, SCK);
	 clr_bit(DDR_DOUT, DOUT);
	 
	 /* DATA - OUTPUT 1 (PULLUP), SCK - OUTPUT 1 */
	 set_bit(PORT_DOUT, DOUT);
	 set_bit(PORT_SCK, SCK);

	 HX711_Read();
	 WAIT(10);
 }

 int32_t HX711_Read()
 {
	 int32_t adc_value = 0;
	 clr_bit(PORT_SCK, SCK);

	 for (int i = 0; i < 24; i++)
	 {
		 set_bit(PORT_SCK, SCK);
		 adc_value <<= 1;
		 clr_bit(PORT_SCK, SCK);
		 
		 if (BitIsSet(PIN_DOUT, DOUT)) adc_value++;
	 }

	 /* CHANEL A, GAIN 128 */
	 set_bit(PORT_SCK, SCK);
	 adc_value = adc_value ^ 0x800000;
	 clr_bit(PORT_SCK, SCK);
	 
	 return adc_value;
 }