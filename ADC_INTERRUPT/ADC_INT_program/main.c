#include "LCD.h"
#include <stdlib.h>
#include <string.h>
#define CLKDIV 			((14)<<8)	// select appropriate clock for <=4.5MHz frequency
#define PDN					(1<<21)		// operational mode

#define ADC_ch0			(1<<0)
#define ADC_ch1			(1<<1)
#define ADC_ch2			(1<<2)
#define ADC_ch3			(1<<3)
#define ADC_ch4			(1<<4)
#define ADC_ch5			(1<<5)
#define ADC_ch6			(1<<6)
#define ADC_ch7			(1<<7)
#define VREF					3.3

#define BURST_MODE_ON  (1<<16)
#define CLOCKS				  (0<<19)|(0<<18)|(0<<17)

uint8_t value[4],volt[4];
uint32_t ADC_data,ADC_int;
float Celcius;

//ADC functions
void ADC_Init(void);
void ADC_interrupt(void);
void ADC_ISR(void)__irq;

int main(void){
	
	 PINSEL0 = 0x00000300;
   PINSEL1 = 0;
	 IO1DIR = ~(0x00);
	 IO1DIR &= LCD_4_BIT_DATA_PINS;
	 
	 LCD_Init(LCD_TYPE_4_BIT);
	 ADC_Init();
	 ADC_interrupt();
	 delay(100);
	
	while(1){

		LCD_send_CMD(LCD_TYPE_4_BIT,LCD_CLEAR);
	  LCD_send_CMD(LCD_TYPE_4_BIT,LCD_CURSOR_1ST_LINE+3);
	  LCD_send_String(LCD_TYPE_4_BIT,"ADC data");
	  
		Celcius = (float)(ADC_data * VREF * 100)/1024;
		
			sprintf((char*)value,"%04u",ADC_data);
		  ADC_int = (int)Celcius;
			sprintf((char*)volt,"%2u",ADC_int);
		
			LCD_send_CMD(LCD_TYPE_4_BIT,LCD_CLEAR);
			LCD_send_CMD(LCD_TYPE_4_BIT,LCD_CURSOR_1ST_LINE);
			LCD_send_String(LCD_TYPE_4_BIT,"data:");
			LCD_send_CMD(LCD_TYPE_4_BIT,LCD_CURSOR_1ST_LINE+5);
			LCD_send_String(LCD_TYPE_4_BIT,(char*)value);
			LCD_send_CMD(LCD_TYPE_4_BIT,LCD_CURSOR_2ND_LINE);
			LCD_send_String(LCD_TYPE_4_BIT,"TEMP:");
			LCD_send_CMD(LCD_TYPE_4_BIT,LCD_CURSOR_2ND_LINE+6);
			LCD_send_String(LCD_TYPE_4_BIT,(char*)volt);
			delay(200);
			memset(value,0,sizeof(value));
			memset(volt,0,sizeof(volt));
			
		
		}
	
	}

// ADC0 Channel-6 configuration function definition.
void ADC_Init(void){
	AD0CR = ADC_ch6 ;
	AD0CR|= CLKDIV ;
	AD0CR|= PDN | BURST_MODE_ON | CLOCKS;
	AD0INTEN = ADC_ch6;

}
// ADC0 interrupt function definition
void ADC_interrupt(void){
	VICIntEnable = (1<<18); // Enable ADC0 interrupt.
	VICVectCntl1 = (0x20)|18; // select slot1 for ADC0.
	VICVectAddr1 = (unsigned)ADC_ISR; // Assign ISR address to specific adress register.
}

// ADC0 ISR function definition.
void ADC_ISR(void)__irq
{
	uint8_t channel;
	uint16_t data, dummy_read;
	channel = (AD0GDR >> 24)&0xF;
	data = (AD0GDR >> 6)& 0x3ff;
	
	if(channel == 6){
		dummy_read = AD0DR6>>6;
		ADC_data = data;
	  (void)dummy_read;
	}
	VICVectAddr = 0;
}
	
	

	
	

	