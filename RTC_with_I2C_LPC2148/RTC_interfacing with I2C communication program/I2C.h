#include <LPC214X.H>
#include <stdint.h>
#define I2EN (1<<6) //Enable/Disable bit
#define STA  (1<<5) //Start Set/Clear bit
#define STO  (1<<4) //Stop bit
#define SI   (1<<3) //Serial Interrupt Flag Clear bit
#define AA   (1<<2) //Assert Acknowledge Set/Clear bit


void I2C0Init(void);
uint8_t I2C0WaitForSI(void);
void I2C0SendStart(void);
void I2C0SendStop(void);
void I2C0TX_Byte(unsigned char data);
unsigned char I2C0RX_Byte(uint8_t isLast);
void I2C0Init(void) 
{
	PINSEL0 |= (0<<7)|(1<<6)|(0<<5)|(1<<4); //Select SCL0(P0.2) and SDA0(P0.3)
	I2C0SCLL = 300;
	I2C0SCLH = 300; //I2C0 @ 100Khz, given PCLK @ 60Mhz
	I2C0CONCLR = STA | STO | SI | AA; //Clear these bits
	I2C0CONSET = I2EN; //Enable I2C0
	//After this we are ready to communicate with any other device connected to the same bus.
}

uint8_t I2C0WaitForSI(void) //Wait till I2C0 block sets SI
{
	int timeout = 0;
	while ( !(I2C0CONSET & SI) ) //Wait till SI bit is set. This is important!
	{
		timeout++;
		if (timeout > 10000) return 0; //In case we have some error on bus
	}
	return 1; //SI has been set
}



void I2C0SendStart(void)
{
	I2C0CONCLR = STA | STO | SI | AA; //Clear everything
	I2C0CONSET = STA; //Set start bit to send a start condition
	I2C0WaitForSI(); //Wait till the SI bit is set
}

void I2C0SendStop(void)
{
	int timeout = 0;
	I2C0CONSET = STO ; //Set stop bit to send a stop condition
	I2C0CONCLR = SI;
	while (I2C0CONSET & STO) //Wait till STOP is send. This is important!
	{
		timeout++;
		if (timeout > 10000) //In case we have some error on bus
		{
			//printf("STOP timeout!\n");
			return;
		}
	}
}

uint8_t checkStatus(uint8_t statusCode) {
if(I2C0STAT!=statusCode) 
{ 
	//printf("Error! Expected status code: %i(decimal), Got: %i(decimal)\n",statusCode,I2C0STAT); 
	I2C0SendStop(); 
	return 0; 
}
return 1;
}
void I2C0TX_Byte(unsigned char data)
{
 	I2C0DAT = data;
	I2C0CONCLR = STA | STO | SI; //Clear These to TX data
	I2C0WaitForSI(); //wait till TX is finished
}

unsigned char I2C0RX_Byte(uint8_t isLast)
{
	if(isLast) I2C0CONCLR = AA; //Send NACK to stop; I2C block will send a STOP automatically, so no need to send STOP thereafter.
	else 	     I2C0CONSET = AA; //Send ACK to continue
	I2C0CONCLR = SI; //Clear SI to Start RX
	I2C0WaitForSI(); //wait till RX is finished
	return I2C0DAT;
}