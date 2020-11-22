#include <LPC214X.H>
#include <stdint.h>
#include <stdio.h>
#include "PLL.h"
#include "Timer.h"



// Commands for LCD

#define LCD_CLEAR							 							0x01
#define LCD_RETURN_TO_HOME		 							0X02
#define LCD_ENTRY_MODE    								  0x06
#define LCD_DISPLAY_ON_CURSOR_BLINK					0x0C
#define LCD_8_BIT_MODE											0X38
#define LCD_4_BIT_MODE											0x28
#define LCD_CURSOR_1ST_LINE									0x80
#define LCD_CURSOR_2ND_LINE 								0xC0


/* LCD 4-bit mode
 *VSS---->GND,
 * VDD---->+5V,
 * VEE---->GND or connected to 10k potentiomer ouput,
 * RS----->P0.0 of LPC2148
 * EN----->P0.1 of LPC2148
 
 
 */
#define RS												(1<<14) //port0 14th pin
#define EN												(1<<15) //port1 15th pin
#define LCD_4_BIT_DATA_PINS				0x00F00000
#define LCD_8_BIT_DATA_PINS				0x00FF0000
#define LCD_TYPE_4_BIT						0
#define LCD_TYPE_8_BIT						1
#define LCD_8_BIT_DATA_SHIFT			16
#define LCD_4_BIT_DATA_SHIFT			20



void LCD_Init(uint8_t Mode);
void LCD_send_CMD(uint8_t Mode,uint8_t cmd);
void LCD_send_Data(uint8_t Mode,uint8_t data);
void LCD_send_String(uint8_t Mode, const char *data);
//void LCD_send_Interger(uint32_t *number, uint16_t Len);

void LCD_Init(uint8_t Mode){


	// Initialze the PLL0
	PLL_init(PLL0,PLL0_60MHz_pclk_60MHz_cclk);
	// Initialize the TImer.
	Timer0_init_1Ms(DELAY_1MS);
	
	LCD_send_CMD(Mode,LCD_CLEAR);
	delay(2);
	if(Mode == LCD_TYPE_4_BIT){
		LCD_send_CMD(Mode,LCD_4_BIT_MODE);
		delay(2);
	}
	else if(Mode == LCD_TYPE_8_BIT){
		LCD_send_CMD(Mode,LCD_8_BIT_MODE);
		delay(2);
	}
		
	LCD_send_CMD(Mode,LCD_ENTRY_MODE);
	delay(2);
	LCD_send_CMD(Mode,LCD_RETURN_TO_HOME);
	delay(2);
	LCD_send_CMD(Mode,LCD_DISPLAY_ON_CURSOR_BLINK);
	delay(2);
	LCD_send_CMD(Mode,LCD_CURSOR_1ST_LINE);
	delay(2);
}
	
	
 
 void LCD_send_CMD(uint8_t Mode,uint8_t cmd){
	 
	 if(Mode == LCD_TYPE_4_BIT){
		 
		IO0DIR |= RS | EN;
		IO0CLR |= RS;
		delay(1);
		IO0SET |= EN;
		delay(1);
		IO1PIN = ((cmd>>4) & (0x0f))<<LCD_4_BIT_DATA_SHIFT;
		IO0CLR |= EN;
		delay(1);
		 
		IO0CLR |= RS;
		delay(1);
		IO0SET |= EN;
		delay(1);
		IO1PIN = ((cmd)&(0x0f))<<LCD_4_BIT_DATA_SHIFT;		
		IO0CLR |= EN;
		delay(1);
	 }
	 else if(Mode == LCD_TYPE_8_BIT){
		 IO0DIR |= RS | EN;
		 IO0CLR |= RS;
		 delay(1);
		 IO1PIN = ((cmd)&(0xFF))<<LCD_8_BIT_DATA_SHIFT;
		 IO0SET |= EN;
		 delay(1);
		 IO0CLR |= EN;
		 delay(1);
	 }
 }
		 
 
	
 
void LCD_send_Data(uint8_t Mode,uint8_t data){
	if(Mode == LCD_TYPE_4_BIT){
		
		IO0DIR |= RS | EN;
		
		IO0SET |= RS;
		delay(1);
		IO0SET |= EN;
		delay(1);
		IO1PIN = ((data>>4) & (0x0F))<<LCD_4_BIT_DATA_SHIFT;
		IO0CLR |= EN;
		delay(1);
		
		IO0SET |= RS;
		delay(1);
		IO0SET |= EN;
		delay(1);
		IO1PIN = ((data)& (0x0f))<<LCD_4_BIT_DATA_SHIFT;
		
		
		
		IO0CLR |= EN;
		delay(1);
}
	else if(Mode == LCD_TYPE_8_BIT){
		IO0DIR |= RS | EN;
		IO0SET |= RS;
		delay(1);
		IO1PIN = ((data)&(0xFF))<<LCD_8_BIT_DATA_SHIFT;
		IO0SET |= EN;
		delay(1);
		IO0CLR |= EN;
		delay(1);
	}
}
		

void LCD_send_String(uint8_t Mode, const char *data){
	int i=0;
	for (i=0; data[i]!='\0';i++){
		LCD_send_Data(Mode, data[i]);
		delay(1);
	}
}





