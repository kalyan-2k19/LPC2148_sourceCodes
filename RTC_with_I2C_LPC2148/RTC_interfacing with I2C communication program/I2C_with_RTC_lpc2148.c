#include "LCD.h"
#include "I2C_Lpc2148.h"
//#include "I2C.h"
#include "UART.h"

uint8_t buf[1] ={0};

// conversions, BCD convert to decimal MSB
uint8_t MSB_BCD_TO_DEC(uint8_t data);
//BCD to decimal of single digit of LSB
uint8_t LSB_BCD_TO_DEC(uint8_t data);

uint8_t Hours,Minutes,sec,Day_of_month,month,year;
uint8_t temp[10] = "00:00:00";
uint8_t date[10] = "00/00/00";

int main(void){
//	uint8_t data;
	PINSEL0 = 0;
	PINSEL1 = 0;
	IO1DIR = ~(0x00);
	IO1DIR &= LCD_4_BIT_DATA_PINS;
	
	LCD_Init(LCD_TYPE_4_BIT);
	UART0_Init();
	I2C0_Init();
	while(1){
	I2C0_Start();
//	while(!Check_Status_Flag(0x08));
	I2C0_MasterSendData(0xD0);
	//while(!Check_Status_Flag(0x18));
	I2C0_MasterSendData(0x00);
	//while(!Check_Status_Flag(0x28));
	I2C0_MasterSendData(0x02); //while(!Check_Status_Flag(0x28));
	I2C0_MasterSendData(0x22);// while(!Check_Status_Flag(0x28));
  I2C0_MasterSendData(0x11); //while(!Check_Status_Flag(0x28));
	I2C0_MasterSendData(0x08);// while(!Check_Status_Flag(0x28));
	I2C0_MasterSendData(0x07); //while(!Check_Status_Flag(0x28));
	I2C0_MasterSendData(0x20); //while(!Check_Status_Flag(0x28));
	I2C0_Stop();
	I2C0_Start();
	I2C0_MasterSendData(0xD1);	
		
	int d = I2C0_MasterReadData();
	I2C0_Stop();
	
	}
	
}


// BCD to ASCII conversion of MSB digit of BCD function definition.
uint8_t MSB_BCD_TO_DEC(uint8_t data){
	uint8_t MSB;
	MSB = data/10 +'0';
	return MSB;
}
// BCD to ASCII conversion of LSB digit of BCD function definition.
uint8_t LSB_BCD_TO_DEC(uint8_t data){
	uint8_t LSB;
	LSB = data%10 +'0';
	return LSB;
}

