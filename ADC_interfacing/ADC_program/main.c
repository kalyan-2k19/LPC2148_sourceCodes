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
#define DONE				(1<<31)
#define STATUS_DONE_6			(1<<6)

uint8_t value[4],volt[15];
uint32_t data,volt_int;
float voltage;

int main(void){
	
	 PINSEL0 = 0x00000300;
   PINSEL1 = 0;
	 IO1DIR = ~(0x00);
	 IO1DIR &= LCD_4_BIT_DATA_PINS;
	 
	 LCD_Init(LCD_TYPE_4_BIT);
	 
		
	AD0CR = ADC_ch6 ;
	AD0CR|= CLKDIV ;
	AD0CR|= PDN;
	
	
	while(1){
		AD0CR |= START_CONVERSATION;
		LCD_send_CMD(LCD_TYPE_4_BIT,LCD_CLEAR);
	  LCD_send_CMD(LCD_TYPE_4_BIT,LCD_CURSOR_1ST_LINE+3);
	  LCD_send_String(LCD_TYPE_4_BIT,"ADC data");
	  //delay(10);
		while((AD0GDR & DONE) ==0 ){}
			
		if(!(AD0STAT & STATUS_DONE_6)){
			data = AD0GDR >>6;
			delay(2);
      data = data & 0x3ff;
			voltage = (data/1023)*3.3;
			volt_int = (int)voltage;
			
		  
			
			sprintf((char*)volt,"%2u",volt_int);
			sprintf((char*)value,"%04u",data);
			
			LCD_send_CMD(LCD_TYPE_4_BIT,LCD_CLEAR);
			LCD_send_CMD(LCD_TYPE_4_BIT,LCD_CURSOR_1ST_LINE);
			LCD_send_String(LCD_TYPE_4_BIT,"data:");
			LCD_send_CMD(LCD_TYPE_4_BIT,LCD_CURSOR_1ST_LINE+5);
			LCD_send_String(LCD_TYPE_4_BIT,(char*)value);
			LCD_send_CMD(LCD_TYPE_4_BIT,LCD_CURSOR_2ND_LINE);
			LCD_send_String(LCD_TYPE_4_BIT,"VOLT:");
			LCD_send_CMD(LCD_TYPE_4_BIT,LCD_CURSOR_2ND_LINE+6);
			LCD_send_String(LCD_TYPE_4_BIT,(char*)volt);
			delay(100);
			memset(value,0,sizeof(value));
			memset(volt,0,sizeof(volt));
			
		
		}
	
	}
}
	