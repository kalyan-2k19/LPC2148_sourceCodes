#include "LCD.h"
#include "UART.h"

uint8_t buffer[13];
uint8_t data,i;


int main(void){
	 PINSEL0 = 0;
   PINSEL1 = 0;
	 IO1DIR = ~(0x00);
	 IO1DIR &= LCD_4_BIT_DATA_PINS;
	
	LCD_Init(LCD_TYPE_4_BIT);
	UART0_Init();
	
	while(1){
		LCD_send_CMD(LCD_TYPE_4_BIT,LCD_CLEAR);
	  LCD_send_CMD(LCD_TYPE_4_BIT,LCD_CURSOR_1ST_LINE+4);
	  LCD_send_String(LCD_TYPE_4_BIT,"RFID DATA");
	  delay(10);
		for(i=0;i<12;i++){
			data = UART0_RX();
			buffer[i] = data;
		}
		buffer[i] = '\0';
		
		
		LCD_send_CMD(LCD_TYPE_4_BIT,LCD_CURSOR_2ND_LINE+1);
		LCD_send_String(LCD_TYPE_4_BIT,(char*)buffer);
		delay(300);
		
		
			
	}
	}

	
	