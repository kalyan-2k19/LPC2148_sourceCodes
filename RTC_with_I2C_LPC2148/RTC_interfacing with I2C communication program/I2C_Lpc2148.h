#include <LPC214X.H>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

//Function declarations of I2C0.

void I2C0_Init(void);
void I2C0_Start(void);
uint8_t Check_Status_Flag(uint8_t status);
void check_SI_Flag(void);
void Clear_SI_Flag(void);
void I2C0_Ack(uint8_t ENorDI);
uint8_t I2C0_MasterSendData(uint8_t add);
uint8_t I2C0_MasterReadData(void);
//uint8_t I2C0_MasterSendData(uint8_t slave_add,uint8_t *buf,uint8_t len);
//uint8_t I2C0_MasterReadData(uint8_t slave_add, uint8_t *buf, uint8_t len);
void I2C0_Stop(void);

//macros of I2C0 
#define AA								(1<<2)
#define SI								(1<<3)
#define STOP							(1<<4)
#define START							(1<<5)
#define I2C_EN						(1<<6)

#define ENABLE							1
#define DISABLE							0


// Function definitions of I2C

void I2C0_Init(void){
	PINSEL0 |= 0x00000050;									// Select I2C0 alternate fnction mode.		
	I2C0CONCLR = SI | START | STOP | AA;  // clear SI, START, STOP bits in the control register
	I2C0SCLH = 300; 										 // Serial clock High pulse at 100KHz at PCLk is 60MHz
	I2C0SCLL = 300;											 // Serial clock Low pulse at 100KHz at PCLk is 60MHz
	I2C0CONSET = I2C_EN;
	
}

void I2C0_Start(void){
	I2C0CONCLR = SI | START | STOP | AA;
	I2C0CONSET |= START;
	check_SI_Flag();
}

uint8_t Check_Status_Flag(uint8_t status){
	if(I2C0STAT == status){
		return 1;
	}
	else 
		return 0;
	
}

void check_SI_Flag(void){
	while(!(I2C0CONSET & SI));
}
void Clear_SI_Flag(void){
	I2C0CONCLR = SI;
}

void I2C0_Ack(uint8_t ENorDI){
	if(ENorDI == ENABLE){
		I2C0CONSET = AA;
	}
	else
		I2C0CONCLR = AA;
}

uint8_t I2C0_MasterSendData(uint8_t add){
	I2C0CONCLR = SI | START | STOP;
	I2C0DAT = add;
	check_SI_Flag();
	return 1;
}

uint8_t I2C0_MasterReadData(void){
	
	I2C0_Ack(ENABLE);
	I2C0CONCLR = SI;
	check_SI_Flag();
	

	return I2C0DAT;
	
}
/*uint8_t I2C0_MasterSendData(uint8_t slave_add,uint8_t *buf,uint8_t len){
	I2C0_Start();
	check_SI_Flag();
	while(!Check_Status_Flag(0x08));
	Clear_SI_Flag();
	I2C0DAT = slave_add;
	I2C0_Ack(ENABLE);
	check_SI_Flag();
	while(!Check_Status_Flag(0x18));
	Clear_SI_Flag();
	while(len >0){
		
		I2C0DAT = *buf;
		buf++;
		len--;
		check_SI_Flag();
		while(!Check_Status_Flag(0x28));
		Clear_SI_Flag();
	}
	I2C0_Stop();
	return 1;
	}

uint8_t I2C0_MasterReadData(uint8_t slave_add, uint8_t *buf, uint8_t len){
	I2C0_Start();
	check_SI_Flag();
	while(!Check_Status_Flag(0x08));
	Clear_SI_Flag();
	I2C0DAT = slave_add;
	I2C0_Ack(ENABLE);
	check_SI_Flag();
	while(!Check_Status_Flag(0x40));
	Clear_SI_Flag();
	while(len == 1){
		I2C0_Ack(DISABLE);
		*buf = I2C0DAT;
		check_SI_Flag();
		while(!Check_Status_Flag(0x50));
		len--;
		Clear_SI_Flag();
		if(Check_Status_Flag(0x58)){
			I2C0_Stop();
			
		}
	}
	while(len>1){
		*buf = I2C0DAT;
		check_SI_Flag();
		while(!Check_Status_Flag(0x50));
		len--;
		Clear_SI_Flag();
		}
	return *buf;
	}*/


void I2C0_Stop(void){
	
	I2C0CONSET = STOP;
	while(!(I2C0CONSET & STOP)){}
  Clear_SI_Flag();
}