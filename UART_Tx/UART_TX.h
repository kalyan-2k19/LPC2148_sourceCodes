#include <LPC214X.H>
#include <stdint.h>

#define TX_STATUS		(1<<5)


void UART0_Init(void);
void UART0_TX(char data);
void UART0_TX_String(const char *data);

void UART0_Init(void){
	PINSEL0 |= 0x05;			// 0000 0101 select Rx and Tx 
	
	U0FCR |= 0x07;   // Enable FIFO register and reset UART0 TX and RX
	U0LCR |= 0x83;	// DLAB =1, and 8bit word length selected.
	U0DLM = 1;
	U0DLL = 134;		// setting baudrate for 9600 bits/sec
	U0LCR = 0x03;		// Disable DLAB = 0.and set 8-bit word.
}

void UART0_TX(char data){
	U0THR = data;
	while(!(U0LSR & TX_STATUS));
}
void UART0_TX_String(const char *data){
	while(*data!='\0'){
		UART0_TX(*data);
		data++;
	}
}


	
	
	
	
	
	


