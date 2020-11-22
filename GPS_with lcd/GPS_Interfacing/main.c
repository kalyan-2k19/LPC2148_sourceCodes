#include "LCD_4_8.h"
#include "UART.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
	float lat_Result;
	float long_Result;
	
	float lat_data_decimal_and_float;
	float long_data_decimal_and_float;
	
	float lat_data_minutes;
	float long_data_minutes;
	
	uint32_t lati_data_degrees;
	uint32_t long_data_degrees;
	
}GPS_t;


static uint8_t Get_GPS_data(void);
static void GPS_data(void);
GPS_t GPS;

char data,latitude[15],longitude[15],buffer[75];
const char *test_data = "$GPGGA";

uint8_t i=0;

 int main(void){
	 PINSEL0 = 0;
   PINSEL1 = 0;
	 IO1DIR = ~(0x00);
	 IO1DIR &= LCD_4_BIT_DATA_PINS;
	 
	 LCD_Init(LCD_TYPE_4_BIT);
	 UART0_Init();
	 LCD_send_CMD(LCD_TYPE_4_BIT,LCD_CLEAR);
	 LCD_send_CMD(LCD_TYPE_4_BIT,LCD_CURSOR_1ST_LINE+3);
	 LCD_send_String(LCD_TYPE_4_BIT,"GPS WITH");
	 LCD_send_CMD(LCD_TYPE_4_BIT,LCD_CURSOR_2ND_LINE+5);
	 LCD_send_String(LCD_TYPE_4_BIT,"LPC2148");
	 delay(100);

	 memset(latitude,0,sizeof(latitude));
	 memset(longitude,0,sizeof(longitude));
	 
	 while(1){
		 LCD_send_CMD(LCD_TYPE_4_BIT,LCD_CLEAR);
		 
		 GPS_data();
		 UART0_TX_String("LAT:");
		 UART0_TX_String(latitude);
		 UART0_TX('N');
		 UART0_TX_String("LON:");
		 UART0_TX_String(longitude);
		 UART0_TX('S');
		 
		 LCD_send_CMD(LCD_TYPE_4_BIT,LCD_CLEAR);
		 LCD_send_CMD(LCD_TYPE_4_BIT,LCD_CURSOR_1ST_LINE);
	   LCD_send_String(LCD_TYPE_4_BIT,"LAT:");
	   LCD_send_String(LCD_TYPE_4_BIT,"");
	   LCD_send_CMD(LCD_TYPE_4_BIT,LCD_CURSOR_2ND_LINE);
	   LCD_send_String(LCD_TYPE_4_BIT,"LON:");
	   LCD_send_String(LCD_TYPE_4_BIT,"");
		 
		 LCD_send_CMD(LCD_TYPE_4_BIT,LCD_CURSOR_1ST_LINE+4);
		 LCD_send_String(LCD_TYPE_4_BIT,latitude);
		 LCD_send_Data(LCD_TYPE_4_BIT,'N');
		 LCD_send_CMD(LCD_TYPE_4_BIT,LCD_CURSOR_2ND_LINE+5);
		 LCD_send_String(LCD_TYPE_4_BIT,longitude);
		 LCD_send_Data(LCD_TYPE_4_BIT,'S');
		 delay(20);
		 memset(latitude,0,sizeof(latitude));
		 memset(longitude,0,sizeof(longitude));
		 
		 
	 }
	 
 }
 
uint8_t Get_GPS_data(void){
	
	 memset((char*)buffer,0,sizeof(buffer));
	 while(1){
		 //Read GPS string data.
		 data = UART0_RX();   //$GPGGA,134658.00,5106.9792,N,11402.3003,W,2,09,1.0,1048.47,M,-16.27,M,08,AAAA*60

		 buffer[i++] = data;
		 UART0_TX(data);
		 
		 
		 
		 // check GPS String data.
		 if(i<7){
			 if(buffer[i-1]!= test_data[i-1]){
					memset(buffer,0,sizeof(buffer));
					i=0;
			 }
		 }
		 if(data == '\r'|| data == '\n'){
			 if(i>38){
				 UART0_TX_String(buffer);
				 UART0_TX_String("\r\n");
				 return 1;
			 }
			 else{
				 i=0;
				 memset(buffer,0,sizeof(buffer));
				 return 0;
			 }
		 } 
		 
	 }
 }
 
	
			 
void GPS_data(void){
	uint8_t Index=0,x=0,y=0,coma=0;

		while(!Get_GPS_data());
			 
		while(Index<i){
			
			
			if(buffer[Index] == ','){
				coma++;
				Index = Index+1;
			}
				if(coma == 2){
					latitude[x++] = buffer[Index];			
					
				}
				else if(coma == 4){
					longitude[y++] = buffer[Index];
					
				
				}
			
			Index++;
		}
		LCD_send_CMD(LCD_TYPE_4_BIT,LCD_CLEAR);
		LCD_send_String(LCD_TYPE_4_BIT,"Getting GPS data");
		LCD_send_CMD(LCD_TYPE_4_BIT,LCD_CLEAR);
		LCD_send_String(LCD_TYPE_4_BIT,"LAT:");
		LCD_send_CMD(LCD_TYPE_4_BIT,LCD_CURSOR_1ST_LINE+4);
	  LCD_send_String(LCD_TYPE_4_BIT,latitude);
		delay(1);
		LCD_send_CMD(LCD_TYPE_4_BIT,LCD_CURSOR_2ND_LINE);
		LCD_send_String(LCD_TYPE_4_BIT,"LON:");
		LCD_send_CMD(LCD_TYPE_4_BIT,LCD_CURSOR_2ND_LINE+4);
		LCD_send_String(LCD_TYPE_4_BIT,longitude);
		delay(1);
		GPS.lat_data_decimal_and_float = strtof(latitude,NULL);
		
		
	 /* UART0_TX_String("Latitude:");
	  UART0_TX_String(latitude);
		UART0_TX_String("Longitude:");
	  UART0_TX_String(longitude);
    */	
		/* convert string to float by using atof().
			 shift the floating point to 2 positions left because getting value from gps is 1234.5678.\
			 we need 12 value , so 1234.567/100 = 12.34567. actually gps data 12 degrees 34.567 minutes\
	     we want degrees not in minutes so convert minutes inti degress devided by 60 or \
       devide the whole data by 100 we get 1234.567/100 = 12.34567, and devide float data by 0.6\
			 we get data in degrees. after that add interger data and float data we get latitude and longitude values.\
		  (for both latitude and longitude the calculations is same).
   */		
	
	 GPS.lat_data_decimal_and_float = GPS.lat_data_decimal_and_float /100;
	 GPS.lati_data_degrees = (uint8_t)GPS.lat_data_decimal_and_float;        // take integer data.
	 GPS.lat_data_minutes = (GPS.lat_data_decimal_and_float - GPS.lati_data_degrees)/0.6;  // remove interger data from actual data we get float data
	 GPS.lat_Result = (float)(GPS.lati_data_degrees + GPS.lat_data_minutes);
	 sprintf(latitude,"%f",GPS.lat_Result);   //result of Latitude of GPS.
	/* UART0_TX_String("Latitude:");
	 UART0_TX_String(latitude);
	 */
	 GPS.long_data_decimal_and_float = strtof(longitude,NULL);
	 GPS.long_data_decimal_and_float =  GPS.long_data_decimal_and_float/100;
	 GPS.long_data_degrees = (uint8_t)GPS.long_data_decimal_and_float;
	 GPS.long_data_minutes = ( GPS.long_data_decimal_and_float - GPS.long_data_degrees)/0.6;
	 GPS.long_Result = (float)(GPS.long_data_degrees + GPS.long_data_minutes);
	 sprintf(longitude,"%f",GPS.long_Result);
	 /*UART0_TX_String("Longitude:");
	 UART0_TX_String(longitude);
	 */
	 i=0;
	 
	 
		
	}

			
			
		
		
			
			