#include "LCD_4-bit mode.h"

int main(void){
	
	PINSEL0 = 0;
	PINSEL1 =0;
	IO1DIR = ~(0x00);
	IO1DIR &= LCD_4_BIT_DATA_PINS;
	LCD_Init(LCD_TYPE_4_BIT);
	
	while(1){
	LCD_send_CMD(LCD_TYPE_4_BIT,LCD_CLEAR);
	LCD_send_String(LCD_TYPE_4_BIT,"Hello World");
	delay(100);
  LCD_send_CMD(LCD_TYPE_4_BIT,LCD_CLEAR);
	LCD_send_String(LCD_TYPE_4_BIT,"My name is ");
	LCD_send_CMD(LCD_TYPE_4_BIT,LCD_CURSOR_2ND_LINE);
	LCD_send_String(LCD_TYPE_4_BIT,"KALYAN KUMAR");
	delay(100);
	}
	
	}











	