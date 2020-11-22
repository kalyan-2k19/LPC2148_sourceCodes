#include "LCD.h"
#include <string.h>
#include "UART.h"
//structure for RTC.

typedef struct {
	int seconds;
	int minutes;
	int hours;
	uint8_t day_of_month;
	uint8_t day_of_week;
	uint16_t day_of_year;
	uint8_t month;
	int year;
	
}RTC_Time_t;

#define SET					(0<<3)
#define UP					(0<<4)
#define DOWN				(0<<5)

//functions for RTC Get time
void RTC_Init(void);
void RTC_setTime(RTC_Time_t set_time);
RTC_Time_t RTC_GetTime(void);

// conversions, BCD convert to decimal MSB
uint8_t MSB_BCD_TO_DEC(uint8_t data);
//BCD to decimal of single digit of LSB
uint8_t LSB_BCD_TO_DEC(uint8_t data);
//decimal to BCD of two digits
uint8_t DECIMAL_TO_BCD(uint8_t decimal);
//BCD to Decimal of two digits
uint8_t BCD_TO_DECIMAL(uint8_t bcd_data);
//decimal to binary function
long int DECIMAL_TO_BINARY(uint8_t decimal);
//binary to decimal 
uint8_t BINARY_TO_DECIMAL(long int binary_data);

uint8_t time[10] = "00:00:00";
uint8_t date[10] = "00/00/00";

int main(void){
	RTC_Time_t Set_Time,Current_Time;
	int set,set_counter = 0;
	
	
	PINSEL0 = 0;
	PINSEL1 = 0;
	IO1DIR = ~(0x00);
	IO1DIR &= LCD_4_BIT_DATA_PINS;
	IO0DIR |= SET | UP |DOWN ;
	//IO0SET = ~(SET| UP|DOWN);
	RTC_Init();
	LCD_Init(LCD_TYPE_4_BIT);
	UART0_Init();
	

	memset(&Set_Time,0,sizeof(Set_Time));
	memset(&Current_Time,0,sizeof(Current_Time));
	
  Set_Time.seconds			= 0;
	Set_Time.minutes			= 17;
	Set_Time.hours				= 12;
	Set_Time.day_of_month = 30;
	Set_Time.month				= 6;
	Set_Time.year					= 20;
	RTC_setTime(Set_Time);
	

	LCD_send_CMD(LCD_TYPE_4_BIT,LCD_CLEAR);
	LCD_send_String(LCD_TYPE_4_BIT,"Time:");
	//LCD_send_String(LCD_TYPE_4_BIT,(char*)time);
	LCD_send_CMD(LCD_TYPE_4_BIT,LCD_CURSOR_2ND_LINE);
	LCD_send_String(LCD_TYPE_4_BIT,"Date:");
	//LCD_send_String(LCD_TYPE_4_BIT,(char*)date);
	//delay(10);
	while(1){
		
		set = 0;
		if((IO0PIN & (1<<3))){
			// check set pin 
			delay(3);
		// to avoid debouncing use delay.
					set_counter++;
			// set_counter is used to select the particular case like 1.HOUR,2.MIN,3.SEC,4.DOM,5.MONTH,6.YEAR
					if(set_counter >= 7){
						set_counter =0;
						}
				}
		
		if(set_counter){
			
			if((IO0PIN & (1<<4))){

				delay(3);
					set =1;
				
			}
		
			if((IO0PIN & (1<<5)) ){
				delay(3);
				
					set = -1;
				
			}
		if(set_counter){
			 /*if(set_counter == 1){
				LCD_send_CMD(LCD_TYPE_4_BIT,LCD_CLEAR);
				LCD_send_String(LCD_TYPE_4_BIT,"Change HOUR");
			}
			else if(set_counter == 2){
				LCD_send_CMD(LCD_TYPE_4_BIT,LCD_CLEAR);
				LCD_send_String(LCD_TYPE_4_BIT,"Change MIN");
			}
			else if(set_counter == 3){
				LCD_send_CMD(LCD_TYPE_4_BIT,LCD_CLEAR);
				LCD_send_String(LCD_TYPE_4_BIT,"Change SEC");
			}
			else if(set_counter == 4){
				LCD_send_CMD(LCD_TYPE_4_BIT,LCD_CLEAR);
				LCD_send_String(LCD_TYPE_4_BIT,"Change MONTH");
			}
			else if(set_counter == 5){
				LCD_send_CMD(LCD_TYPE_4_BIT,LCD_CLEAR);
				LCD_send_String(LCD_TYPE_4_BIT,"Change DATE");
			}
			else if(set_counter == 6){
				LCD_send_CMD(LCD_TYPE_4_BIT,LCD_CLEAR);
				LCD_send_String(LCD_TYPE_4_BIT,"Change YEAR");
			}*/
			
				
			if(set_counter && set){
				
				switch(set_counter){
					
					case 1: Current_Time.hours = HOUR;
									Current_Time.hours = Current_Time.hours & 0b00111111;
								
									Current_Time.hours = Current_Time.hours + set;
									
						      
					
									/*if((Current_Time.hours ) >23){
										Current_Time.hours = 0;
									}
									else if((Current_Time.hours ) < 0){
										Current_Time.hours = 23;
									}*/
								 if(Current_Time.hours > 12){
									 
										Current_Time.hours = 1;
									}
								 else if(Current_Time.hours <= -1){
									 
									 Current_Time.hours = 12;
								 }
									
									HOUR = Current_Time.hours;
									break;
					case 2: Current_Time.minutes = MIN;
									Current_Time.minutes = Current_Time.minutes & 0b11111111;
									Current_Time.minutes = Current_Time.minutes + set;									
									
									if((Current_Time.minutes )> 59){
										
										Current_Time.minutes = 0;
									}
									else if(Current_Time.minutes < 0){
										
										Current_Time.minutes = 59;
									}
									
									MIN = Current_Time.minutes;
									break;
									
					case 3: Current_Time.seconds = SEC;
									SEC = 0x00;
									Current_Time.minutes = SEC;
									break;
									
					case 5: Current_Time.month = MONTH;
									
									Current_Time.month = Current_Time.month + set;
									
									if(Current_Time.month > 12){
										
										Current_Time.month = 1;
									}
									else if(Current_Time.month <= 0){
										
										Current_Time.month = 12;
									}
									
									MONTH = Current_Time.month;
									break;
								
					case 4: Current_Time.day_of_month = DOM;
									
									Current_Time.day_of_month = Current_Time.day_of_month + set;
									
									
									if(Current_Time.day_of_month >= 28){
										
										if(Current_Time.month == 2){
											
											Current_Time.day_of_month = 28;
										}
									}
									
									if(Current_Time.day_of_month >= 30){
										
										if(Current_Time.day_of_month == 31){
											
											if((Current_Time.month == 1) || (Current_Time.month == 3)||(Current_Time.month == 5)|| (Current_Time.month == 7)||(Current_Time.month == 8)|| (Current_Time.month == 10)|| (Current_Time.month == 12)){
												
												Current_Time.day_of_month = 31;
									}
								}
										if(Current_Time.day_of_month == 30){
											
											if((Current_Time.month == 4)|| (Current_Time.month == 6)|| (Current_Time.month == 9)|| (Current_Time.month == 11)){
												
												Current_Time.day_of_month = 30;
										}
									}
										if((Current_Time.month == 4)|| (Current_Time.month == 6)|| (Current_Time.month == 9)|| (Current_Time.month == 11)){
											if(Current_Time.day_of_month > 30){
													
												Current_Time.day_of_month = 1;
										}
									}
									 if((Current_Time.month == 1) || (Current_Time.month == 3)||(Current_Time.month == 5)|| (Current_Time.month == 7)||(Current_Time.month == 8)|| (Current_Time.month == 10)|| (Current_Time.month == 12)){	
										  
											if(Current_Time.day_of_month > 31){
											
												Current_Time.day_of_month = 1;
									}
								}
							}
									 
									
									else if(Current_Time.day_of_month < 1){
										
										if((Current_Time.month == 1) || (Current_Time.month == 3)||(Current_Time.month == 5)|| (Current_Time.month == 7)||(Current_Time.month == 8)|| (Current_Time.month == 10)|| (Current_Time.month == 12)){
											
											Current_Time.day_of_month = 31;
									}
										if((Current_Time.month == 4)|| (Current_Time.month == 6)|| (Current_Time.month == 9)|| (Current_Time.month == 11)){
											
											Current_Time.day_of_month = 30;
										}
									}
									
										
									
									DOM = Current_Time.day_of_month;
									break;
									
					case 6: Current_Time.year = YEAR;
									
									Current_Time.year = Current_Time.year +set;
								
									if(Current_Time.year <= -1){
										
										Current_Time.year = 99;
									}
									else if(Current_Time.year >= 99){
										
										Current_Time.year = 0;
									}
									
									YEAR = Current_Time.year;
									break;
				
				}
			}
		}
	}
		Current_Time	= RTC_GetTime();
		//  time hours
		time[0] = MSB_BCD_TO_DEC(Current_Time.hours);
		time[1] = LSB_BCD_TO_DEC(Current_Time.hours);
		// : (colon) display leave one byte
		// time  minutes
		time[3] = MSB_BCD_TO_DEC(Current_Time.minutes);
		time[4] = LSB_BCD_TO_DEC(Current_Time.minutes);
		// : (colon) display leave one byte
		// time seconds
		time[6] = MSB_BCD_TO_DEC(Current_Time.seconds);
		time[7] = LSB_BCD_TO_DEC(Current_Time.seconds);
		
		
		// day of month
		date[0] = MSB_BCD_TO_DEC(Current_Time.day_of_month);
		date[1] = LSB_BCD_TO_DEC(Current_Time.day_of_month);
		// levae one byte for '/' 
		//current month
		date[3] = MSB_BCD_TO_DEC(Current_Time.month);
		date[4] = LSB_BCD_TO_DEC(Current_Time.month);
		//current year.
	  date[6] = MSB_BCD_TO_DEC(Current_Time.year);
		date[7] = LSB_BCD_TO_DEC(Current_Time.year);
    /*sprintf((char*)time,"%d:%d:%d",Current_Time.hours,Current_Time.minutes,Current_Time.seconds);
		sprintf((char*)date,"%d/%d/%d",Current_Time.day_of_month,Current_Time.month,Current_Time.year);*/
		
		UART0_TX_String((char*)time);
		UART0_TX('\t');
		UART0_TX_String((char*)date);
		UART0_TX_String("\r\n");
		
		//display date and time on LCD.
		LCD_send_CMD(LCD_TYPE_4_BIT,LCD_CURSOR_1ST_LINE+5);
		LCD_send_String(LCD_TYPE_4_BIT,(char*)time);
		LCD_send_CMD(LCD_TYPE_4_BIT,LCD_CURSOR_2ND_LINE+5);
		LCD_send_String(LCD_TYPE_4_BIT,(char*)date);
	
}
	}
		
		
		
		
		
	//RTC Initialization
void RTC_Init(void){
	ILR = 0x00;   						// Interrupt location register is set to ZERO. no need of interrupts
	CCR = 0x02;								// reset the RTC to eneble timers.
	CCR = 0x00;
	PREINT = 0x0726;					// PREINT for 60MHz clock
	PREFRAC = 0x0700;					// PREFRAC for 60MHz clock.
	
	CCR = 0x01;								// Enable RTC.
	
}

//SET the time of RTC before using. function definition
void RTC_setTime(RTC_Time_t set_time){
	SEC 	= set_time.seconds;
	MIN 	= set_time.minutes;
	HOUR  = set_time.hours;
	DOM   = set_time.day_of_month;
	DOW   = set_time.day_of_week;
	DOY   = set_time.day_of_year;
  MONTH = set_time.month;
  YEAR  = set_time.year;

}
// RTC get time from registers of LPC2148 function definition.
RTC_Time_t RTC_GetTime(void){
	RTC_Time_t time;
	
	time.seconds				= SEC;
	time.minutes			  = MIN;
	time.hours          = HOUR;
	time.day_of_month   = DOM;
	time.day_of_week		= DOW;
	time.day_of_year		= DOY;
	time.month					= MONTH;
	time.year						= YEAR;
	
	return time;
	
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

// Decimal to BCD conversion of two digits function definition.
/*uint8_t DECIMAL_TO_BCD(uint8_t decimal){
	int MSB,LSB;
	MSB = decimal/10;
	MSB = MSB <<4;
	LSB = decimal%10;
	MSB = MSB | LSB ;
	//(((decimal/10)<<4)|(decimal %10));
	return MSB;
}
//BCD to Decimal of two digits function definition
uint8_t BCD_TO_DECIMAL(uint8_t bcd_data){
	
	return (((bcd_data>>4)*10) +(bcd_data & 0x0F));
}
//decimal to binary conversion definition 
long int DECIMAL_TO_BINARY(uint8_t decimal){
	long int bin=0;
	int rem, i=1;
	while(decimal!=0){
		rem = decimal % 2;
		decimal = decimal / 2;
		bin = bin + rem*i;
		i = i*10;
	}
	return bin;
}
	


//binary to decimal conversion definition
uint8_t BINARY_TO_DECIMAL(long int binary_data ){
	int rem,decimal_data = 0,dummy, mul = 1;
	
	while(binary_data > 0){
		rem = binary_data % 10;
		dummy = rem*mul;
		decimal_data += dummy;
		mul = mul*2;
		binary_data = binary_data /2;
	}
	return decimal_data;
}*/

