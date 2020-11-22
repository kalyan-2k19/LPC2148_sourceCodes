#include "LCD.h"
#include <stdlib.h>
#include <string.h>
#define CLKDIV 			((14)<<8)	
#define PDN					(1<<21)
#define ADC_ch0			(1<<0)
#define ADC_ch1			(1<<1)
#define ADC_ch2			(1<<2)
#define ADC_ch3			(1<<3)
#define ADC_ch4			(1<<4)
#define ADC_ch5			(1<<5)
#define ADC_ch6			(1<<6)
#define ADC_ch7			(1<<7)
#define START_CONVERSATION		(1<<24)
#define DONE									(1<<31)
#define STATUS_DONE_6					(1<<6)
#define VREF										3.3

uint8_t value[4],temp_data[3];
int ADC_data,temp_int;
float temp;

int main(void){
	
	 PINSEL0 = 0x00000300;
   PINSEL1 = 0;
	 IO1DIR = ~(0x00);
	 IO1DIR &= LCD_4_BIT_DATA_PINS;
	 
	 LCD_Init(LCD_TYPE_4_BIT);
	 		
	AD0CR = ADC_ch6 ; // select channel AD0.6
	AD0CR|= CLKDIV ;	// devide the clock is less or equal to 4.5MHz
	AD0CR|= PDN;			// PDN =1 , operational mode.
	AD0CR |= START_CONVERSATION;  // start the conversation
	while((AD0GDR & DONE) ==0 ) 	// wait for conversation DONE.
	{
	}
	ADC_data = AD0GDR >>6;			// read the Result 6:15 bits, shift to 6-bits to  9:0
		
	
	while(1){
		AD0CR |= START_CONVERSATION;
		LCD_send_CMD(LCD_TYPE_4_BIT,LCD_CLEAR);
	  LCD_send_CMD(LCD_TYPE_4_BIT,LCD_CURSOR_1ST_LINE);
	  LCD_send_String(LCD_TYPE_4_BIT,"LM35_INTERFACE");
		while((AD0GDR & DONE) ==0 ){}											// wait for conversation DONE.
			
		
			ADC_data = AD0GDR >>6;													// read the Result 6:15 bits, shift to 6-bits to  9:0
			delay(2);
      ADC_data = ADC_data & 0x3ff;										// check the result with 0x3FF.
			temp = (float)((ADC_data*VREF*100)/1024);  			// formula to convert adc data to temperature for LM35 sensor
			
			//sprintf((char*)value,"%04u",ADC_data);
			temp_int = (int)temp;													// convert float to int.			
			sprintf((char*)temp_data,"%4u",temp_int);			// convert int to string. assigning to character array.
			
			/*LCD_send_CMD(LCD_TYPE_4_BIT,LCD_CLEAR);
			LCD_send_CMD(LCD_TYPE_4_BIT,LCD_CURSOR_1ST_LINE);
			LCD_send_String(LCD_TYPE_4_BIT,"ADC_data:");
			LCD_send_String(LCD_TYPE_4_BIT,(char*)value);*/
			
			LCD_send_CMD(LCD_TYPE_4_BIT,LCD_CURSOR_2ND_LINE);
			LCD_send_String(LCD_TYPE_4_BIT,"TEMP:");
			LCD_send_String(LCD_TYPE_4_BIT,(char*)temp_data);					//display the temperature data.
			LCD_send_Data(LCD_TYPE_4_BIT,223);												//display the character symbol ASCII number is 223
			LCD_send_Data(LCD_TYPE_4_BIT,'C');												// temperature in cntigrade in C.
			
			delay(1000);																							// delay for 1 second.
			memset(value,0,sizeof(value));														//reset the buffer of value.
			memset(temp_data,0,sizeof(temp_data));										//reset the buffer of temp_data.
			
	
	}
}
	